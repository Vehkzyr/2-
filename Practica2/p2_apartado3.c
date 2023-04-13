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

void fillMatrix (double* matrix,int line, int column, bool isRandom);
void start_counter();
double get_counter();
double mhz();

/* Initialize the cycle counter */

static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;

int main(int argc, char* argv[]) {

    double ck;

    if(argc != 2){
        perror("\nERROR: use example: ./p2_ap1 N\n\n");
        exit(EXIT_FAILURE);
    }

    if (atoi(argv[1]) <= 0 ) {
        perror("\nERROR: wrong value for N\n");
        exit(EXIT_FAILURE);
    }

    //igualamos la N por el valor que le pasamos por linea de comandos
    int N = atoi(argv[1]);

    double a[N][8], b[8][N], c[8], e[N]; //matrices y vector que almacenan valores aleatorios de tipo double.
    int ind[N]; //vector desordenado aleatoriamente que contiene índices de fila/columna sin que se repitan.
    int i, j, k, random;
    int posicionLibre[N];

    //Con srand() establecemos la semilla del generador de numeros aleatorios
    srand(34);

    double f = 0.0; //variable de salida

    double d[N][N]; //matriz que rellenamos con 0

    start_counter();

    //Incializamos un vector con N posiciones de tal manera que sabemos que esta posicion esta libre
    for (i = 0; i < N-4; i += 4) {
        posicionLibre[i] = 0;
        posicionLibre[i+1] = 0;
        posicionLibre[i+2] = 0;
    }

    //En este for rellenamos el vector ind con numeros aleatorios entre 0 y N, y cuando ocupamos la posicion ponemos un 1
    //en el vector posicion libre para saber que su posicion esta ocupada y asi no repetir numeros
    for (i = 0; i < N; i++){
        random = rand()%N;
        while (posicionLibre[random] == 1) random = rand()%N;

        ind[random] = i;
        posicionLibre[random] = 1;
    }

    //Le pasamos el puntero de la primera fila y la primera columna para despues moverlo en la funcion a la fila y columna deseada
    fillMatrix(&a[0][0], N, 8, true);
    fillMatrix(&b[0][0], 8, N, true);
    fillMatrix(&d[0][0], N, N, false);
    //Para rellenar el array le decimos que es una matriz 1x8 ( si lo pensamos realmente es lo que es un array bidimensional )
    fillMatrix(&c[0], 1, 8, true);

    // Creamos un vector de 8 doubles con valor 2.0
    __m256d dos = _mm256_set1_pd(2.0);

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j += 4) {
            __m256d temp = _mm256_setzero_pd(); // Vector de 4 doubles con valor 0.0
            for (k = 0; k < 8; k += 4) {
                __m256d a_vec = _mm256_load_pd(&a[i][k]); // Cargamos un vector de 4 doubles desde a[i][k]
                __m256d b_vec = _mm256_load_pd(&b[k][j]); // Cargamos un vector de 4 doubles desde b[k][j]
                __m256d c_vec = _mm256_load_pd(&c[k]); // Cargamos un vector de 4 doubles desde c[k]

                // Realizamos las operaciones vectoriales
                temp = _mm256_add_pd(temp, _mm256_mul_pd(dos, _mm256_mul_pd(a_vec, _mm256_sub_pd(b_vec, c_vec))));
            }
            _mm256_store_pd(&d[i][j], temp); // Almacenamos el resultado en d[i][j:i+3]
        }
    }


    f = 0;
    for (i = 0; i < N; i++) {
        e[i] = d[ind[i]][ind[i]] / 2;
        f += e[i];
    }

    ck = get_counter();

    printf("\nf: %1.10lf", f);

    //calculamos el tiempo de ejecucion


    //imprimimos el clock
    printf("\nClocks = %1.2lf \n",ck);
}

/*
* @param double* matrix = puntero que apunta a la posicion de la matrix
* @param int line = numero de filas de la matrix
* @param int column = numero de columnas
* @param bool isRandom = si se quiere rellenar la matriz de valores aleatorios o no
* @description esta funcion rellena matrices de numeros aleatorios si se le indica o 0 si no
* @return retorna la matriz pasada como argumento rellena de lo indicado (random o 0)
 * */
void fillMatrix (double* matrix, int line, int column, bool isRandom) {
    //Recorremos las filas con este primer for
    for(int i = 0; i < line; i++) {
        //Recorremos las columnas con este segundo for
        for (int j = 0; j < column; j++) {
            //Si isRandom es true rellenamos la matriz con numeros aleatorios
            if (isRandom){
                //Mediante este arreglo metemos los valores aleatorios e cada posicion, moviendo el puntero a la siguiente posicion
                *(matrix + i * column + j) = ((double) rand() / (double) RAND_MAX) * 100;
            }
                //Si isRandom == false, metemos 0 en cada posicion
            else *(matrix + i * column + j) = 0;
        }
    }
}

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
