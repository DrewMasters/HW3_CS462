all: matrix_multiply matrix_writer matrix_test_reader matrix_test_multiply

matrix_multiply: matrix_multiply.c
	mpicc -o matrix_multiply matrix_multiply.c -lm

matrix_writer: matrix_writer.c
	gcc -o matrix_writer matrix_writer.c

matrix_test_reader: matrix_reader.c
	gcc -o matrix_test_reader matrix_reader.c

matrix_test_multiply: matrix_multiply_tester.c
	gcc -o matrix_test_multiply matrix_multiply_tester.c

clean:
	$(RM) matrix_writer matrix_multiply matrix_test_multiply matrix_test_reader
