#include <stdio.h>
#define DEBUG

void verificaKNN(float *Q, int nq, float *P, int np, int D, int k, int *R) {
    printf(" ---------- VERIFICA KNN ---------- \n");

#ifdef DEBUG
    printf("-------- PONTOS DE Q --------\n");
    for(int linha = 0; linha < nq; linha++) {
        printf("Q[%d] coords:", linha);
        for(int coluna = 0; coluna < D; coluna++)
            printf(" %f", Q[linha * D + coluna]);
        printf("\n");
    }

    printf("-------- PONTOS DE P --------\n");
    for(int linha = 0; linha < np; linha++) {
        printf("P[%d] coords:", linha);
        for(int coluna = 0; coluna < D; coluna++)
            printf("%f", P[linha * D + coluna]);
        printf("\n");
    }

    printf("-------- KNN ------\n");
#endif

    for(int linha = 0; linha < nq; linha++) {
        printf("knn[%d]: ", linha);
        for( int coluna = 0; coluna < k; coluna++)
            printf("%d ", R[linha * k + coluna]);
        printf("\n");
    }
}
