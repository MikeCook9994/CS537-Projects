all: notfsck

notfsck: fsck.o
	gcc fsck.o -o notfsck -Wall -Werror -m32

fsck.o: fsck.c
	gcc fsck.c -c -Wall -m32 

clean: 
	rm -rf *.o notfsck
