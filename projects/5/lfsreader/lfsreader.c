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

checkpoint * ckpt;
int fsd;

int seek(int location) {
	int returnval;
	returnval = lseek(fsd, location, SEEK_SET);
	assert(returnval != -1);
	return returnval;
}

int peruse(void * buf, int count) {
	int returnval;
	returnval = read(fsd, buf, count);
	assert(returnval != -1);
	return returnval;
}

void error(void) {
	printf("Error!\n");
	exit(0);
}

struct node * tokenizeString(char * string) {
	
	char * token;

	Node * head;
	head = malloc(sizeof(Node));
	
	Node * curr;
	curr = head;

	head->string = strtok(string, "/\0\n");
	if(head->string == NULL)
		return NULL;
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

int getinodenum(inode * inode, char * string) {

	int i;
	for(i = 0; i < 14; i++) {
		if(inode->ptr[i] == -1)
			break;
		
		seek(inode->ptr[i]);
		
		dirEnt * tmp;
		tmp = malloc(sizeof(dirEnt));

		while(1) {
			peruse(tmp, sizeof(dirEnt));
			if(strcmp(string, tmp->name) == 0)
				return tmp->inum;
			if(tmp->inum == -1)
				break;
			
		}	
	}

	return -1;
}

inode * getinode(int inodenum, int offset) {
	
	inode * retinode;
	retinode = malloc(sizeof(inode));

	inodeMap * tmp;
	tmp = malloc(sizeof(inodeMap));

	seek(ckpt->iMapPtr[inodenum / 16]);
	peruse(tmp, sizeof(inodeMap));

	seek(tmp->inodePtr[inodenum % 16]);
	peruse(retinode, sizeof(inode));

	seek(offset);

	return retinode;
}

void ls(inode * dirinode) {
	int i;
	for(i = 0; i < 14; i++) {
		if(dirinode->ptr[i] == -1)
			break;

		int currOffset;
		currOffset = dirinode->ptr[i];

		seek(currOffset);
		
		dirEnt * tmp;
		tmp = malloc(sizeof(dirEnt));

		while(1) {

			currOffset += peruse(tmp, sizeof(dirEnt));
			if(tmp->inum == -1) 
				break;

			inode * tmpinode;
			tmpinode = getinode(tmp->inum, currOffset);

			if(tmpinode->type == 0)
				printf("%s/\n", tmp->name);
			else
				printf("%s\n", tmp->name);
		}	
	}
}

void cat(inode * inode) {
	
	int size;
	size = inode->size;

	char * buf;
	buf = malloc(size);

	seek(inode->ptr[0]);
	peruse(buf, size);

	int i;
	for(i = 0; i < 16; i++) {
		if(size >= BLKSIZE) {
			write(1, buf, BLKSIZE);
			size -= BLKSIZE;
		}
		else {
			write(1, buf, size);
			break;
		}
	}
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

	Node * curr;
	curr = head;

	int currinodenum;
	currinodenum = 0;
	
	inode * currinode;
	currinode = rootiNode;

	/* navaigates to the highest level directory or file based on the path name */
	while(curr != NULL) {
		currinodenum = getinodenum(currinode, curr->string);
		if(currinodenum == -1)
			error();
		curr = curr->next;
	
		currinode = getinode(currinodenum, rootinodeMap->inodePtr[0]);
	}

	inode * finalinode = currinode;

	if(strcmp(argv[1], "ls\0") == 0) {
		if(finalinode->type == 1)
			error();
		else
			ls(finalinode);
	}
	else if (strcmp(argv[1], "cat\0") == 0) {
		if(finalinode->type == 0)
			error();
		else
			cat(finalinode);
	}

	return 0;
}