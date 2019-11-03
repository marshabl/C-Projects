# some useful variables
FLAGS=-Wall -Werror -pedantic -std=c11
FILENAME=mymemory_example

# all depends on outputfile
all: mymemory_example

# executable depends on objfile
mymemory_example: mymemory_example.o mymemory.o
	gcc $(FLAGS) -o mymemory_example mymemory_example.o mymemory.o

mymemory_example.o: mymemory_example.c mymemory.h
	gcc $(FLAGS) -c mymemory_example.c

mymemory.o: mymemory.c mymemory.h
	gcc $(FLAGS) -c mymemory.c
