/*
 * @Author: Pablo Lobato and Pablo Seijo
 * @Company: USC ETSE
 * @Date: 13/3/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>
#include <string.h>
#include <math.h>
#include "unistd.h"

#define LINE_SIZE 64.0
#define NUMERO_EJECUCIONES 10

/* Inicializar el contador de ciclos */
static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;

/*
 * @param int* ind = vector dinamico donde queremos almacenar los valores (cada posicion sera la posicion del byte en el salto "i")
 * @param double R = valor de R (tamaño del array y numero de saltos)
 * @param int D = valor de D (numero por el que se mulitplica cada posicion del array y tamaño de salto en bytes)
 * @description esta funcion rellena el array dinamico con los valores de D multiplicandolos por cada valor de i = 0,1,2,3...R-1
 */
void FillArray (int* ind,int R, int D);

/*
* @param double* A = vector dinamico de donde sumaremos los numeros
* @param int* ind = vector donde almacenamos los indices
* @param double R = valor de R (tamaño del array)
* @description esta funcion rellena el array dinamico A con valores random entre [1,2) en valor absoluto
*/
void Fill_A(double* A, int R, int D);
void access_counter(unsigned *hi, unsigned *lo);
void start_counter();
double get_counter();
double mhz(int verbose, int sleeptime);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Main*/
int main(int argc, char* argv[]) {
    double *A = NULL;
    double S[NUMERO_EJECUCIONES], ciclos[NUMERO_EJECUCIONES]; //array donde se almacenan los resultados para hacer la media
    int *ind = NULL; //vector que será el índice para acceder a A[]
    int D = 0; // tamaño de los saltos de linea de cache
    int R = 0; // cantidad de saltos
    double ck = 0.0, resultadoSuma = 0.0, ckTotal = 0.0; //variables para almacenar los resultados de los ciclos y de la suma
    double mediaCiclos=0.0, mediaSuma=0.0; //variables para almacenar las medias de los ciclos y de la suma
    FILE *fptr; //contendra el archivo donde escribirimos la media de los ciclos

    if(argc != 3){
        perror("\nERROR: use example: ./acp1 D L\n\n");
        exit(EXIT_FAILURE);
    }

    if (atoi(argv[1]) <= 0 ) {
        perror("\nERROR: wrong value for D\n");
        exit(EXIT_FAILURE);
    }

    if (atoi(argv[2]) <= 0 ) {
        perror("\nERROR: wrong value for L\n");
        exit(EXIT_FAILURE);
    }

    //abrimos el archivo
    fptr = fopen("resultados.txt", "a");
    if(fptr == NULL){
        perror("\nERROR: file does not exist \n\n");
        exit(EXIT_FAILURE);
    }

    //Asignamos los valores a D y R mediante la conversion de char a int
    D = atoi(argv[1]);

    /* Obtenemos R (el numero de saltos) multiplicando el numero de lineas por el tamaño de la linea, lo dividimos por el
     * tamaño de un double (8 bytes) por el tamaño de salto para obtener el numero de saltos para rellenar 8 lineas,
     * y le restamos el tamaño de linea menos D para obtener el numero minimo de saltos para tener 8 lineas*/
     if (D < 8){
        R = ceil(( 1.0 * (LINE_SIZE * (atoi(argv[2]) - 1) ) / (sizeof(double) * D ) ) )  + 1;  
     } else {
     	R = atoi(argv[2]); //R=L
     }
     
     
    //Reservamos memoria para el array dinamico
    A = (double *) _mm_malloc((D * R * sizeof(double)),
                              LINE_SIZE); //Mediante esta funcion alineamos la reserva con el inicio de una linea cache
    //Reservamos memoria para el array auxiliar
    ind = (int *) malloc(R * sizeof(int));

    if (A == NULL || ind == NULL) {
             printf("ERROR: memory has NOT been reserved");
             exit(1);
    }
     FillArray(ind, R, D);

     for(int i=0; i<NUMERO_EJECUCIONES ; i++) {
         //Inicializamos los vectores dinamicos
         resultadoSuma = 0.0;
         Fill_A(A, R, D);
         
         //Sumamos los elementos
         start_counter();

         for (int indice = 0; indice < R; indice++) {
             resultadoSuma += A[ind[indice]];
                      // ckTotal += ck / R;
         } 
         ck = get_counter();

         ciclos[i] = ck;
         S[i] = resultadoSuma;
     }

     //Sumamos los valores del vector
     for(int i = 0; i < NUMERO_EJECUCIONES; i++) {
     	 printf("\n%1.10lf \n", S[i]);
         mediaCiclos += ciclos[i];
         mediaSuma += S[i];
     }

     mediaSuma = mediaSuma / NUMERO_EJECUCIONES;
     mediaCiclos = mediaCiclos / (NUMERO_EJECUCIONES * R);
     fprintf(fptr,"%1.10lf \n", mediaCiclos);
    //Liberamos los punteros
    _mm_free(A);
    free(ind);

    // Cerrar el archivo
    fclose(fptr);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Funciones*/


void FillArray(int* ind, int R, int D) {

    for(int i = 0; i < R; i++){
        ind[i] = i * D;
    }
}



void Fill_A(double* A, int R, int D){

    for(int i = 0; i < R * D; i++){
        //Para hacer el intervalo en valor absoluto entre [1,2) hacemos lo siguiente
        //Cuando i es un nunmero par Sacamos un numero aleatorio (0. algo) y le sumamos uno para que este dentro del intervalo [1,2)
        if( (i%2) == 0) {
            int numeroRandom = rand();

            //Para asegurarnos que el valor nunca sea 2, randomizamos el numero hasta que sea diferente de RAND_MAX
            while(numeroRandom == RAND_MAX) {
                numeroRandom = rand();
            }

            A[i] = (numeroRandom / (float) RAND_MAX) + 1;

        }

            //si es impar sumamos -2, para generar números en el intervalo (-2, 1]
        else{ A[i] = (rand() / (float) RAND_MAX) - 2;}
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

double mhz(int verbose, int sleeptime){
    double rate;

    start_counter();
    sleep(sleeptime);
    rate = get_counter() / (1e6*sleeptime);
    if (verbose)
        printf("\n Processor clock rate = %.1f MHz\n", rate);
    return rate;
}

