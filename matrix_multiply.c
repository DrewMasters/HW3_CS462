#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv){
	int world_size, world_rank,n;
	FILE *f;
	double *A, *B, *C;
	MPI_Status status;
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
	//printf("%d has started\n",world_rank);
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
	//printf("%d about to initialize matrices\n",world_rank);
	A = (double *)malloc(sizeof(double)*n*n);
	B = (double *)malloc(sizeof(double)*n*n);
	C = (double *)malloc(sizeof(double)*n*n);
	//printf("%d matrices are initialized\n",world_rank);
	
	/*
	 * Read in matrix information from
	 * files A.dat and B.dat
	 */
	if (world_rank==0){
		printf("%d about to read in matrices\n",world_rank);
		f = fopen("A.dat","rb");
		fread(A, sizeof(double), n*n, f);
		fclose(f);

		f = fopen("B.dat","rb");
		fread(B, sizeof(double), n*n, f);
		fclose(f);
		
		printf("%d finished reading in matrices\n",world_rank);
	}
	/*printf("world_rank %d/%d\n",world_rank,world_size);
	MPI_Bcast(&A,sizeof(double)*n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	//printf("%d received matrix A\n",world_rank);
	printf("A world_rank %d/%d\n",world_rank,world_size);
	MPI_Bcast(&B,sizeof(double)*n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	//printf("%d has received both matrices\n",world_rank);
	printf("AB world_rank %d/%d\n",world_rank,world_size);
	*/
	/*
	 * Distribute matrix across all processes
	 */
	int i,j;
	for (i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%d A i:%d j:%d %f\n",world_rank,i,j,A[i*j]);
			printf("%d B i:%d j:%d %f\n",world_rank,i,j,B[i*j]);
		}
	}
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
	/*if (world_rank==0){
		f = fopen("C.dat", "wb");
		fwrite(C, sizeof(double), n*n, f);
		fclose(f);
	}*/

	MPI_Finalize();
}
