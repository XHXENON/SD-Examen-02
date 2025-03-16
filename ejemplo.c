#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

// Función para obtener el tiempo
void get_wall_time(double* wcTime) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    *wcTime = (tp.tv_sec + tp.tv_usec / 1000000.0);
}

// Función para inicializar matrices
void init_matrices(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 6;
            B[i][j] = rand() % 6;
            C[i][j] = 0;
        }
    }
}

// 6 versiones diferentes de multiplicación de matrices
void multiply_ijk(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
}

void multiply_ikj(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                C[i][j] += A[i][k] * B[k][j];
}

void multiply_jik(int** A, int** B, int** C, int n) {
    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
}

void multiply_jki(int** A, int** B, int** C, int n) {
    for (int j = 0; j < n; j++)
        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                C[i][j] += A[i][k] * B[k][j];
}

void multiply_kij(int** A, int** B, int** C, int n) {
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                C[i][j] += A[i][k] * B[k][j];
}

void multiply_kji(int** A, int** B, int** C, int n) {
    for (int k = 0; k < n; k++)
        for (int j = 0; j < n; j++)
            for (int i = 0; i < n; i++)
                C[i][j] += A[i][k] * B[k][j];
}

int main(int argc, char* argv[]) {
    int sizes[] = {100, 500, 1000};
    double start, end, fill_start, fill_end;
    
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        printf("\n=== Resultados para matrices %dx%d ===\n", n, n);
        
        // Asignación de memoria
        int** A = (int**)malloc(n * sizeof(int*));
        int** B = (int**)malloc(n * sizeof(int*));
        int** C = (int**)malloc(n * sizeof(int*));
        for (int i = 0; i < n; i++) {
            A[i] = (int*)malloc(n * sizeof(int));
            B[i] = (int*)malloc(n * sizeof(int));
            C[i] = (int*)malloc(n * sizeof(int));
        }

        // Medir tiempo de llenado
        get_wall_time(&fill_start);
        init_matrices(A, B, C, n);
        get_wall_time(&fill_end);
        printf("Tiempo de llenado: %f s\n", (fill_end - fill_start));

        // Probar cada combinación de bucles
        void (*methods[])(int**, int**, int**, int) = {
            multiply_ijk, multiply_ikj, multiply_jik,
            multiply_jki, multiply_kij, multiply_kji
        };
        const char* names[] = {"ijk", "ikj", "jik", "jki", "kij", "kji"};

        for (int m = 0; m < 6; m++) {
            // Reiniciar matriz C
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    C[i][j] = 0;

            get_wall_time(&start);
            methods[m](A, B, C, n);
            get_wall_time(&end);
            printf("Tiempo método %s: %f s\n", names[m], (end - start));
        }

        // Liberar memoria
        for (int i = 0; i < n; i++) {
            free(A[i]); free(B[i]); free(C[i]);
        }
        free(A); free(B); free(C);
    }
    return 0;
}