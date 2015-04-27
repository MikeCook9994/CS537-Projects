#include "types.h"
#include "user.h"
#include "x86.h"

//FIX STACK FREEING

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

void cv_wait(cond_t * cv, lock_t * lock) {

	if(cv == NULL)
	{
		cv = (cond_t *) malloc(sizeof(cond_t));
	}

	cv_sleep(cv, lock);
}

void cv_signal(cond_t * cv) {
	
	if(cv != NULL)
		cv_wake(cv);
}

int thread_create(void (*start_routine)(void*), void * arg) {
	void * stack = malloc(8192);
	uint temp = (uint) stack;
	if(stack == NULL)
		return -1;
	if((uint)stack % 4096)
		temp = temp + (4096 - (uint)temp % 4096);

	int pid = clone(start_routine, arg, (void *) temp);

	/* pushes the new stackReference onto the list */
/*	stackRef * tmp = (stackRef *) malloc(sizeof(stackRef));
	tmp->pid = pid;
	tmp->stack = stack;
	tmp->next = NULL;
	head = tmp;
*/
	return pid;
}
int thread_join(int pid){

	int ppid;
	ppid = join(pid);
	if(ppid == -1)
		return -1;

/*	stackRef * curr = head;
	stackRef * prev = NULL;
	while(curr != NULL) {
		if(curr->pid == ppid)
			break;
		prev = curr;
		curr = curr->next;
	}
	Free: 
		free(curr->stack);
		prev->next = curr->next;
		free(curr);
*/
	return ppid;
}
