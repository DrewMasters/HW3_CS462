#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
	int i,j,k,n;
	double *A,*B,*C,sum;
	FILE *f;
	
	n=atoi(argv[1]);
	A=(double *)malloc(sizeof(double)*n*n);
	B=(double *)malloc(sizeof(double)*n*n);
	C=(double *)malloc(sizeof(double)*n*n);

	f = fopen("A.dat","rb");
	fread(A,sizeof(double),n*n,f);
	fclose(f);

	f = fopen("B.dat","rb");
	fread(B,sizeof(double),n*n,f);
	fclose(f);
	
	sum=0;
	for (i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				sum = sum + A[i*k]*B[k*j];
			}
			C[i*j]=sum;
			sum=0;
		}
	}

	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			printf("%f ",C[i*j]);
		}
		printf("\n");
	}
}
