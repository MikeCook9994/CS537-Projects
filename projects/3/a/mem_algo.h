#ifndef MEMORY_ALGO_INCLUDED
#define	MEMORY_ALGO_INCLUDED

#include "mymem.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define	NUM_POLICIES			2 	//Number of policies inused to manage mem space
#define SLAB_POLICY				0
#define	NEXT_FIT_POLICY			1


typedef struct listNode {
	struct listNode * next;
} Node;

/**
 * Base Address Of Entire Mem Space
 */
void * REGION_HEAD;
void * SLAB_HEAD;
void * NEXT_FIT_HEAD;

Node * TOP_OF_STACK;
struct FreeHeader * NEXT_FIT_FREE_HEADER;

int SLAB_SIZE;
int REGION_SIZE;
int SLAB_REGION_SIZE;
int NEXT_FIT_REGION_SIZE;

int NUM_SLABS_ALLOCATED;

void * Slab_Alloc(int size);
int Slab_Free(void * ptr);

void * NextFit_Alloc(int size);
int NextFit_Free(void * ptr);


void * (*Alloc_Policies[NUM_POLICIES])(int);
int (*Free_Policies[NUM_POLICIES])(void * ptr);

pthread_mutex_t nextfitlock;
pthread_mutex_t slablock;

#endif //MEMORY_ALGO_INCLUDED
