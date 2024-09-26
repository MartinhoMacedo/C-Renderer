CC = gcc

CFLAGS = -Wall -std=c11 -fms-extensions
LSDL = `sdl2-config --libs --cflags` -F /Library/Frameworks
TEST_DIR = ./test
build:
	$(CC) -g $(CFLAGS) ./src/*.c $(LSDL) -lm -o renderer

run:
	./renderer

test_vector: $(TEST_DIR)/test_vector.c $(TEST_DIR)/unity.c
	$(CC) $(CFLAGS) ./src/vector.c ./test/test_vector.c ./test/unity.c -o ./test/test_vector
	./test/test_vector

test_mesh: $(TEST_DIR)/test_vector.c $(TEST_DIR)/unity.c
	$(CC) $(CFLAGS) ./src/mesh.c ./src/face.c ./src/vector.c ./src/darray.c ./test/test_mesh.c ./test/unity.c -o ./test/test_mesh
	./test/test_mesh


test_darray: $(TEST_DIR)/test_darray.c $(TEST_DIR)/unity.c
	$(CC) -g $(CFLAGS) ./src/darray.c ./test/test_darray.c ./test/unity.c -o ./test/test_darray
	./test/test_darray

test_matrix: $(TEST_DIR)/test_matrix.c $(TEST_DIR)/unity.c
	$(CC) -g $(CFLAGS) ./src/matrix.c ./src/vector.c ./test/test_matrix.c ./test/unity.c -o ./test/test_matrix
	./test/test_matrix

clean:
	rm renderer
