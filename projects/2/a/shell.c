#include "defs.h"

/* wrapper function for printing the error when needed*/
void error() {
	fprintf(stderr, "Error!\n");
}

int main(int charc, char *argv[]) {

	maxcli = 1024;

/* the buffer for command line input*/
	cli = malloc(sizeof(char) * 1024);	

/* holds the last token return by strtok */
	char * token;

	int biprognum;

/*The main program loop*/
	while(1) {
		
		printf("mysh> ");

/* if getline fails to grab from stdin prints error and jumps back to the
 *top of the loop. */
		if(getline(&cli, &maxcli, stdin)	== -1) {
			error();
			continue;
		}

/* strips the new line character off the grabbed line */
		strtok(cli, "\n");
/* grabs the first command line command argument to determine how it
 * should be handled */
		token = strtok(cli, " \t");
		if(token == NULL)
			continue;
	
		else if((biprognum = checkCLProgs(token)) > 0) {
			if(executeCLProg(token, biprognum) == -1)
				error();
		}

		else {
			if(parsecli(cli, token) == -1) 
				error();
		}
	}
	return 0;
}
