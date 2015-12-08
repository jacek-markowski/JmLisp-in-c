CC = gcc
SHARED_FILES = src/lisp.c src/mpc/mpc.c
EXE_FILES = src/main.c $(SHARED_FILES)
TEST_FILES = tests/lisp_test.c tests/seatest/seatest.c $(SHARED_FILES)
OUT_EXE = build/lisp
OUT_TEST = build/lisp_test
LINK = -lm -ledit
OPTIONS = -Wall -g -D_GNU_SOURCE
build: $(FILES)
	mkdir build || echo 1
	$(CC) $(OPTIONS) -o $(OUT_EXE) $(EXE_FILES) $(LINK)

clean:
	rm -r build

test: $(TEST_FILES)
	mkdir build || echo 1
	$(CC) $(OPTIONS) -o $(OUT_TEST) $(TEST_FILES) $(LINK)
	./build/lisp_test
all: build test
rebuild: clean build
