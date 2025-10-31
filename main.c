#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
//#include "heap.c"

float **geraConjuntoDeDados(float *C, int nc, int d) {

}

float **computeKNN(float *Q, int nq, float *P, int np, int D, int k) {
    float **R = malloc(sizeof(float *)*nq);
    for(int i = 0; i < k; ++i)
        R[i] = malloc(sizeof(float)*k*D);

    
}

void verifyKNN(float *Q, int nq, float *p, int np, int D, int k, int *R) {
    printf("------ VERIFICA KNN ------\n");

    for(int linha = 0; linha < nq; linha++) {
        printf("knn[%d]", linha);
        for( int coluna=0; coluna<k; coluna++)
            printf("%d ", R[ linha*k+coluna ]);
        printf("\n");
    }
}

// Por enquanto apenas testando a biblioteca MPI
int main(int argc, char** argv) {
    int nq, np, d, k;

    float *Q = malloc(sizeof(float)*nq*d);
    float *P = malloc(sizeof(float)*np*d);

    geraConjuntoDeDados(Q, nq, d);
    geraConjuntoDeDados(P, np, d);

    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
    processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
}

