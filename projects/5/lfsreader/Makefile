all: lfsreader

test:
	./lfsreader ls /nested/directory/example/ example.img > output.txt

notfsck: lfsreader.c
	gcc -o lfsreader -Wall -Werror -m32

clean: 
	rm -rf *.o lfsreader
