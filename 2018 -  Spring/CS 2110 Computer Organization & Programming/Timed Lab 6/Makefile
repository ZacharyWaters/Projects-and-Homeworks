CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Werror -Wextra
DEBUG_FLAGS = -g -DDEBUG

run-test : build-debug
	./test

run-gdb : build-debug
	gdb ./test

run-valgrind : build-debug
	valgrind --leak-check=yes --show-reachable=yes --tool=memcheck ./test

build-debug : CFLAGS += $(DEBUG_FLAGS)
build-debug : verify list_array.o test.o list_array.h
	$(CC) $(CFLAGS) test.o list_array.o -o test

list_array.o : list_array.c list_array.h
	$(CC) $(CFLAGS) -c list_array.c

test.o : test.c list_array.h
	$(CC) $(CFLAGS) -c test.c

clean :
	rm -f *.o test

submit : verify
	tar -czf tl6.tar.gz list_array.c

verify : verify.sh
	@/bin/bash verify.sh