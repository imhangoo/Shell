/**
 * testsuite for the history struct and associated functions
 */

#include "history.h"

int main() {

	struct history 	*hs = malloc(sizeof(struct history));

	init_history(hs);
	for (int ii = 0; ii < 4; ++ii) {
		add_to_history(hs, "command the first");
		add_to_history(hs, "command the second");
		add_to_history(hs, "command the third");
		add_to_history(hs, "command the fourth");
		
		print_history(hs);
		printf("-----\n");
	}

	char command[COMMAND_LENGTH];
	retrieve_from_history(hs, command, 2);
	printf("%s\n",command);

	retrieve_from_history(hs, command, 9);
	printf("%s\n",command);

	retrieve_from_history(hs, command, 15);
	printf("%s\n",command);
	
	return 0;
}
