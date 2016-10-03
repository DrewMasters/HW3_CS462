#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv){
	int i,j,row,col,t,world_size, world_rank,n;
	FILE *f;
	double *A, *B, *C,*dot,*tc,tmp,va,vb;
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
	row = world_rank % n;
	col = world_rank / n;
	/*
	 * Create matrix objects
	 */
	//printf("%d about to initialize matrices\n",world_rank);
	A = (double *)malloc(sizeof(double)*n*n);
	B = (double *)malloc(sizeof(double)*n*n);
	C = (double *)malloc(sizeof(double)*n*n);
	dot = (double *)malloc(sizeof(double)*n);
	tc = (double *)malloc(sizeof(double)*n);
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
		va = A[0];
		vb = B[0];
		for (i=0; i < n; i++) tc[i]=B[i];
		for (i=1;i<n*n;i++){
			/*for (j=0; j<n*n; j++)
			{
				MPI_Send(&A[j],1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
				MPI_Send(&B[j],1,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
			}
			*/
			/*
			 * send each process it's matrix element
			 */
			MPI_Send(&A[i],1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			MPI_Send(&B[i],1,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
			printf("col: %d sending to %d\n",i/n,i);
			MPI_Send(&A[n*(i/n)],n,MPI_DOUBLE,i,2,MPI_COMM_WORLD);
		}
	}
	else{
		printf("%d about to start receive matrix\n",world_rank);
		/*for (j=0; j<n*n; j++){
			MPI_Recv(&A[j],1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(&B[j],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}
		*/
		/*
		 * receive element from root process
		 */
		MPI_Recv(&va,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(&vb,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(&tc[0],n,MPI_DOUBLE,0,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}

	printf("%d va: %f row: %d col: %d\n",world_rank,va,row,col);
	printf("%d vb: %f row: %d col: %d\n",world_rank,vb,row,col);
	for (i=0;i<n;i++) printf("%d tc[%d]: %f\n", world_rank, i, tc[i]);
	/*
	 * take the first row of A and multiply it
	 * by the value received for B
	 * all elements in first column of A is 
	 * multiplied by value in B
	 */
	for (i=0;i<n;i++) dot[i]=vb*tc[i];

	/*
	 * Distribute matrix across all processes
	 */
	//MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	/*printf("%d about to print matrix\n",world_rank);
	for (i=0;i<n*n;i++){
		printf("%d A i:%d %f\n",world_rank,i,A[i]);
		printf("%d B i:%d %f\n",world_rank,i,B[i]);
	}*/
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
