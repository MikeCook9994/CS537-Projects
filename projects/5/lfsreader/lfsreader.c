#define BLKSIZE 4096

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include "structdefs.h"

typedef struct node {
	char * string;
	struct node * next;
} Node;

int fsd;

void seek(int location) {
	assert(lseek(fsd, location, SEEK_SET) != -1);
}

void peruse(void * buf, int count) {
	assert(read(fsd, buf, count) != -1);
}

void error(void) {
	printf("Error!\n");
}

struct node * tokenizeString(char * string) {
	
	char * token;

	Node * head;
	head = malloc(sizeof(Node));
	
	Node * curr;
	curr = head;

	head->string = strtok(string, "/\0\n");
	head->next = NULL;
	
	while(1) {
		token = strtok(NULL, "/\0\n");
		if(token == NULL) {
			break;
		}
		Node * tmp;
		tmp = malloc(sizeof(Node));

		tmp->string = token;
		tmp->next = NULL;
		curr->next = tmp;
		curr = tmp;
	}

	return head;
}

int main(int charc, char *argv[]) {

	/* some basic argument checks */
	if(charc != 4)
		error();
	if((strcmp("cat\0", argv[1]) != 0) && (strcmp("ls\0", argv[1]) != 0))
		error();

	/* attempts to open the file system image */
	fsd = open(argv[3], O_RDWR);
	if(fsd < 0)
		error();

	checkpoint * ckpt;
	inodeMap * rootinodeMap;
	inode * rootiNode;
	Node * head;

	/* tokenizes the path */
	head = tokenizeString(argv[2]);

	/* allocates space for the checkpoint region struct */
	ckpt = malloc(sizeof(checkpoint));

	/* reads in the checkpoint region */
	peruse(ckpt, sizeof(checkpoint));

	/* reads in the first iNode Map that contains the root iNode */
	rootinodeMap = malloc(sizeof(inodeMap));
	seek(ckpt->iMapPtr[0]);
	peruse(rootinodeMap, sizeof(inodeMap));

	/* reads in the first inode Map that contasins the root iNode */
	rootiNode = malloc(sizeof(inode));
	seek(rootinodeMap->inodePtr[0]);
	peruse(rootiNode, sizeof(inode));

	

	return 0;
}