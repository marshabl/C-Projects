FLAGS=-Wall -Werror -std=c11 -pedantic -g

all: mymake makefile_parser_driver

makefile_parser_driver: makefile_parser.o makefile_parser_driver.o varstring.o 
	gcc $(FLAGS) -o makefile_parser_driver makefile_parser.o makefile_parser_driver.o varstring.o

mymake: mymake_main.o mymake.o digraph.o makefile_parser.o varstring.o util.o
	gcc $(FLAGS) -o mymake mymake_main.o mymake.o digraph.o makefile_parser.o varstring.o util.o

makefile_parser.o: makefile_parser.c varstring.c makefile_parser.h makefile_parser_config.h
	gcc $(FLAGS) -o makefile_parser.o -c makefile_parser.c

makefile_parser_driver.o: makefile_parser_driver.c makefile_parser.c
	gcc $(FLAGS) -o makefile_parser_driver.o -c makefile_parser_driver.c

mymake_main.o: mymake.c makefile_parser.c
	gcc $(FLAGS) -o mymake_main.o -c mymake_main.c

mymake.o: mymake.c digraph.c util.c varstring.c
	gcc $(FLAGS) -o mymake.o -c mymake.c

digraph.o: digraph.c
	gcc $(FLAGS) -o digraph.o -c digraph.c

util.o: util.c
	gcc $(FLAGS) -o util.o -c util.c

varstring.o: varstring.c varstring.h
	gcc $(FLAGS) -o varstring.o -c varstring.c

clean: 
	rm -rf mymake makefile_parser_driver makefile_parser.o makefile_parser_driver.o mymake_main.o mymake.o digraph.o util.o varstring.o

