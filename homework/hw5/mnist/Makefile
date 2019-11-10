# some useful variables
FLAGS=-Wall -Werror -pedantic -std=c11
TEST=test_mnist

# all depends on outputfile
all: $(TEST) mnist2pgm

# build unit test
test: test_mnist.o mnist.o
	gcc $(FLAGS) -o test_mnist test_mnist.o mnist.o -lcunit
	./test_mnist

# build varstring example from varstring_example.c
mnist2pgm: mnist2pgm.o mnist.o
	gcc $(FLAGS) -o mnist2pgm mnist2pgm.o mnist.o

# remove files made from make
clean: 
	rm -rf test_mnist mnist2pgm test_mnist.o mnist.o mnist2pgm.o

# executable depends on objfile
$(TEST): $(TEST).o mnist.o
	gcc $(FLAGS) -o $(TEST) $(TEST).o mnist.o -lcunit

$(TEST).o: $(TEST).c mnist.o
	gcc $(FLAGS) -c $(TEST).c

mnist2pgm.o: mnist2pgm.c mnist.o
	gcc $(FLAGS) -c mnist2pgm.c 

mnist.o: mnist.c mnist.h
	gcc $(FLAGS) -c mnist.c
