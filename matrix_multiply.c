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
	if (world_rank==0)printf("n: %d\n",n);
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
	if (world_rank==0){
		f = fopen("A.dat","rb");
		fread(A, sizeof(double), n*n, f);
		fclose(f);

		f = fopen("B.dat","rb");
		fread(B, sizeof(double), n*n, f);
		fclose(f);
		int i,j;
		
		printf("A\n");
		for (i=0;i<n;i++){
			for (j=0;j<n;j++){
				printf("%f ",A[i*j]);
			}
			printf("\n");
		}
		printf("\n");
		printf("B\n");
		for (i=0;i<n;i++){
			for (j=0;j<n;j++){
				printf("%f ",B[i*j]);
			}
			printf("\n");
		}
	}

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
	/*f = fopen("C.dat", "wb");
	fwrite(C, sizeof(double), n*n, f);
	fclose(f);
	*/
	MPI_Finalize();
}
