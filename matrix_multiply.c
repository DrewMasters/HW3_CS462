#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv){
	int i, j, root, row, col, t, r_rank, r_size,
		c_rank, c_size, world_size, world_rank, n, k;
	FILE *f;
	double *A, *B, *C,*tr, *tc, va, vb, dot;
	MPI_Comm r, c;
	
	/*
	 * root process
	 */
	root = 0;
	
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
	 * find mapping for process
	 */
	row = world_rank % n;
	col = world_rank / n;
	
	/*
	 * create row and column communicators
	 * r = row communicator
	 * c = column communicator
	 */
	MPI_Comm_split(MPI_COMM_WORLD, row, world_rank, &r);
	MPI_Comm_split(MPI_COMM_WORLD, col, world_rank, &c);
	MPI_Comm_size(r, &r_size);
	MPI_Comm_size(c, &c_size);
	MPI_Comm_rank(r, &r_rank);
	MPI_Comm_rank(c, &c_rank);

	/*
	 * Create matrix objects
	 */
	A = (double *)malloc(sizeof(double)*n*n);
	B = (double *)malloc(sizeof(double)*n*n);
	C = (double *)malloc(sizeof(double)*n*n);
	tr = (double *)malloc(sizeof(double)*n);
	tc = (double *)malloc(sizeof(double)*n);

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

		/*
		 * distribute elements to processes
		 */
		k=0;
		for (i=0; i < n; i++){
			for (j=i; j < world_size; j+=n){
				if (j==0){
					va = A[j];
					vb = B[j];
				}
				else{
					MPI_Send(&A[k], 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
					MPI_Send(&B[k], 1, MPI_DOUBLE, j, 1, MPI_COMM_WORLD);
				}
				k++;
			}
		}
	}
	else{
		/*
		 * receive elements from root
		 */
		MPI_Recv(&va, 1, MPI_DOUBLE, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&vb, 1, MPI_DOUBLE, root, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	//printf("row: %d col: %d world_rank: %d va: %f vb: %f\n",
	//		row, col, world_rank, va, vb);

	/*
	 * construct rows and columns for dot product
	 */
	MPI_Comm_rank(r, &r_rank);
	MPI_Comm_rank(c, &c_rank);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Allgather(&va, 1, MPI_DOUBLE, tr, 1, MPI_DOUBLE, r);

	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_rank(r, &r_rank);
	MPI_Comm_rank(c, &c_rank);
	MPI_Allgather(&vb, 1, MPI_DOUBLE, tc, 1, MPI_DOUBLE, c);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_rank(r, &r_rank);
	MPI_Comm_rank(c, &c_rank);

	/*printf("%d about to print row array\n",world_rank);
	for (i=0; i < n; i++){
		printf("%d tr[%d]: %f\n",world_rank, i, tr[i]);
	}
	
	MPI_Comm_rank(r, &r_rank);
	MPI_Comm_rank(c, &c_rank);

	
	printf("%d about to print column array\n",world_rank);
	for (i=0; i < n; i++){
		printf("%d tc[%d]: %f\n",world_rank, i, tc[i]);
	}
	*/
	/*
	 * calculate dot product
	 */
	dot = 0;
	for (i=0; i<n; i++){
		dot = dot + tr[i]*tc[i];
	}

	if (world_rank==0){
		/*
		 * receive elements from other processes
		 * and place elements in correct position
		 * of matrix/array C
		 */
		k=0;
		for (i=0; i < n; i++){
			for (j=i; j < world_size; j+=n){
				if (j==0){
					C[k]=dot;
				}
				else{
					MPI_Recv(&C[k], 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				}
				k++;
			}
		}

		/*
		 * write C to C.dat
		 */
		for (i=0; i < world_size; i++){
			printf("%d C[%d]: %f\n",world_rank,i,C[i]);
		}

	}
	else{
		/*
		 * send result back to root
		 */
		MPI_Send(&dot, 1, MPI_DOUBLE, root, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
