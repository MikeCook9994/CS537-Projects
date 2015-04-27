#include "types.h"
#include "x86.h"

void klock_init(lock_t * lock) {
	lock->ticket = 0;
  	lock->turn = 0;
} 

void klock_acquire(lock_t * lock) {
	int myTurn = fetch_and_add(&lock->ticket, 1);
	while(lock->turn != myTurn)
    	; //spin
}

void klock_release(lock_t * lock) {
	fetch_and_add(&lock->turn, 1);
}