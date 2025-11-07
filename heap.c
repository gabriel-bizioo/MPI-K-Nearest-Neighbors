#include <math.h>
#include <stdio.h>
// (c) by W.Zola set/25
//
// EXAMPLE Sequential MAX-Heap,
//                    decreaseMax operation
//                    and others

// para compilar:
// gcc -O3 max-heap.c -o max-heap -lm

#define SHOW_DECREASE_MAX_STEPS 1

void swap(int *ka, int *kb, float *va, float *vb)
{
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

// #define parent(pos) ( pos/2 ) // SE nao usar posicao 0
#define parent(pos) ((pos - 1) / 2)

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
