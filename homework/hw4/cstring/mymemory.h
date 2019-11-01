#pragma once
#define MY_MEMORY

// Copy string
char * mystrdup(const char * mystr);

// Add word to array
char ** add_word(char ** array, unsigned int wordcount, const char * word_to_add);

// Free word array (including all the strings it contains!)
void free_word_array(char ** array, unsigned int wordcount);

// Print the word array to the screen, one word per line.
void print_word_array(char ** array, unsigned int wordcount);
