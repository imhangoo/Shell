CC=cc
CXX=CC
CCFLAGS= -g -std=c99 -Wall -Werror

all: shell

# Without "%.h", the changes of .h files would not be detected, 
# when someone only change .h files without changing .c files
%.o : %.c %.h
	$(CC) -c $(CCFLAGS) $<

shell:	shell.o myfunctions.o history.o
	$(CC) -g -o shell  shell.o myfunctions.o history.o

clean:
	rm -f core *.o shell
