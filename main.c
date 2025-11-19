// TRABALHO 3: ci1316 2o semestre 2025
// Aluno1: Gabriel Pucci Bizio        GRR: GRR20211751
// Aluno2:                            GRR:

#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chrono.c"
#include "heap.c"
#include "verificaKNN.c"

chronometer_t tempoKNN;

void geraConjuntoDeDados(float *C, int nc, int d) {
  int total = nc * d;
  for (int i = 0; i < total; i++) {
    C[i] = (float)((double)rand() / (double)(RAND_MAX / 1000));
  }
}

static inline float distanciaEuclidiana(const float *restrict a,
                                        const float *restrict b, int D) {
  float dist = 0.0f;

#pragma omp simd reduction(+ : dist)
  for (int l = 0; l < D; ++l)
    dist += (a[l] - b[l]) * (a[l] - b[l]);

  return dist;
}

void computaKNN(float *Q, int nq, float *P, int np, int D, int k, int tid,
                float **distancias, int **chaves) {

  float distancia, mul = 0.0;
  int heapsize, chave, heapindex;

  for (int i = (tid * nq) * D; i < ((tid + 1) * nq) * D; i += D) {
    heapsize = 0;
    for (int j = tid; j < np * D; j += D) {

      distancia = distanciaEuclidiana(&P[j], &Q[i], D);

      /* 'chave' indica qual dos np pontos de P estamos inserindo na heap.
      P eh uma matriz armazenada como um vetor,
      logo o indice real do ponto eh igual a 'chave * D' */
      chave = j == 0 ? 0 : j / D;
      heapindex = i == 0 ? 0 : i / D;
      if (heapsize < k)
        insert(chaves[heapindex], distancias[heapindex], &heapsize,
                distancia, chave);
      else
        decreaseMax(chaves[heapindex], distancias[heapindex], heapsize, 
                distancia, chave);
    }
  }
}

int main(int argc, char **argv) {

  // qtd de pontos em q e p
  int nq, np;
  // qtd de dimensoes em cada ponto, qtd de vizinhos
  int d, k;
  int t;
  // Chamar ou nao 'verificaKNN'
  int v = 0;

  char Args[256];
  nq = np = d = k = 0;
  for (int i = 1; i < argc; i++) {
    strncpy(Args, argv[i], 256);
    if (!strncmp(Args, "-nq=", 4))
      nq = atoi(Args + 4);
    else if (!strncmp(Args, "-npp=", 5))
      np = atoi(Args + 5);
    else if (!strncmp(Args, "-d=", 3))
      d = atoi(Args + 3);
    else if (!strncmp(Args, "-k=", 3))
      k = atoi(Args + 3);
    else if (!strncmp(Args, "-v", 2))
      v = 1;
    else if (!strncmp(Args, "-t=", 3))
      t = atoi(Args + 3);
    else {
      fprintf(stderr, "ERRO: argumentos invalidos\n");
      exit(1);
    }

    memset(Args, 0, 256);
  }

  if (!np || !nq || !k || !d) {
    fprintf(stderr, "ERRO: argumentos nao especificados ou iguais a 0\n");
    exit(1);
  }

  float *Q;
  float *P;
  float *QProcesso;
  float *Plocal;

  int *R;    // Chaves dos pontos mais proximos ponto[i] = P[R[i] * d]
  float *dR; // Distancias dos pontos mais proximos
  int **RPprocesso;
  float **dRprocesso;

  int worldSize;
  int worldRank;
  int tamanhoFaixaProcesso;

  MPI_Init(&argc, &argv);

  /* ======= INICIALIZACAO DAS VARIAVEIS GLOBAIS ======= */
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
  if (nq % worldSize) {
    fprintf(stderr, "ERRO: tamanho de Q nao divisivel pelo numero de processos "
                    "nao foi implementado\n");
    exit(1);
  }
  tamanhoFaixaProcesso = nq / worldSize;

  P = malloc(sizeof(float) * np * d);
  if (worldRank == 0) {
    Q = malloc(sizeof(float) * nq * d);

    geraConjuntoDeDados(Q, nq, d);
    geraConjuntoDeDados(P, np, d);

    // nq linhas e k colunas
    R = malloc(sizeof(int) * nq * k);
    dR = malloc(sizeof(float) * nq * k);
  }

  /* ======= ALOCACAO DAS VARIAVEIS LOCAIS =======
  Necessario que esteja em forma de matriz para que seja aceita
  pela funçao e pela biblioteca de heap */
  // tfaixa linhas
  RPprocesso = malloc(sizeof(int *) * tamanhoFaixaProcesso);
  dRprocesso = malloc(sizeof(float *) * tamanhoFaixaProcesso);
  //
  // k colunas
  for (int i = 0; i < tamanhoFaixaProcesso; ++i) {
      RPprocesso[i] = malloc(sizeof(int) * k);
      dRprocesso[i] = malloc(sizeof(float) * k);
  }
  QProcesso = malloc(sizeof(float) * tamanhoFaixaProcesso * d);

  MPI_Bcast(P, np * d, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Scatter(Q, tamanhoFaixaProcesso * d, MPI_FLOAT, QProcesso, tamanhoFaixaProcesso * d,
          MPI_FLOAT, 0, MPI_COMM_WORLD);
  #pragma omp parallel num_threads(t)
  {
      int tamanhoFaixaThread;
      tamanhoFaixaThread = tamanhoFaixaProcesso / t;
      if(tamanhoFaixaProcesso % t != 0 && omp_get_thread_num() == t - 1)
          tamanhoFaixaThread += tamanhoFaixaProcesso % t;


      if (worldRank == 0 && omp_get_thread_num() == 0) {
          chrono_reset(&tempoKNN);
          chrono_start(&tempoKNN);
      }

      MPI_Barrier(MPI_COMM_WORLD);
      /* ======= PASSAGEM DE DADOS E COMPUTACAO ======= */

      int tid = omp_get_thread_num();
      computaKNN(QProcesso, tamanhoFaixaThread, P, np, d, k, tid, dRprocesso, RPprocesso);
  }

  /* ======= SERIALIZACAO ======= */
  // Vetoriza os dados em cada nodo (matriz -> vetor), para que seja aceito pelo
  // MPI_Gather
  int *bufR = malloc(tamanhoFaixaProcesso * k * sizeof(int));
  float *bufdR = malloc(tamanhoFaixaProcesso * k * sizeof(float));
  for (int i = 0; i < tamanhoFaixaProcesso; i++) {
    memcpy(&bufR[i * k], RPprocesso[i], k * sizeof(int));
    memcpy(&bufdR[i * k], dRprocesso[i], k * sizeof(float));
  }
  MPI_Gather(bufR, tamanhoFaixaProcesso * k, MPI_INT, R, tamanhoFaixaProcesso * k,
          MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gather(bufdR, tamanhoFaixaProcesso * k, MPI_FLOAT, dR, tamanhoFaixaProcesso * k,
          MPI_FLOAT, 0, MPI_COMM_WORLD);
  if (worldRank == 0)
    chrono_stop(&tempoKNN);

  /* ======= REPORTAGEM DE TEMPO ======= */
  if (worldRank == 0) {

    double tempoKNNSegundos =
        (double)chrono_gettotal(&tempoKNN) / ((double)1000 * 1000 * 1000);
    printf("TEMPO_COMPUTA_KNN %lf\n", tempoKNNSegundos);

    if (v)
      verificaKNN(Q, nq, P, np, d, k, R);
  }

  /* ====== DESALOCACAO ====== */
  free(bufR);
  free(bufdR);
  for (int i = 0; i < tamanhoFaixaProcesso; ++i) {
    free(RPprocesso[i]);
    free(dRprocesso[i]);
  }
  free(RPprocesso);
  free(dRprocesso);

  free(P);
  free(QProcesso);

  MPI_Finalize();
}
