/*
 * @Author: Pablo Lobato and Pablo Seijo
 * @Company: USC ETSE
 * @Date: 13/3/2023
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void fillMatrix (double* matrix,int line, int column, bool isRandom);
void start_counter();
double get_counter();
double mhz();

/* Initialize the cycle counter */

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
    int *ind, *posicionLibre; // vector desordenado aleatoriamente que contiene índices de fila/columna sin que se repitan.
    int i = 0, j = 0, k = 0; // indices usados en los bucles
    int random, size, filas, columnas; // variables a usar en las operaciones

    if(argc != 2){
        perror("\nERROR: use example: ./p2_ap1 N\n\n");
        exit(EXIT_FAILURE);
    }
    // Igualamos tamanho por el valor que le pasamos por linea de comandos
    size = atoi(argv[1]);

    if (size <= 0 ) {
        perror("\nERROR: wrong value for N\n");
        exit(EXIT_FAILURE);
    }

    // Con srand() establecemos la semilla del generador de numeros aleatorios
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

    // Inicializamos c, e, posicionLibre e ind
    c = (double *) malloc(8 * sizeof(double));
    e = (double *) malloc(size * sizeof(double));
    posicionLibre = (int *) malloc(size * sizeof(int));
    ind = (int *) malloc(size * sizeof(int));



    for (i = 0; i < size; i++) {
        posicionLibre[i] = 0;
    }

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
    // Le pasamos el puntero de la primera fila y la primera columna para despues moverlo en la funcion a la fila y columna deseada
    fillMatrix(&a[i * columnas + j], size, 8, true);
    fillMatrix(&b[i * columnas + j], 8, size, true);
    // Para rellenar el array le decimos que es una matriz 1x8 ( si lo pensamos realmente es lo que es un array bidimensional )
    fillMatrix(&c[0], 1, 8, true);

    start_counter();
    // D
    filas = size;
    columnas = size;
    d = (double *) malloc(filas * columnas * sizeof(double));
    
    fillMatrix(&d[i * columnas + j], size, size, false);

    columnas = size;
    // Realizamos las operaciones
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            for (k = 0; k < 8; k++) {
                d[i * columnas + j] += 2.0 * a[i * 8 + k] * (b[k * columnas + j] - c[k]);
            }
        }
    }

    for (i = 0; i < size; i ++) {
        e[i] = d[ind[i] * size + ind[i]] / 2;
        f += e[i];
    }

    ck = get_counter();

    // Liberaciones de memoria
    free(a); free(b); free(c); free(e); free(d);  free(posicionLibre); free(ind);

    printf("\nf: %1.10lf", f);

    // imprimimos el clock
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





