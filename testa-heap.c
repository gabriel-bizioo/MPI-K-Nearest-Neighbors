#include <stdio.h>
#include "heap.c"
//
// (c) by W.Zola set/25
//
// EXAMPLE Sequential MAX-Heap, 
//                    decreaseMax operation 
//                    and others

// para compilar:
// gcc -O3 max-heap.c -o max-heap -lm

#define SHOW_DECREASE_MAX_STEPS 1

#define MAX_HEAP_SIZE (1024)

int main() {
    int keys[ MAX_HEAP_SIZE ] = {0, 1, 2, 3, 4, 5};
    float values[ MAX_HEAP_SIZE ] = {60, 50, 40, 30, 20, 10};//{70, 40, 50, 30, 20, 10};
    int heapSize = 6;

    printf(">>>Initial Max-Heap (keys) ---------:\n");
    for (int i = 0; i < heapSize; ++i)
        fprintf(stderr, "%d ", keys[i]);
    printf("\n>>>Initial Max-Heap (values) ---------:\n");
    for (int i = 0; i < heapSize; ++i)
        fprintf(stderr, "%f ", values[i]);

    printf("\n");

    printf("------Max-Value-Heap Tree------\n");
    drawValueHeapTree( values, heapSize, 3 );

    printf("------Max-Key-Heap Tree------\n");
    drawKeyHeapTree( keys, heapSize, 3 );

    decreaseMax(keys, values, heapSize, 5, 0); // Decreasing the maximum value to 5

    printf("\nMax-Heap after decrease operation:\n");
    for (int i = 0; i < heapSize; ++i)
        printf("%f ", values[i]);
    printf("\n");

    printf("------Max-Value-Heap Tree------\n");
    drawValueHeapTree( values, heapSize, 3 );

    printf("------Max-Key-Heap Tree------\n");
    drawKeyHeapTree( keys, heapSize, 3 );

    printf("=========================\n");

    //bijecao valor <==> chave
    float data[] =    {40,  10,  30,  70,  51,  20,  4,   5,   44,  41,  55,  50};
    int data_keys[] = {200, 111, 233, 321, 445, 890, 666, 779, 888, 960, 690, 420};
    int n = sizeof(data) / sizeof(data[0]);

    // empty heap
    heapSize = 0;
    printf("EMPTY heap!\n");

    printf("will insert: " );
    for( int i=0; i<n; i++ ) {
      printf("%f (%d), ", data[i], data_keys[i]);
    }
    printf( "\n" );

    for( int i=0; i<n; i++ ) {
      fprintf(stderr, "\ndata_keys[11] = %d\n\n", data_keys[11]);
      printf("inserting %f (data[%d], key %d)\n", data[i], i, data_keys[i]);
      insert( keys, values, &heapSize, data[i], data_keys[i]);

      printf("------Max-Value-Heap Tree------ ");
      if( isMaxValueHeap( values, heapSize ) )
         printf( "is a heap!\n" );
      else
         printf( "is NOT a heap!\n" );

      printf("------Max-Key-Heap Tree------ ");
      if( isMaxKeyHeap( keys, heapSize ) )
         printf( "is a heap!\n" );
      else
         printf( "is NOT a heap!\n" );
      #ifndef SHOW_DECREASE_MAX_STEPS 
        drawHeapTree( heap, heapSize, 4 );
      #endif

    }
    #ifdef SHOW_DECREASE_MAX_STEPS 
       drawValueHeapTree( values, heapSize, 4 );
       drawKeyHeapTree( keys, heapSize, 4);
    #endif   

    printf("=========================\n");
    printf("=====decreaseMAX tests===\n");
    int data2[] = {4, 10, 30, 70, 55, 20, 4, 5, 25};
    n = sizeof(data2) / sizeof(data2[0]);


    printf("will decreaseMAX to the following values: " );
    for( int i=0; i<n; i++ ) {
      printf("%d ", data2[i]);
    }
    printf( "\n" );

    for( int i=0; i<n; i++ ) {
      printf("decreaseMAX to %d\n", data2[i]);
      int new_value = data2[i];
      decreaseMax( keys, values, heapSize, new_value, data[i]);

      printf("------Max-Value-Heap Tree (after decrease)------ ");
      if( isMaxValueHeap( values, heapSize ) )
         printf( "is a max heap!\n" );
      else
         printf( "is NOT a max heap!\n" );

      printf("------Max-Key-Heap Tree (after decrease)------ ");
      if( isMaxKeyHeap( keys, heapSize ) )
         printf( "is a max heap!\n" );
      else
         printf( "is NOT a max heap!\n" );
      #ifndef SHOW_DECREASE_MAX_STEPS 
       drawHeapTree( heap, heapSize, 4 );
      #endif

    }

    return 0;
}
