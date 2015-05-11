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
	exit(0);
}

void printckpt(checkpoint * ckpt) {
	printf("ckpt->size = %d\n", ckpt->size);
	int i;
	for(i = 0; i < INODEPIECES; i++) {
		printf("ckpt->iMapPtr[%d] = %d\n", i, ckpt->iMapPtr[i]);
	}
}

void printdir(dirEnt * dir) {
	printf("dir->name = %s\n", dir->name);
	printf("dir->inum = %d\n", dir->inum);
}

void printinode(inode * inode) {
	printf("inode->size = %d\n", inode->size);
	if(inode->type == 0)
		printf("inode->type = MFS_DIRECTORY\n");
	else
		printf("inode->type = MFS_REGULAR_FILE\n");
	int i;
	for(i = 0; i < 14; i++) {
		printf("inode->ptr[%d] = %d\n", i, inode->ptr[i]);
	}
}

void printinodeMap(inodeMap * inodemap) {
	int i;
	for(i = 0; i < 16; i++) {
		printf("inodemap->inodePtr[%d] = %d\n", i, inodemap->inodePtr[i]);
	}
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

int findinode(inode * inode, char * string) {

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

void ls(inode * inode) {
	int i;
	for(i = 0; i < 14; i++) {
		if(inode->ptr[i] == -1)
			break;
		
		seek(inode->ptr[i]);
		
		dirEnt * tmp;
		tmp = malloc(sizeof(dirEnt));

		while(1) {
			peruse(tmp, sizeof(dirEnt));
			if(tmp->inum == -1)
				break;
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

	Node * curr;
	curr = head;

	int currinodenum;
	currinodenum = 0;
	
	inode * currinode;
	currinode = rootiNode;

	/* navaigates to the highest level directory or file based on the path name */
	while(curr != NULL) {
		currinodenum = findinode(currinode, curr->string);
		if(currinodenum == -1)
			error();
		curr = curr->next;

		inodeMap * tmp;
		tmp = malloc(sizeof(inodeMap));
		
		seek(ckpt->iMapPtr[currinodenum / 16]);
		peruse(tmp, sizeof(inodeMap));

		seek(tmp->inodePtr[currinodenum % 16]);
		peruse(currinode, sizeof(inode));
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
	else {
		error();
	}

	return 0;
}