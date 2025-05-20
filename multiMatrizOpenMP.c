/*#######################################################################################
 #* Fecha: Mayo 2025
 #* Autor: Samuel Emperador
 #* Tema: Programación Modular en C
 #* 	- Programa Multiplicación de Matrices algoritmo clásico
 #* 	- Paralelismo con OpenMP
######################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

struct timeval inicio, fin; /* Para mediciones de tiempo */

/**
 * @brief Inicia la medición de tiempo
 */
void InicioMuestra() {
    gettimeofday(&inicio, (void *)0);
}

/**
 * @brief Finaliza la medición de tiempo e imprime el tiempo transcurrido en microsegundos
 */
void FinMuestra() {
    gettimeofday(&fin, (void *)0);
    fin.tv_usec -= inicio.tv_usec;
    fin.tv_sec  -= inicio.tv_sec;
    double tiempo = (double)(fin.tv_sec*1000000 + fin.tv_usec); 
    printf("Tiempo de ejecución: %9.0f microsegundos\n", tiempo);
}

/**
 * @brief Imprime el contenido de la matriz (solo para matrices pequeñas)
 * 
 * @param matrix Puntero a los datos de la matriz
 * @param D Dimensión de la matriz
 */
void impMatrix(size_t *matrix, int D) {
    if(D < 9) {
        printf("\nContenidos de la matriz:\n");
        for(int i=0; i<D*D; i++) {
            if(i%D==0) printf("\n");
            printf("%zu ", matrix[i]);
        }
        printf("\n**-----------------------------**\n");
    }
}

/**
 * @brief Inicializa las matrices con valores
 * 
 * La matriz A se llena con valores i*2
 * La matriz B se llena con valores i+2
 * 
 * @param m1 Puntero a la matriz A
 * @param m2 Puntero a la matriz B
 * @param D Dimensión de la matriz
 */
void iniMatrix(size_t *m1, size_t *m2, int D) {
    for(int i=0; i<D*D; i++, m1++, m2++) {
        *m1 = i*2; /* Podría usar rand()%10 para valores aleatorios */
        *m2 = i+2; /* Podría usar rand()%10 para valores aleatorios */
    }
}

/**
 * @brief Realiza la multiplicación de matrices usando OpenMP
 * 
 * C = A * B donde A, B y C son matrices D×D
 * 
 * @param mA Puntero a la matriz A
 * @param mB Puntero a la matriz B
 * @param mC Puntero a la matriz resultante C
 * @param D Dimensión de la matriz
 */
void multiMatrix(size_t *mA, size_t *mB, size_t *mC, int D) {
    #pragma omp parallel
    {
        #pragma omp for schedule(static)
        for(int i=0; i<D; i++) {
            for(int j=0; j<D; j++) {
                size_t *pA = mA + i*D;  /* Apunta al inicio de la fila i en la matriz A */
                size_t *pB = mB + j;    /* Apunta al elemento en la fila 0, columna j en la matriz B */
                size_t Suma = 0;
 
                /* Calcula el producto punto de la fila i de A y la columna j de B */
                for(int k=0; k<D; k++, pA++, pB+=D) {
                    Suma += *pA * *pB;
                }
                mC[i*D+j] = Suma;
            }
        }
    }
}

/**
 * @brief Función principal
 * 
 * @param argc Contador de argumentos
 * @param argv Argumentos (tamañoMatriz y numHilos)
 * @return int Estado de salida
 */
int main(int argc, char *argv[]) {
    /* Comprueba los argumentos de línea de comandos */
    if(argc < 3) {
        printf("\nUso: %s tamañoMatriz numHilos\n\n", argv[0]);
        exit(1);
    }

    int N = atoi(argv[1]);  /* Tamaño de la matriz */
    int TH = atoi(argv[2]); /* Número de hilos */
    
    /* Comprueba la validez de la entrada */
    if (N <= 0 || TH <= 0) {
        printf("Error: El tamaño de la matriz y el número de hilos deben ser positivos\n");
        exit(1);
    }

    printf("Multiplicación de matrices usando OpenMP\n");
    printf("Tamaño de matriz: %d x %d\n", N, N);
    printf("Número de hilos: %d\n", TH);

    /* Asigna memoria para las matrices */
    size_t *matrixA = (size_t *)calloc(N*N, sizeof(size_t));
    size_t *matrixB = (size_t *)calloc(N*N, sizeof(size_t));
    size_t *matrixC = (size_t *)calloc(N*N, sizeof(size_t));
    
    /* Comprueba la asignación de memoria */
    if (!matrixA || !matrixB || !matrixC) {
        printf("Falló la asignación de memoria\n");
        exit(2);
    }

    /* Establece la semilla aleatoria */
    srand(time(NULL));

    /* Establece el número de hilos OpenMP */
    omp_set_num_threads(TH);

    /* Inicializa matrices y las imprime (si son lo suficientemente pequeñas) */
    iniMatrix(matrixA, matrixB, N);
    impMatrix(matrixA, N);
    impMatrix(matrixB, N);

    /* Inicia la medición del tiempo y realiza la multiplicación */
    InicioMuestra();
    multiMatrix(matrixA, matrixB, matrixC, N);
    FinMuestra();

    /* Imprime la matriz resultante (si es lo suficientemente pequeña) */
    impMatrix(matrixC, N);

    /* Libera la memoria asignada */
    free(matrixA);
    free(matrixB);
    free(matrixC);
    
    return 0;
}



//CONCLUSIONES
/*
En este codigo se evidencia como es el uso del omp para la programacion en paralelo principalmente usando la directiva de
#pragma omp parallel asi como tambien la directiva #pragma omp schedule(static) la cual distribuye las iteraciones del bucle exterior entre multiples hilos, permitiendo asi calcular disferentes filas de la matriz resultante en paralelo.
Por otro lado tambien se evidencia como es el control de hilos ya que el programa recibe el numero de hilos como parametro de linea de comandos y los configura mediante  la directiva "omp_set_num_trheads(TH) lo que permite al usuario adaptar la ejecucion a las caracteristicas de su hardware.
Esta implementacion muestra como una de las ppocas directivas de OpenMP se puede conseguir una paralelizacion efectiva sin modificar significaente la estrucutura del algoritmo secuencial original demostrando asi la potencia que tiene OpenMP para la programacion en paralelo.

*/
