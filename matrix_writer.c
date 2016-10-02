#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char **argv){
	double *A;
	FILE *fr,*fb;
	int i,j,n;
	n = atoi(argv[1]);
	if (argc != 4){
		printf("./matrix_maker size byte_file_name human_file_name\n");
		exit(1);
	}
	A=(double *)malloc(sizeof(double)*n*n);
	srand(time(NULL));
	for (i=0;i<n*n;i++){
		A[i]=((double)rand()/(double)RAND_MAX)*100;
	}
	
	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			printf("%f ",A[i*j]);
		}
		printf("\n");
	}	

	fb = fopen(argv[2],"wb");
	fwrite(A,sizeof(double),n*n,fb);
	fclose(fb);
	fr = fopen(argv[3],"w");
	for (i=0; i<n;i++){
		for (j=0; j<n;j++){
			fprintf(fr, "%f ", A[i*j]);
		}
		fprintf(fr,"\n");
	}
	fclose(fr);
}
