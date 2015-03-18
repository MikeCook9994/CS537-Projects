all: clean tester thread clean_objects

tester: mymem.c tester.c
	gcc -c -fpic -lpthread mymem.c mem_algo.c -Wall -Werror -g
	gcc -shared -lpthread -o libmem.so mymem.o mem_algo.o -g
	gcc -lmem -L. -lpthread -o tester tester.c -Wall -Werror -g
	
thread: mymem.c thread.c
	gcc -c -fpic -lpthread mymem.c mem_algo.c -Wall -Werror -g
	gcc -shared -lpthread -o libmem.so mymem.o mem_algo.o -g
	gcc -lmem -L. -lpthread -o thread thread.c -Wall -Werror -g
	
clean_objects:
	rm -rf *.o

test:
	python ~cs537-1/testing/p3a/MemTest.pyc .

clean:
	rm -rf *.o *.so tester thread
