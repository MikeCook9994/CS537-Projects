#include "types.h"
#include "stat.h"
#include "user.h"


/*
PROBLEM: both helloe word and hellow mike print out hello world -> set up stack wrong??
 */
void helloWorld() {
	sleep(12); // sleep 12s
	printf(1, "Hello World\n");
	exit();
}
void helloZen() {
	printf(1, "Hello Zen\n");
	exit();
}
void helloMike() {
	printf(1, "Hello Mike\n");
	int pid = thread_create(helloZen, NULL);
	thread_join(pid);
	exit();
}
int main(int argc, char * argv[]) {

	thread_create(helloWorld, NULL);
	thread_create(helloMike, NULL);

	while(-1 != thread_join(-1)){}

	printf(1,"%s\n", "ciao");
	exit();
}