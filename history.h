#ifndef _HISTORY_H_
#define _HISTORY_H_

#include "myfunctions.h"

/*
* circular queue
*/
struct history {
	// position of first of last 10 commands
	int front ;
	// position of next command to be added
	int rear ;
	int command_count;
	char commands[HISTORY_DEPTH][COMMAND_LENGTH];
};

/**
 * History encapsulation functions
 */
void init_history();

void add_to_history(char* str);

void print_history();

void retrieve_from_history(char *command, int index);

void execute_history(char *buff,int i);

void free_history();

#endif
