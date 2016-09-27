#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv){
	int world_size, world_rank,n;
	FILE *f;
	double *A, *B, *C;
	/*
	 * Initialize MPI
	 */
	MPI_Init(NULL,NULL);

	/*
	 * Get world size and rank
	 */
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	n = (int) sqrt(world_size);
	/* 
	 * Check to see if matrix/grid
	 * is a square
	 */
	if (sqrt(world_size)-n!=0){
		MPI_Finalize();
		exit(1);
	}
	/*
	 * Create matrix objects
	 */
	A = (double *)malloc(sizeof(double)*n*n);
	B = (double *)malloc(sizeof(double)*n*n);
	C = (double *)malloc(sizeof(double)*n*n);
	
	/*
	 * Read in matrix information from
	 * files A.dat and B.dat
	 */
	f = fopen("A.dat","rb");
	fread(A, sizeof(double), n*n, f);
	fclose(f):

	f = fopen("B.dat","rb");
	fread(B, sizeof(double), n*n, f);
	fclose(f);

	/*
	 * Distribute matrix across all processes
	 */
	/*
	 * Multiply matrix
	 */
	/*
	 * Collect matrix elements from all processes and combine
	 * it in to result matrix C
	 */

	/*
	 * Write resulting matrix to C.dat
	 */
	f = fopen("C.dat", "wb");
	fwrite(C, sizeof(double), n*n, f);
	fclose(f);
}
