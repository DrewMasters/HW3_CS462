#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv){
	int i,j,world_size, world_rank,n;
	FILE *f;
	double *A, *B, *C,tmp;
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
	if (world_rank==0)printf("n: %d world size: %d\n",n,world_size);
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
	
		for (i=1;i<n*n;i++){
			for (j=0; j<n*n; j++)
			{
				MPI_Send(&A[j],1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
				MPI_Send(&B[j],1,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
			}
		}
	}
	else{
		for (j=0; j<n*n; j++){
			MPI_Recv(&A[j],1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(&B[j],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}
	}
	/*
	 * Distribute matrix across all processes
	 */
	//MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	printf("%d about to print matrix\n",world_rank);
	for (i=0;i<n*n;i++){
		printf("%d A i:%d %f\n",world_rank,i,A[i]);
		printf("%d B i:%d %f\n",world_rank,i,B[i]);
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
