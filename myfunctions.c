#include "myfunctions.h"

/**
 * Handler for SIGINT - prints history if SIGINT caught, and sets
 * itself up as the interrupt handler again.
 */
void sigint_handler(int signo) {
    if (signo == SIGINT) {
	wrapped_write(" received, printing history...\n");
	print_history();
    }
}

/**
 * Read commands from user then parse/tokenize it by tokenize_command()
 * function
 */
void read_command(char *buff, char *tokens[], _Bool *in_background)
{
    *in_background = false;
    set_array_null(tokens, NUM_TOKENS, 1);
    int i = 0; // index of buff

    // Read input

    int length, is_blank;
    do {
	is_blank = 0;
	errno = 0;
	_prompt();
	length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1); 
    // check whether the user input nothing 
	while(buff[i] == ' '&&i < length)
	    i++;
	if (i == length-1)
	    is_blank = 1;
	if ( (length < 0) && (errno != EINTR) )	{
	    perror("Unable to read command. Terminating.\n");
	    exit(-1); /* terminate with error */
	}
    } while (is_blank || errno == EINTR);


    // Null terminate and strip \n.
    buff[length] = '\0';
    if (buff[strlen(buff) - 1] == '\n') {
	buff[strlen(buff) - 1] = '\0';
    }

    is_history_command = false;
    // if command starts with ! replace buffer with history
    i = 0;

    while(buff[i] == ' '&&i < length)
	i++;
    if (buff[i] == '!')
	execute_history(buff,i);


    // Tokenize (saving original command string)
    int token_count = tokenize_command(buff, tokens);
    if (token_count == 0) {
	return;
    }

    // Extract if running in background:
    if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
	*in_background = true;
	tokens[token_count - 1] = 0;
    }
   
    // add valid command to history
    if (buff[0]!='\0')
    {
    	add_to_history(buff);
    }
    
}

/**
 * Parse commands into tokens then return the number of tokens
 *
 */
int tokenize_command(char *buff, char *tokens[]) {
	// index of buff[], -- buff[i]
	int i = 0;
	// index of tokens[], -- tokens[j]
	int j = 0;
	// index of tokens[j][]; -- tokens[j][k]
	int k = 0;

	// allocate space for first token
	tokens[0] = malloc(EACH_TOKEN_SIZE * sizeof(char));
	// start from the first char which is not ' '
	while(buff[i] == ' ') 
		i++;
	// check each char in the buff string
	do {
	    // ' ' is the separator of tokens
	    if (buff[i] != ' ') {
		// put chars into each token
		tokens[j][k++] = buff[i];
	    } else if (buff[i+1] != ' ' && buff[i+1] != '\0') {
                // avoid mistakes caused by consecutive ' 's

		// NULL terminated
		tokens[j][k] = '\0';
		// next token(next string)
		j++;
		// set the index of next token to 0
		k = 0;
		tokens[j] = malloc(EACH_TOKEN_SIZE * sizeof(char));
	    }
	    i++;
	} while (buff[i] != '\0');

	// null-terminate the last token (only tokens ending in ' ' would
	// have been null-terminated in the do..while loop above
	tokens[j][k] = '\0';
	if (tokens[j][k-1] == '&' && strcmp(tokens[j],"&"))
	{
		tokens[j][k-1] = '\0';
		j++;
		tokens[j] = malloc(EACH_TOKEN_SIZE * sizeof(char));
		tokens[j][0] = '&';
		tokens[j][1] = '\0';

	}


	// return token_count
	return j+1;
}

void set_array_null(char *array[], int length, int free_memory) {
    for (int ii = 0; ii < length; ++ii) {
	if (array[ii]) {
	     if (free_memory)
		  free(array[ii]);
	     array[ii] = NULL;
	}
    }
}

int run_external(char *tokens[], bool in_background) {
    pid_t fpid = fork();
    if (fpid < 0) {
	wrapped_write("shell: Error in fork!\n");
    } else if(fpid == 0) { // child process
	// change the context of the child process(pid unchanged)
	if (execvp(tokens [0], tokens)) {
	    wrapped_write("shell: ");
	    wrapped_write(tokens[0]);
	    wrapped_write(": Error executing command\n");
	    exit(1);
	}

    } else { // parent process
	// if child process does not run in background, parent process
	// waits for child process
	if (in_background == false)
	{
	    waitpid(fpid,NULL,0);
	}
    }
    return 0;
}

int string_in_arr(char* needle, const char* haystack[]) {
    int ii = 0;
    while (haystack[ii]) {
	if (!strcmp(needle, haystack[ii])) {
	    return ii;
	}
	ii++;
    }
    return -1;
}

/**
 * The internal_command() commands are the built-in shell commands.
 * They do not all necessarily need the tokens, but due to the way
 * they are called, it is necessary for all the functions to have the
 * same signature.
 */

void internal_exit(char *tokens[]) {
     set_array_null(tokens, NUM_TOKENS, 1);
    free_history(history);
    exit(0);
}

void internal_cd(char *tokens[]) {
    if (tokens[1]) {
	int retval = chdir(tokens[1]);
	if (retval) {
	    char *errmsg = strerror(errno);
	    wrapped_write(tokens[0]);
	    wrapped_write(": ");
	    wrapped_write(tokens[1]);
	    wrapped_write(": ");
	    wrapped_write(errmsg);
	    wrapped_write("\n");
	}
    }
}

void internal_pwd(char *tokens[]) {
    _pwd();
    write(STDOUT_FILENO, "\n", sizeof("\n"));
}

void internal_history(char *tokens[]) {
    print_history(history);
}

void _pwd() {
    char *cwd;
    char buf[PATH_MAX + 1];
    cwd = getcwd(buf, sizeof(buf));
    write(STDOUT_FILENO, cwd, strlen(cwd));
}

void _prompt() {
    _pwd();
    wrapped_write(" > ");
}

/**
 * This function is used to wrap write() function and output string,
 * since printf() may not work well in this project
 */    
void wrapped_write(char* str) {
    write (STDOUT_FILENO, str, strlen(str));
}

