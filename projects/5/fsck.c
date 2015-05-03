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
struct dinode * dinode;
struct stat * stats;

int seek(int location) {
	return (int) lseek(fsi, location, SEEK_SET);
}

/* sniffs the super block for inconsistent contents. */
int sniffsb(uint size, uint nblocks, uint ninodes) {
	if(size <= 0)
		return -1;
	if(size % BSIZE != 0)
		return -1;
	if(ninodes % IPB != 0)
		return -1;
	if(size * BSIZE != (int) stats->st_size)
		return -1;
	if(ninodes > size)
		return -1;
	return 0;
}

int main(int charc, char * argv[]) {

	if(charc != 2) {
		printf("No valid system image file name provided.\n");
		return -1;
	}

	sb = malloc(sizeof(struct superblock));
	assert(sb != NULL);

	dinode = malloc(sizeof(struct superblock));
	assert(dinode != NULL);

	stats = malloc(sizeof(struct stat));
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
/*printf("size:\t\t%d\n", sb->size);
	printf("nblocks:\t%d\n", sb->nblocks);
	printf("ninodes:\t%d\n\n", sb->ninodes); */

	if(sniffsb(sb->size, sb->nblocks, sb->ninodes) < 0) {
		printf("Inrepairable Super Block State Found. The FS Image cannot be repaired. Exiting.\n");
		exit(0);
	}

	/* nagivates to the start of the inodes */
	seek(BSIZE * IBLOCK(0));
	/* skips the garbage inode and sets you to the root inode */
	seek(BSIZE * IBLOCK(1) + sizeof(struct dinode));

	/* populates the root inode struct and prints its contents */
/*assert(read(fsi, dinode, sizeof(struct dinode)) != -1);
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
	printf("addr[13]:\t0x%x\n", dinode->addrs[13]); // Indirect pointer address */

	printf("FSCK complete. Exiting.\n");

	return 0;	
}
