#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int fsd;

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

	return 0;
}
