#include "cmstructs.h"
#include "defs.h"
#include <sys/wait.h>
#include <sys/types.h>

int executeProcesses(CmNode * command) {

	pid_t pid;
	int status;

	CmNode * curr;
	curr = command;

	char ** argv;

	pid = fork();
	argv = buildArgList(curr);
		
	if(pid == -1) {
		return -1;
	}
	else if(pid > 0) {
		if(waitpid(pid, &status, 0) == -1)
			return -1;
	}
	else {
		if(execvp(argv[0], argv) == -1) {
			fprintf(stderr, "Error!\n");
			_Exit(0);
		}
	}

	return 0;
}

char ** buildArgList(CmNode * commandNode) {
	
	int size;
	size = 0;

	ArgNode * curr;
	curr = commandNode->command->arguments;
	
	while(curr != NULL) {
		size++;
		curr = curr->next;
	}	

	char ** argv;
	argv = malloc(sizeof(char *) * size);

	int i;
	curr = commandNode->command->arguments;
	for(i = 0; i < size; i++){
		argv[i] = curr->argument;
		curr = curr->next;
	}

	return argv;
}
