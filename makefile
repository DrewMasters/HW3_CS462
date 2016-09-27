
matrix_multiply: matrix_multiply.c
	mpicc -o matrix_multiply matrix_multiply.c -lm

clean:
	$(RM) matrix_multiply
