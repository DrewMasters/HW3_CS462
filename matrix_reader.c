#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	FILE *f;
	int n, i, j;
	double *A;

	A=(double *)malloc(sizeof(double)*n*n);

	n=atoi(argv[1]);
	f = fopen(argv[2],"rb");
	
	fread(A,sizeof(double),n*n,f);
	fclose(f);

	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			printf("%f ", A[i*j]);
		}
		printf("\n");
	}
}
