#include "mem_algo.h"

void * (*Alloc_Policies[])(int) = {
	[SLAB_POLICY]		Slab_Alloc,
	[NEXT_FIT_POLICY]	NextFit_Alloc,	
};

int (*Free_Policies[])(void * ptr) = {
	[SLAB_POLICY]		Slab_Free,
	[NEXT_FIT_POLICY]	NextFit_Free,
};

void * Slab_Alloc(int size){ 

	pthread_mutex_lock(&slablock);

	if(TOP_OF_STACK == NULL) { 
		pthread_mutex_unlock(&slablock);
		return NextFit_Alloc(size);
	}

	void * free_slab_ptr = TOP_OF_STACK;

	TOP_OF_STACK = TOP_OF_STACK->next;

	NUM_SLABS_ALLOCATED++;

	/* clear memory to zero */
	unsigned char * p = free_slab_ptr;
	int i;
	for(i = 0; i < 16; i++) {
		*p++ = (unsigned char) 0;
	}

	pthread_mutex_unlock(&slablock);

	return free_slab_ptr;
}

int Slab_Free(void * ptr){

	pthread_mutex_lock(&slablock);

	/**
	 * Pointers out of Bound
	 */
	if((ptr == NULL) || (ptr < REGION_HEAD) || (ptr >= NEXT_FIT_HEAD)) {
		pthread_mutex_unlock(&slablock);
		return -1;
	}

	/**
	 * Error on Invalid Slab Pointers
	 * Slab Pointers have to be Multiple of SlabSize
	 */
	if((size_t) ptr % SLAB_SIZE != 0){
		pthread_mutex_unlock(&slablock);
		return -1;
	}

	/** 
	 * Error On Freeing Already Freed Slabs
	 */
	Node * curr = TOP_OF_STACK;
	while(curr != NULL) {
		if(curr == ptr) {
			pthread_mutex_unlock(&slablock);
			return -1;
		}
		curr = curr->next;
	}

	/**
	 * Push Slab Back on Stack
	 */
	Node * tmp = TOP_OF_STACK;
	TOP_OF_STACK = (Node *) ptr;
	TOP_OF_STACK->next = tmp;

	/**
	 * Decrement # slabs allocated
	 */
	NUM_SLABS_ALLOCATED--;

	pthread_mutex_unlock(&slablock);

	return 0;
}

void * NextFit_Alloc(int size){
	
	printf("grabbing alloc\n");
	pthread_mutex_lock(&nextfitlock);
	printf("grabbed alloc\n");

	if(NEXT_FIT_FREE_HEADER == NULL){
		pthread_mutex_unlock(&nextfitlock);
		return NULL;
	}
	/* rockstar as fuck method of finding multiple of 16 */
	size = ((size + 15) & ~15); 

	/**
	 * Searching for the next fit
	 */	
	int fitCount = 0;
	int turn = 0;
	struct FreeHeader * curr;
	struct FreeHeader * nextFit;
	curr = NEXT_FIT_FREE_HEADER;
	nextFit = NULL;


	/* REWRITE THIS STUFF */
	while(fitCount < 2 && turn < 1) {		

		if(curr == NULL) {
			curr = NEXT_FIT_FREE_HEADER;
			turn++;
		}

		if(curr == NULL && turn > 0) {
			//meaning already loop alreay twice
			break;
		}

		//SEGFAULT HERE TODO
		if(curr->length >= size){
			fitCount++;
		}
	
		if(fitCount > 1){
			//ignore the first time we find a fit, only take the 2nd time
			nextFit = curr;
		}
		
		curr = curr->next;
	}

	if(nextFit == NULL) {
		pthread_mutex_unlock(&nextfitlock);
		return NULL;
	}

	struct FreeHeader * nextFree = nextFit->next;

	if(nextFit->length > size){
		struct FreeHeader *tmp;
		tmp = (struct FreeHeader*)((char*)(nextFit) + size + (int)sizeof(struct FreeHeader));
		tmp->next = nextFit->next;
		tmp->length = nextFit->length - size -(int)sizeof(struct FreeHeader);
		nextFree = tmp;
	}

	
	if(nextFit == NEXT_FIT_FREE_HEADER) {
		NEXT_FIT_FREE_HEADER = nextFree;
	}
	else {
		curr = NEXT_FIT_FREE_HEADER;
		
		while(curr->next != nextFit) {
			curr = curr->next;
		}
		curr = nextFree;
	}

	struct AllocatedHeader * ret = (struct AllocatedHeader *) (nextFit);
	ret->length = size;
	ret->magic = (void *) MAGIC;
  char * charret = ((char *)(ret) + (int)sizeof(struct AllocatedHeader));

	printf("releasing alloc\n");
	pthread_mutex_unlock(&nextfitlock);
	printf("released alloc\n");

	return charret;
}

int NextFit_Free(void * ptr){

	printf("grabbing free\n");	
	pthread_mutex_lock(&nextfitlock);
	printf("grabbed free\n");

	/**
	 * Pointers out of Bound
	 */
	if((ptr == NULL) || (ptr < NEXT_FIT_HEAD) || (ptr >= (NEXT_FIT_HEAD + NEXT_FIT_REGION_SIZE))) {
		pthread_mutex_unlock(&nextfitlock);
		return -1;
	}

	struct AllocatedHeader * alloted = (struct AllocatedHeader *)((char *)(ptr) - (int)sizeof(struct AllocatedHeader));

	if(alloted->magic != (void *)MAGIC){
		pthread_mutex_unlock(&nextfitlock);
		return -1;
	}

	struct FreeHeader * newFree = (struct FreeHeader *)(alloted);

	/* handles freeing blocks with addresses smaller than  that of the current NEXT_FIT_FREE_HEADER */ 
	if(NEXT_FIT_FREE_HEADER == NULL || newFree < NEXT_FIT_FREE_HEADER) {
		newFree->next = NEXT_FIT_FREE_HEADER;
		NEXT_FIT_FREE_HEADER = newFree;
	}

	/* handles freeing blocks with addresses great than that of the current NEXT_FIT_FREE_HEADER */
	else {
		struct FreeHeader * curr = NEXT_FIT_FREE_HEADER;
		//SEGFAULT HERE TODO
		while(curr != NULL && curr->next != NULL){
			if((struct FreeHeader *)((char *)curr + curr->length + (int)(sizeof(struct FreeHeader))) == newFree) {
				break;
			}
			curr = curr->next;
		}
		newFree->next = curr->next;
		curr->next = newFree;
	}

	/**
	 * Do coalesce right
	 */
	if((struct FreeHeader *)((char *)newFree + newFree->length + (int)sizeof(struct FreeHeader)) == newFree->next){

	 	newFree->length += newFree->next->length + (int)sizeof(struct FreeHeader);
	 	newFree->next = newFree->next->next;
	 }

	 struct FreeHeader * prev;
	 prev = NEXT_FIT_FREE_HEADER;

	/**
 	 * Do coalesce left
 	 */
	 while(prev != NULL && prev->next < newFree){
	 	prev = prev->next;
	 }

	if(prev != NULL && (struct FreeHeader *)((char *)prev + prev->length + (int)sizeof(struct FreeHeader)) == newFree){
		
		prev->length += newFree->length + (int)sizeof(struct FreeHeader);
		prev->next = newFree->next;
	}

	printf("releasing free\n");
	pthread_mutex_unlock(&nextfitlock);	
	printf("releasing free\n");

	return 0;
}
