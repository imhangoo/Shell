#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>



// macro definitions
#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define EACH_TOKEN_SIZE 30
#define HISTORY_DEPTH 10

#include "history.h"

extern struct history *history;
extern _Bool is_history_command;

void sigint_handler(int signo);

void read_command(char *buff, char *tokens[], _Bool *in_background);

int tokenize_command(char *buff, char *tokens[]);

void set_array_null(char *array[], int length, int free_memory);

int run_external(char *tokens[], bool in_background);

int string_in_arr(char* needle, const char* haystack[]);

void internal_exit(char *tokens[]);

void internal_cd(char *tokens[]);

void internal_pwd(char *tokens[]);

void internal_history(char *tokens[]);

void _pwd();

void _prompt();

void wrapped_write(char* str);


#endif
