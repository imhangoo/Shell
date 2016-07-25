#include "myfunctions.h"
#include "history.h"
struct history *history;
_Bool is_history_command = false;

/**
  * Main and Execute Commands
  */
int main (int argc, char* argv[])
{
    char input_buffer[COMMAND_LENGTH];
    char *tokens[NUM_TOKENS];

    
    history = malloc(sizeof(struct history));
    init_history(history);
   
    // array of internal commands
    const char* internal_commands[] = {
	"exit",
	"cd",
	"pwd",
	"history",
	NULL
    };

    // array of function pointers accepting string arrays
    void (*internal_functions[]) (char **) = {
	&internal_exit,
	&internal_cd,
	&internal_pwd,
	&internal_history,
	NULL
    };

    set_array_null(tokens, NUM_TOKENS, 0);

    // set up history printing as handler for SIGINT
    struct sigaction handler;
    handler.sa_handler = sigint_handler;
    handler.sa_flags = 0;
    sigemptyset(&handler.sa_mask);
    sigaction(SIGINT, &handler, NULL);
    
    while (true) {

        // Get command

	_Bool in_background = false;
        read_command(input_buffer, tokens, &in_background);
        if (is_history_command)
	{
		wrapped_write(tokens[0]);
		wrapped_write("\n");
	}

	// first check if command is internal
	int internal_index = string_in_arr(tokens[0], internal_commands);

	if (internal_index > -1) {
	    // index the array of function pointers, and pass token array
	    (*internal_functions[internal_index]) (tokens);
      
	} else if (input_buffer[0] != '\0') {
	    // if not internal, fork a child process

	    run_external(tokens, in_background);
	}
	
	// Cleanup any previously exited background child processes
        // (The zombies)
	while (waitpid(-1, NULL, WNOHANG) > 0); // do nothing

    }

    return 0;
}
