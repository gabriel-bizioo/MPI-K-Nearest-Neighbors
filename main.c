#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <mpi.h>

#include "heap.c"
#include "verificaKNN.c"

void geraConjuntoDeDados(float *C, int nc, int d) {
    int total = nc * d;
    for(int i = 0; i < total; i++) {
        C[i] = (float)((double)rand()/(double)(RAND_MAX/1000));
    }
}


// Partes da funcao vao ter que mudar na hora de implementar o MPI
float **computeKNN(float *Q, int nq, float *P, int np, int D, int k) {
    // nq linhas
    int **R = malloc(sizeof(int *) * nq);
    float **dR = malloc(sizeof(float *) * nq);
    // k colunas
    for(int i = 0; i < nq; ++i) {
        R[i] = malloc(sizeof(int) * k);
        dR[i] = malloc(sizeof(float) * k);
    }

    float distancia, mul = 0.0;
    int heapsize, chave, heapindex;
    for(int i = 0; i < nq*D; i+=D) {
        printf("==== Calculando vizinhos de Q[%d] ====\n Pontos de Q[%d]: ", i, i);
        for(int ponto = 0; ponto < D; ponto++)
            printf("%f ", Q[i + ponto]);
        printf("\n============\n");

        heapsize = 0;
        for(int j = 0; j < np*D; j+=D) {

            distancia = 0.0;
            for(int l = 0; l < D; ++l) {
                mul = 0;

                mul = P[j + l] - Q[i + l];
                mul *= mul;
                distancia += mul;
            }

            chave = j == 0 ? 0 : j/D;
            heapindex = i == 0 ? 0 : i/D;
            if(heapsize < k) {
                /* A chave indica qual dos np pontos de p
                 estamos inserindo na heap.
                 Como P eh um vetor e nao matriz,
                 o indice real do ponto = chave * D */
                printf(" Inserindo na maxHeap (tamanho %d):\n  Pontos de P[%d]: ", heapsize, chave);
                for(int ponto = 0; ponto < D; ponto++)
                    printf("%f ", P[j + ponto]);
                printf("\n  Distancia: %f\tChave: %d\n ========", distancia, chave);
                insert(R[heapindex], dR[heapindex], &heapsize,
                        distancia, chave);
                if(!isMaxValueHeap(dR[heapindex], heapsize)){
                    printf("ERRO: Heap quebrou (insert)\n");
                    exit(1);
                }
            }
            else {
                printf(" decreaseMax (tamanho %d):\n  Pontos de P[%d]: ", heapsize, chave);
                for(int ponto = 0; ponto < D; ponto++)
                    printf("%f ", P[j + ponto]);
                printf("\n  Distancia: %f\tChave: %d\n ========\n", distancia, chave);
                decreaseMax(R[heapindex], dR[heapindex], heapsize, distancia, chave);

                if(!isMaxValueHeap(dR[heapindex], heapsize)) {
                    fprintf(stderr, "ERRO: heap quebrou (decreaseMax)\n");
                    exit(1);
                }
            }
        }
    }
    printf("==== Imprimindo heaps ====\n");
    for(int i = 0; i < nq ; i ++) {
        printf(" Pontos com menor distancia de Q[%d]:\n", i);
        drawValueHeapTree(dR[i], k, k);
        printf(" Posicao dos pontos em P:\n");
        drawKeyHeapTree(R[i], k, k);
        printf("\n");
    }

    return dR;
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
        if(!strncmp(Args, "nq=", 3))
            nq = atoi(Args+3);
        else if(!strncmp(Args, "npp=", 4))
            np = atoi(Args+4);
        else if(!strncmp(Args, "d=", 2))
            d = atoi(Args+2);
        else if(!strncmp(Args, "k=", 2))
            k = atoi(Args+2);
        else {
            fprintf(stderr, "ERRO: argumentos invalidos\n");
            exit(1);
        }

        memset(Args, 0, 256);
    }

    if(!np || !nq || !k || !d) {
        fprintf(stderr, "ERRO: argumentos nao especificados ou iguais a 0\n");
        exit(1);
    }

    float *Q = malloc(sizeof(float) * nq * d);
    float *P = malloc(sizeof(float) * np * d);

    geraConjuntoDeDados(Q, nq, d);
    geraConjuntoDeDados(P, np, d);

    printf("==== Pontos de Q: ====\n");
    for(int i = 0; i < nq*d; i+=d) {
        int chave = i == 0 ? 0 : i/d;
        printf("Q[%d]: ", chave);
        for(int j = 0; j < d; j++)
            printf("%f ", Q[i + j]);
        printf("\n");
    }

    printf("==== Pontos de P: ====\n");
    for(int i = 0; i < np*d; i+=d) {
        int chave = i == 0 ? 0 : i/d;
        printf("P[%d]: ", chave);
        for(int j = 0; j < d; j++)
            printf("%f ", P[i + j]);
        printf("\n");
    }

    computeKNN(Q, nq, P, np, d, k);
    // MPI_Init(&argc, &argv);
    //
    // int world_size;
    // MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //
    // int world_rank;
    // MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    //
    // char processor_name[MPI_MAX_PROCESSOR_NAME];
    // int name_len;
    // MPI_Get_processor_name(processor_name, &name_len);
    //
    // // Finalize the MPI environment.
    // MPI_Finalize();
}
