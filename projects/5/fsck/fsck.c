#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "fs.h"

int fsi;
struct superblock * sb;
struct dinode * inodeList;
struct stat * stats;
int * inodebitmap;

/* Hard seeks to a place in the image file */
int seek(int location) {
	return (int) lseek(fsi, location, SEEK_SET);
}

/* prints the contents of an inode */
void printinode(struct dinode * dinode) {
	printf("type:\t\t%hd\n", dinode->type); //File type
	printf("major:\t\t%hd\n", dinode->major); // Major device number
	printf("minor:\t\t%hd\n", dinode->minor); // Minor device number
	printf("nlink:\t\t%hd\n", dinode->nlink); // Number of links to the inode on the system
	printf("size:\t\t%u\n", dinode->size); // Size of the block
	printf("addr[0]:\t0x%x\n", dinode->addrs[0]); // Data block pointer addresses
	printf("addr[1]:\t0x%x\n", dinode->addrs[1]);
	printf("addr[2]:\t0x%x\n", dinode->addrs[2]);
	printf("addr[3]:\t0x%x\n", dinode->addrs[3]);
	printf("addr[4]:\t0x%x\n", dinode->addrs[4]);
	printf("addr[5]:\t0x%x\n", dinode->addrs[5]);
	printf("addr[6]:\t0x%x\n", dinode->addrs[6]);
	printf("addr[7]:\t0x%x\n", dinode->addrs[7]);
	printf("addr[8]:\t0x%x\n", dinode->addrs[8]);
	printf("addr[9]:\t0x%x\n", dinode->addrs[9]);
	printf("addr[10]:\t0x%x\n", dinode->addrs[10]);
	printf("addr[11]:\t0x%x\n", dinode->addrs[11]);
	printf("addr[12]:\t0x%x\n", dinode->addrs[12]);
	printf("addr[13]:\t0x%x\n\n", dinode->addrs[13]); // Indirect pointer address */
}

/* sniffs the super block for inconsistent contents. */
int sniffsb(uint * size, uint nblocks, uint ninodes) {
	/* verifies the size is greater than zero */
	if(*size <= 0)
		return -1;
	/* verifies the size is a multiple of the block size */
	if(*size % BSIZE != 0)
		return -1;
	/* verifies the number of nodes is a multiple of the number of inodes per block */
	if(ninodes % IPB != 0)
		return -1;
	/* if the number of inodes exceeds the size returns -1 */
	if(ninodes > *size)
		return -1;
	 /* repairs the super block by correcting the size value if the size times block size 
 	 * does not equal the file size */
	if(*size * BSIZE != (int) stats->st_size) {
		printf("Repairing Superblock size value\n");
		*size = stats->st_size / BSIZE;
	}
	/* the block doesn't smell */
	return 0;
}

int main(int charc, char * argv[]) {

	/* validates that an input file name is provided */
	if(charc != 2) {
		printf("No valid system image file name provided.\n");
		return -1;
	}

	/* allocates space for the superblock and file stats structs */
	sb = malloc(sizeof(struct superblock));
	stats = malloc(sizeof(struct stat));	

	assert(sb != NULL);
	assert(fstat != NULL);

	/* attempts to open the provided fs image */
	fsi = open(argv[1], O_RDWR);
	assert(fsi != -1);

	/* stats the file */
	fstat(fsi, stats);

	printf("*~*~*~*~*~*~*~*~*~*~*~*\n");
	printf("*   WELCOME TO FSCK   *\n");
	printf("* YOUR SHIT IS FUCKED *\n");
	printf("*~*~*~*~*~*~*~*~*~*~*~*\n\n");

	/*scans over the FS image to the super block*/
	seek(BSIZE);

	/* populates the superblock struct and prints its contents */
	assert(read(fsi, sb, sizeof(struct superblock)) != -1);
	printf("CHECKING SUPER BLOCK CONTENTS\n");
	printf("size:\t\t%d\n", sb->size);
	printf("nblocks:\t%d\n", sb->nblocks);
	printf("ninodes:\t%d\n\n", sb->ninodes);

	/* allocates space for enumerating the inodes and the inode bitmap*/	
	inodeList = malloc(sizeof(struct dinode) * sb->ninodes);
	inodebitmap = malloc(sizeof(int) * (sb->ninodes / IPB));
	assert(inodeList != NULL);

	/* a pointer to size value stored in the superblock that allows the super block to be repaired */
	uint * sbsize;
	sbsize = &sb->size;

	/* callst the function that sniffs the super block */
	if(sniffsb(sbsize, sb->nblocks, sb->ninodes) < 0) {
		printf("Inrepairable Super Block State Found. The FS Image cannot be repaired. Exiting.\n");
		exit(0);
	}

	/* nagivates to the start of the inodes */
	seek(BSIZE * IBLOCK(0));
	/* skips the garbage inode and sets you to the root inode */
	seek(BSIZE * IBLOCK(1) + sizeof(struct dinode));

	/* populates the list of inodes for future access*/
	int i;
	for(i = 0; i < sb->ninodes; i++) {
		assert(read(fsi, inodeList + i, sizeof(struct dinode)) != -1);
	//	printinode(inodeList + i);
	}

	/* seeks to the beginning of the inode bitmap */
	seek(BSIZE * (1 /* garbage block */ + 1 /* super block */ + (sb->ninodes / IPB) /* number of blocks occuppied by inodes */ + 1 /* bitmap */));

	/* reads in the bit map */
	for(i = 0; i < sb->ninodes; i++) {
		assert(read(fsi, inodebitmap + i, sizeof(int)) != -1);
	}

	printf("FSCK complete. Exiting.\n");

	return 0;	
}
