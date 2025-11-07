#include <stdio.h>
#include <math.h>
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

void drawHeapTreeV0( int heap[], int size, int nLevels )   // FIX ME!
{
     int offset = 0;
     int space = (int) pow( 2, nLevels-1 );
     //int space = 0;

     int nElements = 1;
     for( int level=0; level<nLevels; level++ ) {

        // print all elements in this level     
        for( int i=offset; i<size && i<(offset+nElements); i++ ) {

           printf( "[%3d]", heap[i] );

        }   
        printf( "\n" );

        offset += nElements;
        space = nElements-1;
        nElements *= 2;
     }
}

void printNSpaces( int n )
{
    for( int i=0; i<n; i++ )
       printf( " " );
}

void drawValueHeapTree( float values[], int size, int nLevels )   // FIX ME!
{
     int offset = 0;
     int space = (int) pow( 2, nLevels-1 );
     //int space = 0;
     //printf( "drawHeapTree %d\n", nLevels );
     int nElements = 1;
     for( int level=0; level<nLevels; level++ ) {

        // print all elements in this level     
        for( int i=offset; i<size && i<(offset+nElements); i++ ) {
           printNSpaces( ((pow(2,nLevels-1-level))*2)-2 );
           printf( "[%.2f]", values[i] );
           printNSpaces( ((pow(2,nLevels-1-level))*2)-2 );
        }
        printf( "\n" );

        offset += nElements;
        space = nElements-1;
        nElements *= 2;
     }
}

void drawKeyHeapTree( int keys[], int size, int nLevels )   // FIX ME!
{
     int offset = 0;
     int space = (int) pow( 2, nLevels-1 );
     //int space = 0;
     //printf( "drawHeapTree %d\n", nLevels );
     int nElements = 1;
     for( int level=0; level<nLevels; level++ ) {

        // print all elements in this level     
        for( int i=offset; i<size && i<(offset+nElements); i++ ) {
           printNSpaces( ((pow(2,nLevels-1-level))*2)-2 );
           printf( "[%2d]", keys[i] );
           printNSpaces( ((pow(2,nLevels-1-level))*2)-2 );
        }
        printf( "\n" );

        offset += nElements;
        space = nElements-1;
        nElements *= 2;
     }
}

//#define parent(pos) ( pos/2 ) // SE nao usar posicao 0
#define parent(pos) ( (pos-1)/2 )

void swap(int *ka, int *kb, float *va, float *vb) {
  int itemp = *ka;
  *ka = *kb;
  *kb = itemp;

  float ftemp = *va;
  *va = *vb;
  *vb = ftemp;
}

void maxHeapify(int keys[], float values[], int size, int i) {
  while (1) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && values[left] > values[largest])
      largest = left;

    if (right < size && values[right] > values[largest])
      largest = right;

    if (largest != i) {
      swap(&keys[i], &keys[largest], &values[i], &values[largest]); // Using the swap function
      i = largest;

    }
    else
        break;
  }
}

void heapifyUp(int keys[], float values[], int *size, int pos) {
  float val = values[pos];
  int key = keys[pos];
  while (pos > 0 && val > values[parent(pos)]) {
    values[pos] = values[parent(pos)];
    keys[pos] = keys[parent(pos)];
    pos = parent(pos);
  }
  values[pos] = val;
  keys[pos] = key;
}


void insert(int keys[], float values[], int *size, float element, int key) {
  int last = *size;
  *size += 1;

  values[last] = element;
  keys[last] = key;
  heapifyUp(keys, values, size, last);
}

int isMaxKeyHeap( int keys[], int size ) {
    for( int i=1; i<size; i++ )
        if( keys[i] <= keys[parent(i)] )
           continue;
        else {
           printf( "\nbroke at [%d]=%d\n", i, keys[i] );
           printf( "father at [%d]=%d\n",
                             parent(i), keys[parent(i)] );
           return 0;
        }
    return 1;
}

int isMaxValueHeap( float values[], int size ) {
    for( int i=1; i<size; i++ )
        if( values[i] <= values[parent(i)] )
           continue;
        else {
           printf( "\nbroke at [%d]=%f\n", i, values[i] );
           printf( "father at [%d]=%f\n",
                             parent(i), values[parent(i)] );
           return 0;
        }
    return 1;
}

void decreaseMax(int keys[], float values[], int size, float new_value, int new_key) {
  if (size == 0) // Heap is empty
    return;

  if (values[0] > new_value) {
    values[0] = new_value;
    keys[0] = new_key;
#if SHOW_DECREASE_MAX_STEPS
    int nLevels = (int)log2(size) + 1;
#endif
    maxHeapify(keys, values, size, 0);
  }
}

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
