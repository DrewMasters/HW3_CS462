#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
	int i,j,k,n;
	double **A,**B,**C,sum,*tmpA,*tmpB;
	FILE *f;
	
	n=atoi(argv[1]);
	tmpA=(double *)malloc(sizeof(double)*n*n);
	tmpB=(double *)malloc(sizeof(double)*n*n);

	A=(double **)malloc(sizeof(double *)*n);
	B=(double **)malloc(sizeof(double *)*n);
	C=(double **)malloc(sizeof(double *)*n);
	
	for(i=0;i<n;i++){
		A[i]=(double *)malloc(sizeof(double)*n);
		B[i]=(double *)malloc(sizeof(double)*n);
		C[i]=(double *)malloc(sizeof(double)*n);
	}

	f = fopen("A.dat","rb");
	fread(tmpA,sizeof(double),n*n,f);
	fclose(f);

	f = fopen("B.dat","rb");
	fread(tmpB,sizeof(double),n*n,f);
	fclose(f);
	
	for (i=0; i<n; i++){
		for (j=0; j<n; j++){
			A[i][j]=tmpA[i*j];
			B[i][j]=tmpB[i*j];
		}
	}

	sum=0;
	for (i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				sum = sum + A[i][k]*B[k][j];
			}
			C[i][j]=sum;
			sum=0;
		}
	}

	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			printf("%f ",C[i][j]);
		}
		printf("\n");
	}
}
