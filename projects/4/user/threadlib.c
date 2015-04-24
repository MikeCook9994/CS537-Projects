#include "types.h"
#include "user.h"
#include "x86.h"

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
	void * stack = malloc(4096);
	return clone(start_routine, arg, stack);
}