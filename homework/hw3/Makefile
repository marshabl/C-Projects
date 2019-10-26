# some useful variables
FLAGS=-Wall -Werror -pedantic -std=c11
FILENAME=substcode
INPUTFILENAME=assignment
OBJFILE=$(FILENAME).o
PPFILE=$(FILENAME).i
EXECUTABLE=$(FILENAME)
CFILE=$(FILENAME).c

# all depends on outputfile
all: $(EXECUTABLE)

# executable depends on objfile
$(EXECUTABLE): $(OBJFILE)
	gcc $(FLAGS) -o $(EXECUTABLE) $(OBJFILE)

#objfile depends on preprocessed file
$(OBJFILE): $(PPFILE)
	gcc $(FLAGS) -c $(PPFILE) -o $(OBJFILE)

#preprocessed depends on c file
$(PPFILE): $(CFILE)
	gcc $(FLAGS) -E $(CFILE) -o $(PPFILE)
