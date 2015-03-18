all: mysh mytee

mysh: clprogs.o commandhandler.o parsecl.o shell.o
	gcc clprogs.o commandhandler.o parsecl.o shell.o -o mysh -Wall -m32

commandhandler.o: commandhandler.c
	gcc commandhandler.c -c -Wall -m32

parsecl.o: parsecl.c
	gcc parsecl.c -c -Wall -m32

clprogs.o: clprogs.c
	gcc clprogs.c -c -Wall -m32

shell.o: shell.c
	gcc shell.c -c -Wall -m32

mytee: mytee.c
	gcc mytee.c -o mytee -Wall -m32

clean:
	rm -rf *.o mysh mytee 
