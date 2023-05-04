/*
 * @Author: Pablo Lobato and Pablo Seijo
 * @Company: USC ETSE
 * @Date: 27/4/2023
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

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
    double *a = NULL, *b = NULL, *c = NULL, *e = NULL; // matrices y vector que almacenan valores aleatorios de tipo double.
    double *d = NULL; // matriz que rellenamos con 0
    double f = 0.0; // variable de salida
    double ck; // double donde meteremos el numero de ciclos de reloj

    // Enteros
    int *ind, *posicionLibre; //vector desordenado aleatoriamente que contiene índices de fila/columna sin que se repitan.
    int i = 0, j = 0, k = 0; // indices usados en los bucles
    int random, size, numeroHilos, filas, columnas; // variables a usar en las operaciones
    FILE* fResultados;

    if(argc != 3){
        perror("\nERROR: use example: ./p2_apartado4 N threads\n\n");
        exit(EXIT_FAILURE);
    }
    // Igualamos tamanho por el valor que le pasamos por linea de comandos
    size = atoi(argv[1]);
    numeroHilos = atoi(argv[2]);

    if (size <= 0 ) {
        perror("\nERROR: wrong value for N\n");
        exit(EXIT_FAILURE);
    }

    if (numeroHilos <= 0 ) {
        perror("\nERROR: wrong value for threads\n");
        exit(EXIT_FAILURE);
    }

    fResultados = fopen("resultados_apartado4.txt", "a");

    if (fResultados == NULL) {
        printf("Error al abrir el archivo\n");
        exit(EXIT_FAILURE);
    }

    // Con srand() establecemos la semilla del generador de numeros aleatorios
    srand(34);

    /* Inicializamos las matrices y vectores */

    // A
    filas = size;
    columnas = 8;
    a = (double *) malloc(filas  * columnas * sizeof(double));

    // B
    filas = 8;
    columnas = size;
    b = (double *) malloc(filas * columnas * sizeof(double));

    // Inicializamos c, e, posicionLibre e ind
    c = (double *) malloc(8 * sizeof(double));
    e = (double *) malloc(size * sizeof(double));
    posicionLibre = (int *) calloc(size, sizeof(int));
    ind = (int *) malloc(size * sizeof(int));


    // En este for rellenamos el vector ind con numeros aleatorios entre 0 y tamanho, y cuando ocupamos la posicion ponemos un 1
    // en el vector posicion libre para saber que su posicion esta ocupada y asi no repetir numeros
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


    start_counter();

    // D
    filas = size;
    columnas = size;

    // Inicializamos d con calloc para que todos sus argumentos sean 0 y nos ahorramos la llamada a la funcion, asi como sus bucles
    d = (double *) calloc(filas * columnas, sizeof(double));

    columnas = size;

    /* Realizamos las operaciones */

    //Como cambio del primer apartado a este desenrrollamos el bucle de operaciones, para buscar una mejor optimizacion
    //temporal:

    #pragma omp parallel shared(a, b, c, d, e, f, k) private(i, j)  num_threads (numeroHilos)
    {
        #pragma omp for schedule(static)
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                k = 0;
                //Desenrrollamos el bucle de k de tal manera que no lo ponemos (k hacia 8 iteraciones)
                d[i * columnas + j] += 2.0 * a[i * 8 + k] * (b[k * columnas + j] - c[k]);
                d[i * columnas + j] += 2.0 * a[i * 8 + (k + 1)] * (b[(k + 1) * columnas + j] - c[k + 1]);
                d[i * columnas + j] += 2.0 * a[i * 8 + (k + 2)] * (b[(k + 2) * columnas + j] - c[k + 2]);
                d[i * columnas + j] += 2.0 * a[i * 8 + (k + 3)] * (b[(k + 3) * columnas + j] - c[k + 3]);
                d[i * columnas + j] += 2.0 * a[i * 8 + (k + 4)] * (b[(k + 4) * columnas + j] - c[k + 4]);
                d[i * columnas + j] += 2.0 * a[i * 8 + (k + 5)] * (b[(k + 5) * columnas + j] - c[k + 5]);
                d[i * columnas + j] += 2.0 * a[i * 8 + (k + 6)] * (b[(k + 6) * columnas + j] - c[k + 6]);
                d[i * columnas + j] += 2.0 * a[i * 8 + (k + 7)] * (b[(k + 7) * columnas + j] - c[k + 7]);
            }
        }

        #pragma omp for schedule(static)
        for (i = 0; i < size; i++) {
            e[i] = d[ind[i] * size + ind[i]] / 2;
        }

    }

    for (i = 0; i < size; i++) {
        f += e[i];
    }

    ck = get_counter();

    // Liberaciones de memoria
    free(a); free(b); free(c); free(e); free(d);  free(posicionLibre); free(ind);

    //Imprimimos nuestro resultado
    printf("\nf: %1.10lf", f);

    // imprimimos el clock
    // printf("\nClocks = %1.2lf \n",ck);
    fprintf(fResultados, "%1.2lf \n",ck);
    fclose(fResultados);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////* FUNCIONES DE OPERACION *////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
/////////////////////////////////////* FUNCIONES DE OPERACION DE RELOJ *////////////////////////////////////////////////
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