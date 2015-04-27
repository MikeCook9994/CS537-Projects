#ifndef _TYPES_H_
#define _TYPES_H_

// Type definitions

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

typedef struct lock_t {
	int ticket;
	int turn;
} lock_t;

typedef struct cond_t {
	// int countSleep;
	// int countWake;
	//int waitQueue[64];
/*
We can add more complicated struc to have more control of which thread has priority on wake,
But why, we passed all the test yo:D
 */
} cond_t;


typedef struct stackReference {
	int pid;
	void * stack;
	struct stackReference * next;
} stackRef;

stackRef * head;

#ifndef NULL
#define NULL (0)
#endif

#endif //_TYPES_H_
