#include "types.h"
#include "user.h"
#include "x86.h"
/*
add global tracker to keep track of pairs Thread-Stack
so that we can free(stack ptr) on thread_join
 */
void lock_init(lock_t * lock) {
	lock->ticket = 0;
	lock->turn = 0;
} 

void lock_acquire(lock_t * lock) {
	int myTurn = fetch_and_add(&lock->ticket, 1);
	while(lock->turn != myTurn)
		; //spin
}

void lock_release(lock_t * lock) {
	fetch_and_add(&lock->turn, 1);
}

int thread_create(void (*start_routine)(void*), void * arg) {
	void * stack = malloc(8192);
	uint temp = (uint) stack;
	if(stack == NULL)
		return -1;
	if((uint)stack % 4096)
		temp = temp + (4096 - (uint)temp % 4096);
	//printf(1,"TC-%p\n", start_routine);
	int pid = clone(start_routine, arg, (void *) temp);
	printf(1, "thread_create-PID-%d\n", pid);
	return pid;
}
int thread_join(int pid){
	//more work to do, free stack
	return join(pid);
}
