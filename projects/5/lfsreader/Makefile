all: lfsreader

test:
	./lfsreader ls /nested/directory/example/ example.img > output.txt

notfsck: lfsreader.o
	gcc lfsreader.o -o lfsreader -Wall -Werror -m32

fsck.o: lfsreader.c
	gcc lfsreader.c -c -Wall -m32 

clean: 
	rm -rf *.o lfsreader
