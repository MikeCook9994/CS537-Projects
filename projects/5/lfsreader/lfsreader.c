#define BLKSIZE 4096

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include "structdefs.h"

int fsd;
checkpoint * ckpt;
inodeMap * imap[INODEPIECES];

int seek(int location) {
	return (int) lseek(fsd, location, SEEK_SET);
}

void error(void) {
	printf("Error!\n");
	exit(0);
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

	/* allocates space for the checkpoint region struct */
	ckpt = malloc(sizeof(checkpoint));

	/* reads in the checkpoint region */
	assert(read(fsd, ckpt, sizeof(checkpoint)) != -1);	


	/* locates all the imap regions and sticks them in an array for latter access */
	int i;
	for(i = 0; i < INODEPIECES; i++) {
			imap[i] = (ckpt->iMapPtr[i] + (void *)ckpt); 
			printf("imap[%d] address: %p\n", i, imap[i]);
			int j;
			for(j = 0; j < 16; j++) {
			printf("imap[%d]->inodePtr[%d]: %d\n", i, j, imap[i]->inodePtr[j]);
		} 
	}
	
	return 0;
}
