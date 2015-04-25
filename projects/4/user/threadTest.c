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
	//exit();
}
void helloMike() {
	printf(1, "Hello Mike\n");
	thread_create(helloZen, NULL);
	//exit();
}
int main(int argc, char * argv[]) {
	//printf(1,"%p\n", (void *)helloWorld);
	//printf(1,"%p\n", (void *)helloMike);
	int pid = thread_create(helloWorld, NULL);
	int pid1 = thread_create(helloMike, NULL);

	/**
	 * WTF pid == pid1 = 0???????????????
	 */
	printf(1, "pid-%d\n", pid); 
	printf(1, "pid1-%d\n", pid1);

	thread_join(pid);
	thread_join(pid1);
	printf(1,"%s\n", "ciao"); // this should print after all children finish, so WEAIRDDDDDDDDDDDDDDDD
	exit();
}