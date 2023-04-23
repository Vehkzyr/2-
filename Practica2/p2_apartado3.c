/*
 * @Author: Pablo Lobato and Pablo Seijo
 * @Company: USC ETSE
 * @Date: 13/3/2023
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <immintrin.h>

double AVXMatrixOperation(double *a, double *b, double *c, double *d, int* ind, int size, int columnas);
void fillMatrix (double* matrix,int line, int column);

void start_counter();
double get_counter();
double mhz();

/* Inicializamos el contador de ciclos */

static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;

int main(int argc, char* argv[]) {

    /* Inicializamos las variables */
    //Doubles
    double *a = NULL, *b = NULL, *c = NULL, *e = NULL; //matrices y vector que almacenan valores aleatorios de tipo double.
    double *d = NULL; //matriz que rellenamos con 0
    double f = 0.0; //variable de salida
    double ck; //double donde meteremos el numero de ciclos de reloj

    //Enteros
    int *ind, *posicionLibre; //vector desordenado aleatoriamente que contiene índices de fila/columna sin que se repitan.
    int i = 0, j = 0, k = 0; // indices usados en los bucles
    int random, size, filas, columnas; // variables a usar en las operaciones

    if(argc != 2){
        perror("\nERROR: use example: ./p2_ap1 N\n\n");
        exit(EXIT_FAILURE);
    }
    //Igualamos tamanho por el valor que le pasamos por linea de comandos
    size = atoi(argv[1]);

    if (size <= 0 ) {
        perror("\nERROR: wrong value for N\n");
        exit(EXIT_FAILURE);
    }

    //Con srand() establecemos la semilla del generador de numeros aleatorios
    srand(34);

    /* Inicializamos las matrices y vectores */

    //A
    filas = size;
    columnas = 8;
    a = (double *) malloc(filas  * columnas * sizeof(double));

    //B
    filas = 8;
    columnas = size;
    b = (double *) malloc(filas * columnas * sizeof(double));

    //D
    filas = size;
    columnas = size;
    //Inicializamos d con calloc para que todos sus argumentos sean 0 y nos ahorramos la llamada a la funcion, asi como sus bucles
    d = (double *) calloc(filas * columnas, sizeof(double));

    //Inicializamos c, e, posicionLibre e ind
    c = (double *) malloc(8 * sizeof(double));
    e = (double *) malloc(size * sizeof(double));
    posicionLibre = (int *) malloc(size * sizeof(int));
    ind = (int *) malloc(size * sizeof(int));

    start_counter();

    //Incializamos un vector con N posiciones de tal manera que sabemos que esta posicion esta libre
    for (i = 0; i < size - 4; i += 4) {
        posicionLibre[i] = 0;
        posicionLibre[i+1] = 0;
        posicionLibre[i+2] = 0;
    }

    //En este for rellenamos el vector ind con numeros aleatorios entre 0 y tamanho, y cuando ocupamos la posicion ponemos un 1
    //en el vector posicion libre para saber que su posicion esta ocupada y asi no repetir numeros
    for (i = 0; i < size; i++){
        random = rand() % size;
        while (posicionLibre[random] == 1) random = rand() % size;

        ind[random] = i;
        posicionLibre[random] = 1;
    }

    /* Rellenamos las matrices y vectores con numeros aleatorios */
    i = 0; j = 0; columnas = 0;
    //Le pasamos el puntero de la primera fila y la primera columna para despues moverlo en la funcion a la fila y columna deseada
    fillMatrix(&a[i * columnas + j], size, 8);
    fillMatrix(&b[i * columnas + j], 8, size);
    //Para rellenar el array le decimos que es una matriz 1x8 ( si lo pensamos realmente es lo que es un array bidimensional )
    fillMatrix(&c[0], 1, 8);

    columnas = size;

    /* Realizamos las operaciones */

    //Transformamos las operaciones a AVX y para simplicidad en la lectura del codigo lo hacemos en una funcion a parte
    f = AVXMatrixOperation(a, b, c, d, ind, size, columnas);

    ck = get_counter();

    // Liberaciones de memoria
    free(a); free(b); free(c); free(d); free(e); free(posicionLibre); free(ind);

    //Imprimimos nuestro resultado
    printf("\nf: %1.10lf", f);

    // imprimimos el clock
    printf("\nClocks = %1.2lf \n",ck);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////* FUNCIONES DE OPERACION *////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* @param double* a = puntero que apunta a la posicion de a
* @param double* b = puntero que apunta a la posicion de b
* @param double* c = puntero que apunta a la posicion de c
* @param double* d = puntero que apunta a la posicion de d
* @param double* ind = puntero que apunta a la posicion de ind
* @param int size = numero que determina el tamaño de la matriz d (aunque en el codigo tmb influye en el resto)
* @param int columnas = numero de columnas
* @description realiza las operaciones indicadas con AVX (Advanced Vector Extensions), que permiten realizar operaciones
 *             de cálculo intensivo de forma más eficiente en aplicaciones que requieren el procesamiento de grandes
 *             cantidades de datos
* @return devuelve un double con las operaciones realizadas
*/
double AVXMatrixOperation(double *a, double *b, double *c, double *d,  int *ind, int size, int columnas) {
    int i, j, k;

    /* Operaciones sobre d */
    for (i = 0; i < size; i++) {
        //Comprobamos si size es divisible entre 8 para poder hacer las operaciones
        if(size % 8 == 0) {
            for (j = 0; j < size; j += 8) {
                //Declaramos las variables en AVX
                __m256d a_vec, b_vec, c_vec, d_vec;
                __m256d constant = _mm256_set1_pd(2.0);

                //Cargamos las matrices y vectores en nuestras variables
                d_vec = _mm256_loadu_pd(&d[i * columnas + j]);
                c_vec = _mm256_loadu_pd(c);
                a_vec = _mm256_loadu_pd(&a[i * 8]);
                b_vec = _mm256_loadu_pd(&b[columnas + j]);

                //Realiza las operaciones de suma e iguala d a ello
                d_vec = _mm256_fmadd_pd(constant, a_vec, d_vec);
                d_vec = _mm256_fmadd_pd(constant, _mm256_sub_pd(b_vec, c_vec), d_vec);

                //Se almacenan los valores en d_vec
                _mm256_storeu_pd(&d[i * columnas + j], d_vec);
            }
        }

        else{
            for (j = 0; j < size; j ++) {
                //Declaramos las variables en AVX
                __m256d a_vec, b_vec, c_vec, d_vec;
                __m256d constant = _mm256_set1_pd(2.0);

                //Cargamos las matrices y vectores en nuestras variables
                d_vec = _mm256_loadu_pd(&d[i * columnas + j]);
                c_vec = _mm256_loadu_pd(c);
                a_vec = _mm256_loadu_pd(&a[i * 8]);
                b_vec = _mm256_loadu_pd(&b[columnas + j]);

                //Realiza las operaciones de suma e iguala d a ello
                d_vec = _mm256_fmadd_pd(constant, a_vec, d_vec);
                d_vec = _mm256_fmadd_pd(constant, _mm256_sub_pd(b_vec, c_vec), d_vec);

                //Se almacenan los valores en d_vec
                _mm256_storeu_pd(&d[i * columnas + j], d_vec);
            }
        }
    }

    /* Operaciones para el calculo de f */

    // Inicia una variable __m256d sum, que es un vector de 256 bits que contiene 4 números en punto flotante de doble precisión, y lo inicializa en cero.
    __m256d sum = _mm256_setzero_pd();

    // Para cada índice "i" en el rango [0, size):
    for (i = 0; i < size; i++) {
        // Calcula el índice "index" como el índice de la matriz d correspondiente a la fila y columna especificadas por los valores de ind[i].
        int index = ind[i] * size + ind[i];
        // Crea un vector __m256d "val" que contiene el valor de d[index], copiando ese valor en cada posición del vector.
        __m256d val = _mm256_broadcast_sd(&d[index]);
        // Agrega el vector "val" al vector "sum" utilizando la operación "addition" de AVX.
        sum = _mm256_add_pd(sum, val);
    }

    // Crea un arreglo "tmp" de 4 elementos en doble precisión y alineado a 32 bytes.
    double tmp[4]__attribute__((aligned(32)));

    // Almacena el contenido del vector "sum" en el arreglo "tmp".
    _mm256_store_pd(tmp, sum);

    // Devuelve la suma de los valores del arreglo "tmp"
    return tmp[0] + tmp[1] + tmp[2] + tmp[3];
}

/*
* @param double* matrix = puntero que apunta a la posicion de la matrix
* @param int line = numero de filas de la matrix
* @param int column = numero de columnas
* @param bool isRandom = si se quiere rellenar la matriz de valores aleatorios o no
* @description esta funcion rellena matrices de numeros aleatorios si se le indica o 0 si no
* @return retorna la matriz pasada como argumento rellena de lo indicado (random o 0)
 * */
void fillMatrix (double* matrix, int line, int column) {
    //Recorremos las filas con este primer for
    for(int i = 0; i < line; i++) {
        //Recorremos las columnas con este segundo for
        for (int j = 0; j < column; j++) {
            //Mediante este arreglo metemos los valores aleatorios e cada posicion, moviendo el puntero a la siguiente posicion
            *(matrix + i * column + j) = ((double) rand() / (double) RAND_MAX) * 100;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////* FUNCIONES DE RELOJ *////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Set *hi and *lo to the high and low order bits of the cycle counter.
Implementation requires assembly code to use the rdtsc instruction. */
void access_counter(unsigned *hi, unsigned *lo)
{
    asm("rdtsc; movl %%edx,%0; movl %%eax,%1" /* Read cycle counter */
            : "=r" (*hi), "=r" (*lo) /* and move results to */
            : /* No input */ /* the two outputs */
            : "%edx", "%eax");
}

/* Record the current value of the cycle counter. */
void start_counter()
{
    access_counter(&cyc_hi, &cyc_lo);
}

/* Return the number of cycles since the last call to start_counter. */
double get_counter()
{
    unsigned ncyc_hi, ncyc_lo;
    unsigned hi, lo, borrow;
    double result;

    /* Get cycle counter */
    access_counter(&ncyc_hi, &ncyc_lo);

    /* Do double precision subtraction */
    lo = ncyc_lo - cyc_lo;
    borrow = lo > ncyc_lo;
    hi = ncyc_hi - cyc_hi - borrow;
    result = (double) hi * (1 << 30) * 4 + lo;
    if (result < 0) {
        fprintf(stderr, "Error: counter returns neg value: %.0f\n", result);
    }
    return result;
}

double mhz(int verbose, int sleeptime)
{
    double rate;

    start_counter();
    sleep(sleeptime);
    rate = get_counter() / (1e6*sleeptime);
    if (verbose)
        printf("\n Processor clock rate = %.1f MHz\n", rate);
    return rate;
}
