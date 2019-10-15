//  preprocessor  directives  go here
#define World Class
#define test test2
// end  preprocessor  directives

void  test()
{
	printf("Hello  World");
}

/* I have tried to change World to class and test to test2 in the defines above.\
Because the way tokenization works, the preprocessor can successfully change test to test2, but\
it will fail to change World to Class. test is one token so it can be replaced, but World is\
within a string " ", so the preprocessor takes the entire string "Hello World" as one token\
therefore, the preprocessor does not see the token World anywhere in the file, only "Hello world"\
so it does not think there is anywhere in the file where it can replace World with Class. */
