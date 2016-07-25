#include <stdio.h>
#include "history.h"
#include "myfunctions.h"

void init_history() {
    history->command_count = 0;
    history->front = 0;
    history->rear = 0;
}

void add_to_history(char *str) {
    strcpy(history->commands[history->rear % HISTORY_DEPTH], str);
	history->rear = (history->rear+1) % HISTORY_DEPTH;
    history->command_count++;
    if(history->command_count > HISTORY_DEPTH)
    	history->front = (history->front+1) % HISTORY_DEPTH;   
}

void print_history() {
  if (history->command_count == 0)
      return;
  // next position of command to be output
  int p = history->front;
  // number of first command to be printed
  int start_number = (history->command_count <= HISTORY_DEPTH) ?
      1 : history->command_count - HISTORY_DEPTH + 1;
  // int to string
  char history_line[COMMAND_LENGTH+10]; 
  // print history list line by line
  do{
      sprintf(history_line,"%d\t%s\n",start_number++,history->commands[p]);
      wrapped_write(history_line);
      p = (p + 1) % HISTORY_DEPTH;
  } while (p!=history->rear);
   
}

void retrieve_from_history(char *buff, int index) {
    int count = history->command_count;
    if (index < count - HISTORY_DEPTH || index > count || index < 1 || count == 0) {
      wrapped_write("No command or command out of range\n");
      /* if comment out add_to_history(buff), 
       * invalid commands like "!!" "!33" 
       * would not be put in history, like bash does
       */
    //add_to_history(buff); 
      buff[0] = '\0';
      return;
    }
    is_history_command = true;
    index = (index -1) % HISTORY_DEPTH;
    strcpy(buff, history->commands[index]);
}

/**
 * Check if the command starts with !, and if so,
 * retrieve the command from history and replace the
 * buffer. i is the index of first '!'
 */
void execute_history(char *buff, int i) {
    // absolute index of history command
    int abs_index;

    // if it's just ! ignore it
    if (buff[i+1] == '\0' || buff[i+1] == ' '){
        return;
    }
    // otherwise check whether it is "!!" or not, if it is "!!"
    // ignore everything after "!!"
    abs_index = (buff[i+1] == '!')? history->command_count: atoi(&buff[i+1]);
    retrieve_from_history(buff, abs_index);
    return;
}

void free_history() {
    free(history);
}
