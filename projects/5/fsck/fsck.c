#define BIT_SET(a,b) ((a) |= (1<<(b)))

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "fs.h"

int fsd;
struct superblock * sb;
struct dinode * inodeList;
struct stat * stats;
char * databitmap;

/* Hard seeks to a place in the image file */
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

/* prints the contents of an inode */
void printinode(struct dinode * dinode) {
	printf("type:\t\t%hd\n", dinode->type); //File type
	printf("major:\t\t%hd\n", dinode->major); // Major device number
	printf("minor:\t\t%hd\n", dinode->minor); // Minor device number
	printf("nlink:\t\t%hd\n", dinode->nlink); // Number of links to the 
											  //inode on the system
	printf("size:\t\t%u\n", dinode->size); // Size of the block
	
	int i;
	for(i = 0; i < 13; i++) {
		printf("addr[%d]:\t0x%x\n", i, dinode->addrs[i]);
	}
	printf("\n");	
}

void printSuperBlock(void) {
	printf("SUPER BLOCK CONTENTS\n");
	printf("size:\t\t%d\n", sb->size);
	printf("nblocks:\t%d\n", sb->nblocks);
	printf("ninodes:\t%d\n\n", sb->ninodes);
}

/* sniffs the super block for inconsistent contents. */
int sniffsb(uint * size, uint nblocks, uint ninodes) {

	/* repairs the super block by correcting the size value if the 
	size times block size does not equal the file size */
	if(*size * BSIZE != (int) stats->st_size) {
		*size = stats->st_size / BSIZE;
		seek(BSIZE);
		write(fsd, size, 4);
	}
	/* verifies the size is a multiple of the block size */
	if(*size % BSIZE != 0)
		return -1;
	/* verifies the number of nodes is a multiple of the number of 
	inodes per block */
	if(ninodes % IPB != 0)
		return -1;
	/* verifies that the total number of blocks is greater than or equal to 
	the the number of data blocks plus the number of inode blocks */
	if(nblocks + (ninodes / IPB) > *size)
		return -1;
	/* the block doesn't smell */
	return 0;
}

void clearinode(struct dinode * inode) {
	memset(inode, 0, sizeof(struct dinode));
}

void checkinode(struct dinode * inode) {

	if(inode->type == 0) {
		return;	
	}
	if(inode->nlink == 0 || inode->nlink >= MAXFILE) {
		clearinode(inode);	
	}
	/* checks the inode type */
	if(inode->type > 3 || inode->type < 0) {
		clearinode(inode);
	}
	if(inode->size < 0 || inode->size > (sb->size * BSIZE)) {
		clearinode(inode);
	}
}

void setbit(int datablock) {

	char byte;
	byte = databitmap[datablock / 8];
	
	char mask;
	mask = 1 << datablock % 8;

	databitmap[datablock / 8] = byte | mask;
}

void constructbitmap(void) {

	struct dinode * tmp;
	uint addr;
	uint * indirectblock;

	int i;
	for(i = 0; i < 1 /* garbage block */ + 1 /* super block */ + (sb->ninodes / IPB) /* number of blocks occuppied by inodes */ + 1 /* bitmap */ + 1 /* garbage bitmap block */; i++) {
		setbit(i);
	}	

	int j;
	for(i = 0; i < sb->ninodes; i++) {

		tmp = inodeList + i;

		for(j = 0; j < NDIRECT; j++) {
			addr = tmp->addrs[j];
			if(addr == 0)
				break;
			setbit(addr);
		}

		addr = tmp->addrs[NDIRECT];
		if(addr == 0) 
			continue;
		setbit(addr);

		indirectblock = malloc(BSIZE);
		seek(addr * BSIZE);
		peruse(indirectblock, BSIZE);
		for(j = 0; j < BSIZE / 4; j++) {
			addr = *(indirectblock + j);
			if(addr == 0)
				break;
			setbit(addr);
		}
	}
}

int main(int charc, char * argv[]) {

	/* validates that an input file name is provided */
	if(charc < 2) {
		printf("No valid system image file name provided.\n");
		exit(0);
	}

	// allocates space for the superblock and file stats structs */
	sb = malloc(sizeof(struct superblock));
	assert(sb != NULL);	

	stats = malloc(sizeof(struct stat));	
	assert(stats != NULL);	

	/* attempts to open the provided fs image */
	fsd = open(argv[1], O_RDWR);
	assert(fsd != -1);

	/* stats the file */
	fstat(fsd, stats);

	/*scans over the FS image to the super block*/
	seek(BSIZE);

	/* populates the superblock struct and prints its contents */
	peruse(sb, sizeof(struct superblock));

	/* a pointer to size value stored in the superblock that allows the super 
	   block to be repaired */
	uint * sbsize;
	sbsize = &sb->size;

	/* calls the function that sniffs the super block */ 
	if(sniffsb(sbsize, sb->nblocks, sb->ninodes) != 0) {
		printf("Error\n");
		exit(0);
	}

	/* allocates space for enumerating the inodes and the inode bitmap */	
	inodeList = malloc(sizeof(struct dinode) * sb->ninodes);	
	assert(inodeList != NULL);

	/* skips the garbage inode and sets you to the root inode */
	seek(BSIZE * IBLOCK(1) + sizeof(struct dinode));

	/* populates the list of inodes for future access */
	int i;
	for(i = 0; i < sb->ninodes; i++) {
		peruse(inodeList + i, sizeof(struct dinode));
		checkinode(inodeList + i);
	}

	databitmap = malloc(sb->nblocks / 8 + 1);
	assert(databitmap != NULL);
	memset(databitmap, 0, sb->nblocks / 8 + 1);

	/* checks through the inodes to construct a correct inode bitmap */
	constructbitmap();

	seek(1 /* garbage block */ + 1 /* super block */ + (sb->ninodes / IPB) /* number of blocks occuppied by inodes */ + 1 /* bitmap */);

	write(fsd, databitmap, BSIZE);

	close(fsd);	

	return 0;	
}
