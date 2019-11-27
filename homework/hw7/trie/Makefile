# some useful variables
FLAGS=-Wall -Werror -pedantic -std=c11

test: trie_test.o trie.o
	gcc $(FLAGS) -o trie_test trie_test.o trie.o -lcunit
	./trie_test

clean: 
	rm -rf trie_test trie_test.o trie.o

trie_test.o: trie_test.c trie.o
	gcc $(FLAGS) -c trie_test.c

trie.o: trie.c trie.h
	gcc $(FLAGS) -c trie.c

