# some useful variables
FLAGS=-Wall -Werror -pedantic -std=c11
TEST=varstring_test

# all depends on outputfile
all: $(TEST)

# build unit test
check: varstring_unit.o varstring.o
	gcc $(FLAGS) -lcunit -o varstring_unit varstring_unit.o varstring.o

# run valgrind
valgrind: $(TEST).o varstring.o
	gcc $(FLAGS) -o $(TEST) $(TEST).o varstring.o
	valgrind ./$(TEST)

# build varstring example from varstring_example.c
varstring_example: varstring_example.o varstring.o
	gcc $(FLAGS) -o varstring_example varstring_example.o varstring.o

# remove files made from make
clean: 
	rm -rf varstring_example varstring_test varstring_example.o varstring_test.o varstring.o

# executable depends on objfile
$(TEST): $(TEST).o varstring.o
	gcc $(FLAGS) -o $(TEST) $(TEST).o varstring.o

varstring_example.o: varstring_example.c varstring.h
	gcc $(FLAGS) -c varstring_example.c

varstring_unit.o: varstring_unit.c varstring.h
	gcc $(FLAGS) -c varstring_unit.c

$(TEST).o: $(TEST).c varstring.h
	gcc $(FLAGS) -c $(TEST).c

varstring.o: varstring.c varstring.h
	gcc $(FLAGS) -c varstring.c
