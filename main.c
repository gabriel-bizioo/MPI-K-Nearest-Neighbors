

// TRABALHO2: ci1316 2o semestre 2025
// Aluno1: Gabriel Pucci Bizio        GRR: GRR20211751
// Aluno2:                            GRR:


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "heap.c"
#include "verificaKNN.c"
#include "chrono.c"

chronometer_t tempoKNN;

void geraConjuntoDeDados(float *C, int nc, int d) {
    int total = nc * d;
    for(int i = 0; i < total; i++) {
        C[i] = (float)((double)rand()/(double)(RAND_MAX/1000));
    }
}

static inline float distanciaEuclidiana(const float * restrict a, const float * restrict b, int D) {
    float dist = 0.0f;

    #pragma omp simd reduction(+:dist)
    for (int l = 0; l < D; ++l)
        dist += (a[l] - b[l]) * (a[l] - b[l]);

    return dist;
}

void computaKNN(float *Q, int nq, float *P, int np, int D, int k, float **distancias, int **chaves) {

    float distancia, mul = 0.0;
    int heapsize, chave, heapindex;

    for(int i = 0; i < nq * D; i += D) {
        heapsize = 0;
        for(int j = 0; j < np * D; j += D) {

            distancia = distanciaEuclidiana(&P[j], &Q[i], D);

            /* 'chave' indica qual dos np pontos de P estamos inserindo na heap.
            P eh uma matriz armazenada como um vetor,
            logo o indice real do ponto eh igual a 'chave * D' */
            chave = j == 0 ? 0 : j / D;
            heapindex = i == 0 ? 0 : i / D;
            if(heapsize < k)
                insert(chaves[heapindex], distancias[heapindex], &heapsize, distancia, chave);
            else
                decreaseMax(chaves[heapindex], distancias[heapindex], heapsize, distancia, chave);
        }
    }

}

// '/home2/pp/mpi+slurm': exemplos de script slurm
int main(int argc, char** argv) {

    // qtd de pontos em q e p
    int nq, np;
    // qtd de dimensoes em cada ponto, qtd de vizinhos
    int d, k;
    // Chamar ou nao 'verificaKNN'
    int v = 0;


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
        else if(!strncmp(Args, "-v", 2))
            v = 1;
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


    float *Q;
    float *P;
    float *Qlocal;
    float *Plocal;


    int *R; // Chaves dos pontos mais proximos ponto[i] = P[R[i] * d]
    float *dR; // Distancias dos pontos mais proximos
    int **Rlocal;
    float **dRlocal;

    int worldSize;
    int worldRank;
    int tamanhoFaixa;

    MPI_Init(&argc, &argv);

    /* ======= INICIALIZACAO DAS VARIAVEIS GLOBAIS ======= */
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    if(nq % worldSize) {
        fprintf(stderr, "ERRO: tamanho de Q nao divisivel pelo numero de processos nao foi implementado\n");
        exit(1);
    }
    tamanhoFaixa = nq / worldSize;

    P = malloc(sizeof(float) * np * d);
    if(worldRank == 0) {
        Q = malloc(sizeof(float) * nq * d);

        geraConjuntoDeDados(Q, nq, d);
        geraConjuntoDeDados(P, np, d);

        // nq linhas e k colunas
        R = malloc(sizeof(int) * nq * k );
        dR = malloc(sizeof(float) * nq * k);
    }


    /* ======= ALOCACAO DAS VARIAVEIS LOCAIS =======
    Necessario que esteja em forma de matriz para que seja aceita
    pela funçao e pela biblioteca de heap */
    // tfaixa linhas
    Rlocal = malloc(sizeof(int *) * tamanhoFaixa);
    dRlocal = malloc(sizeof(float *) * tamanhoFaixa);
    //
    // k colunas
    for(int i = 0; i < tamanhoFaixa; ++i) {
        Rlocal[i] = malloc(sizeof(int) * k);
        dRlocal[i] = malloc(sizeof(float) * k);
    }
    Qlocal = malloc(sizeof(float) * tamanhoFaixa * d);

    MPI_Barrier(MPI_COMM_WORLD);
    if(worldRank == 0) {
        chrono_reset(&tempoKNN);
        chrono_start(&tempoKNN);
    }

    /* ======= PASSAGEM DE DADOS E COMPUTACAO ======= */
    MPI_Bcast(P, np * d, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(Q, tamanhoFaixa * d, MPI_FLOAT, Qlocal, tamanhoFaixa * d, MPI_FLOAT, 0, MPI_COMM_WORLD);

    computaKNN(Qlocal, tamanhoFaixa, P, np, d, k, dRlocal, Rlocal);

    /* ======= SERIALIZACAO ======= */
    // Vetoriza os dados em cada nodo (matriz -> vetor), para que seja aceito pelo MPI_Gather
    int *bufR = malloc(tamanhoFaixa * k * sizeof(int));
    float *bufdR = malloc(tamanhoFaixa * k * sizeof(float));
    for(int i = 0; i < tamanhoFaixa; i++) {
        memcpy(&bufR[i * k], Rlocal[i], k * sizeof(int));
        memcpy(&bufdR[i * k], dRlocal[i], k * sizeof(float));
    }
    MPI_Gather(bufR, tamanhoFaixa * k, MPI_INT, R, tamanhoFaixa * k, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(bufdR, tamanhoFaixa * k, MPI_FLOAT, dR, tamanhoFaixa * k, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if(worldRank == 0)
        chrono_stop(&tempoKNN);

    /* ======= REPORTAGEM DE TEMPO ======= */
    if(worldRank == 0) {

        double tempoKNNSegundos =
            (double)chrono_gettotal(&tempoKNN) /
            ((double)1000 * 1000 * 1000);
        printf("TEMPO_COMPUTA_KNN %lf\n", tempoKNNSegundos);

        if(v)
            verificaKNN(Q, nq, P, np, d, k, R);
    }

    /* ====== DESALOCACAO ====== */
    free(bufR);
    free(bufdR);
    for(int i = 0; i < tamanhoFaixa; ++i) {
        free(Rlocal[i]);
        free(dRlocal[i]);
    }
    free(Rlocal);
    free(dRlocal);

    free(P);
    free(Qlocal);

    MPI_Finalize();
}
