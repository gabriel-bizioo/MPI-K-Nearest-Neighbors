knn-mpi:
	mpicc -O3 -march=native -ffast-math -funroll-loops -fopenmp main.c -lm -o knn-mpi

clean:
	rm knn-mpi

testa-heap:
	gcc testa-heap.c -lm -o testa-heap
