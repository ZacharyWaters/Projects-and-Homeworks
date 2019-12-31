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
build-debug : verify circ_list.o array_list.o test.o circ_list.h array_list.h list.h
	$(CC) $(CFLAGS) test.o circ_list.o array_list.o -o test

circ_list.o : circ_list.c circ_list.h list.h
	$(CC) $(CFLAGS) -c circ_list.c

array_list.o : array_list.c array_list.h list.h
	$(CC) $(CFLAGS) -c array_list.c

test.o : test.c circ_list.h array_list.h list.h
	$(CC) $(CFLAGS) -c test.c

clean :
	rm -f *.o test

submit : verify
	tar -czf list_submission.tar.gz circ_list.c array_list.c

verify : verify.sh
	@echo @/bin/bash verify.sh

tarball:
	tar -czf hw10.tar.gz array_list.c array_list.h circ_list.c circ_list.h list.h test.c verify.sh Makefile

