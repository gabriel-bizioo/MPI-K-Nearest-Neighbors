#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

//#include "heap.c"
#include "verificaKNN.c"

void geraConjuntoDeDados(float *C, int nc, int d) {
    for(int i = 0; i < nc; ++nc) {
        for(int j = 0; j < d; ++j)
            C[i + j] = (float)((double)rand()/(double)(RAND_MAX/1000));
    }
}

float **computeKNN(float *Q, int nq, float *P, int np, int D, int k) {
    // nq linhas
    float **R = malloc(sizeof(float *)*nq);
    float **dR = malloc(sizeof(float *)*nq);
    // k colunas
    for(int i = 0; i < k; ++i) {
        R[i] = malloc(sizeof(float)*k*D);
        dR[i] = malloc(sizeof(float)*k*D);
    }

    int qIndex, pIndex;
    qIndex = pIndex = 0;
    for(int i = 0; i < nq; ++i) {
        for(int j = 0; j < np; ++j) {

            float mul;
            float distancia = 0.0;
            for(int l = 0; l < D; ++l) {
                mul = 0;

                mul = P[pIndex + l] - Q[qIndex + l];
                mul *= mul;
                distancia += mul;
            }
            //reduceMax(distancia);
            pIndex += D;
        }
        qIndex += D;
    }
}

// '/home2/pp/mpi+slurm': exemplos de script slurm
int main(int argc, char** argv) {

    // qtd de pontos em q e p
    int nq, np;
    // qtd de dimensoes em cada ponto, qtd de vizinhos
    int d, k;

    char Args[256];
    nq = np = d = k = 0;
    for(int i = 1; i < argc; i++) {
        strncpy(Args, argv[i], 256);
        if(!strncmp(Args, "nq=", 3)) {
            nq = atoi(Args+3);
            printf("nq=%d\n", nq);
        }
        else if(!strncmp(Args, "npp=", 4)) {
            np = atoi(Args+4);
            printf("npp=%d\n", np);
        }
        else if(!strncmp(Args, "d=", 2)) {
            d = atoi(Args+2);
            printf("d=%d\n", d);
        }
        else if(!strncmp(Args, "k=", 2)) {
            k = atoi(Args+2);
            printf("k=%d\n", k);
        }
        else {
            fprintf(stderr, "ERRO: argumentos invalidos");
            exit(2);
        }
        memset(Args, 0, 256);
    }

    if(!np || !nq || !k || !d){
        fprintf(stderr, "ERRO: argumentos nao especificados ou iguais a 0");
        exit(1);
    }

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

    // Finalize the MPI environment.
    MPI_Finalize();
}

