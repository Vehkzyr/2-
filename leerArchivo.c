#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#define NUMERO_DATOS 350
#define N 10


void insertionSort(double *datos);

int main() {
    FILE *fResultados, *fMenoresDatos; //archivo inicial y final
    double resultados[NUMERO_DATOS], datosIndividuales[N], geo, mult;
    char linea[100];
    int i = 0, j = 0, indice = 0;

    fResultados = fopen("resultados.txt", "r");
    fMenoresDatos = fopen("menoresDatos.txt", "w");

    if (fResultados == NULL || fMenoresDatos == NULL) {
        perror("ERROR: fopen");
        exit(1);
    }

    //cogemos los 350 datos del archivo
    for (i = 0; i < 351; i++) {
        fgets(linea, 100, fResultados);
        resultados[i] = atof(linea); 
    }

    //recorrermo el array procesando las 35 combinaciones diferentes de L y D
    for ( i = 0; i < 35; i ++) {
        j = i;
        

        //Sqaltamos de 35 y 35, frecuencia con la que se repite la combinacion de L y D, 10 datos
        while (j < 350) {
            datosIndividuales[indice] = resultados[j];
            indice++;
            j+=35;
        }

        if (indice == 10){
        //ordenamos de menor a mayor
        insertionSort(datosIndividuales);
        //fprintf(fMenoresDatos, "%1.10lf %1.10lf %1.10lf\n", datosIndividuales[0], datosIndividuales[1], datosIndividuales[2]);
        //multiplicacion de los 3 menores
        mult = datosIndividuales[0] * datosIndividuales[1] * datosIndividuales[2];
        //media geometrica, 0.33 porque 1/3 no funciona correctamente
        geo = pow(mult, 0.33333333f);
        //imprimimos en el archivo
        fprintf(fMenoresDatos, "%1.10lf \n", geo);
        indice = 0;
        }
    }

    fclose(fResultados);
    fclose(fMenoresDatos);
}

//algoritmo burbuja para ordenar de menor a mayor
void insertionSort(double *datos) {
    double aux;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N-1-i; j++) {
            if (datos[j] > datos[j+1]) {
                aux = datos[j];
                datos[j] = datos[j+1];
                datos[j+1] = aux;
            }
        }
    }
}
