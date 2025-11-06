knn-mpi:
	mpicc main.c -o knn-mpi

clean:
	rm knn-mpi

testa-heap:
	gcc testa-heap.c -lm -o testa-heap
