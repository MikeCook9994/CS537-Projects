#include "mem_algo.h"

/**
 * Init User Program Heap Memory,
 * Using Slab and Next Fit Algo
 * 				
 * @param  sizeOfRegion [size of memory space]
 * @param  slabSize     [size of each slab]
 * @return              [base address of memory region]
 */
void * Mem_Init(int sizeOfRegion, int slabSize) {

	static int called = 0;
	
	if(called != 0) {
		return NULL;
	}

	if(sizeOfRegion <= 0 || slabSize <= 0){
		return NULL;
	}

	int numSlabs;

	SLAB_REGION_SIZE = sizeOfRegion / 4;
	NEXT_FIT_REGION_SIZE = (sizeOfRegion / 4) * 3;
	SLAB_SIZE = slabSize;
	numSlabs = SLAB_REGION_SIZE / slabSize;

	if(sizeOfRegion % slabSize != 0) {
		return NULL;
	}

	int pagesize;
	int padsize;
	pagesize = getpagesize();
	padsize = sizeOfRegion % pagesize;
	padsize = (pagesize - padsize) % pagesize;
	REGION_SIZE = sizeOfRegion + padsize;

	REGION_HEAD = mmap(NULL, REGION_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(REGION_HEAD == MAP_FAILED) {
		return NULL;
	}
	called++;

	/**
	 * Base Address Of Slab Memory Region
	 */
	SLAB_HEAD = REGION_HEAD;

	/**
	 * Base Address Of Next Fit Memory Region
	 */
	NEXT_FIT_HEAD = (REGION_HEAD + (SLAB_REGION_SIZE));


	/**
	 * Set Up Slab Space Free Node
	 */
	TOP_OF_STACK = (Node *) SLAB_HEAD;
	
	/**
	 * Set Up Next Fit Space, Entire Space Free At Init
	 */
	NEXT_FIT_FREE_HEADER = (struct FreeHeader *) NEXT_FIT_HEAD;
	NEXT_FIT_FREE_HEADER->next = NULL;
	NEXT_FIT_FREE_HEADER->length = NEXT_FIT_REGION_SIZE - (int)sizeof(struct FreeHeader);


	/**
	 * Partition Slab Memory Region Into Equal Sized Pieces
	 */
	void * tmp = TOP_OF_STACK;
	int i;
	for(i = 0; i < numSlabs; i++) {
		Node temp_node;
		if((tmp + i * slabSize) + slabSize >= NEXT_FIT_HEAD){
			temp_node.next = NULL;
		}
		else{
			temp_node.next = (tmp + i * slabSize) + slabSize;
		}	
		*((Node *)(tmp + i * slabSize)) = temp_node; 
	}

	/**
 	 * Initializes locks
 	 */ 
	assert(pthread_mutex_init(&slablock, NULL) == 0);
	assert(pthread_mutex_init(&nextfitlock, NULL) == 0);

	return REGION_HEAD;
}

void * Mem_Alloc(int size) {

	if(size <= 0) 
		return NULL;

	return Alloc_Policies[(size == SLAB_SIZE) ? SLAB_POLICY : NEXT_FIT_POLICY](size); 
}

int Mem_Free(void *ptr) {
	return Free_Policies[ptr < NEXT_FIT_HEAD ? SLAB_POLICY : NEXT_FIT_POLICY](ptr);
}

void Mem_Dump() {

	printf("-------------------------------------------------------------------\n");
	printf("DUMPING MEMORY INFO\n\n");
	printf("REGION START: %p\t|\tREGION SIZE: %d\t|\tREGION END: %p \n", REGION_HEAD, REGION_SIZE, (REGION_HEAD + REGION_SIZE));
	printf("TOP OF STACK: %p\t|\tSLAB SIZE: %d\t\t|\tSLAB END: %p\n", SLAB_HEAD, SLAB_REGION_SIZE, (REGION_HEAD + SLAB_REGION_SIZE));
	printf("NF_BASE_ADD: %p\t|\tNEXT FIT SIZE: %d\t|\tNEXT FIT END: %p\n\n", NEXT_FIT_HEAD, NEXT_FIT_REGION_SIZE, (NEXT_FIT_FREE_HEADER + NEXT_FIT_REGION_SIZE));

	printf("SLAB ALLOCATER DATA\n");

	int slabsAlloted = NUM_SLABS_ALLOCATED * SLAB_SIZE;
	printf("TOTAL ALLOCATED SLAB MEMORY: %d\n", slabsAlloted);
	
	int freeSlabs = 0;
	Node * tmp = TOP_OF_STACK;
	while(tmp != NULL) {
		freeSlabs+=SLAB_SIZE;
		tmp = tmp->next;
	}

	printf("TOTAL FREE SLAB MEMORY: %d\t|\tTOTAL SLAB MEMORY: %d\n\n", freeSlabs, freeSlabs + slabsAlloted);

	printf("NEXT FIT ALLOCATER DATA\n");


	int memLeft = 0;
	struct FreeHeader * freeTmp;
	freeTmp = NEXT_FIT_FREE_HEADER;
	while(freeTmp != NULL) {

		printf("ADDRESS: %p\t|\tSIZE: %d\t|\tNEXT ADDRESS: %p\n", freeTmp, freeTmp->length, freeTmp->next);
		memLeft += freeTmp->length + (int)(sizeof(struct FreeHeader));
		freeTmp = freeTmp->next;
	}
	printf("TOTAL FREE MEMORY LEFT: %d\n", memLeft);

	printf("-------------------------------------------------------------------\n");
	
}
