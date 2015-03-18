#include "defs.h"

int changeDir() {

	char * token;

	token = strtok(NULL, " \t");
	
	if(token == NULL) {
		token = getenv("HOME");
		if(chdir(token) == -1)
			return -1;
	}
	
	else {
		if(chdir(token) == -1)	
			return -1;
	}
		
	return 1;
}

void printWorkingDir() {	
	
	char * dir;
	dir = NULL;

	dir = getcwd(dir, (size_t) NULL);
	printf("%s\n", dir);

}

int executeCLProg(char * token, int prognum) {
	
	if(prognum == 1) {
		token = strtok(NULL, " \t");
		if(token != NULL)
			return -1;
		else
			exit(0);
	}
		
	else if(prognum == 2) {
		if(changeDir() == -1)
			return -1;

	}

	else printWorkingDir();
	
	return 1;
}

int checkCLProgs(char * token) {
	if(strcmp(token, "exit") == 0) return 1;
	else if(strcmp(token, "cd") == 0) return 2;
	else if(strcmp(token, "pwd") == 0) return 3;
	else return 0;
}
