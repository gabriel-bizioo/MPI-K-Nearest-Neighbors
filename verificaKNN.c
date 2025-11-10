#include <stdio.h>

void verificaKNN(float *Q, int nq, float *P, int np, int D, int k, int *R) {
    printf(" ---------- VERIFICA KNN ---------- \n");

    for(int linha = 0; linha < nq; linha++) {
        printf("knn[%d]: ", linha);
        for( int coluna = 0; coluna < k; coluna++)
            printf("%d ", R[linha * k + coluna]);
        printf("\n");
    }
}
