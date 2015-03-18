#include "mymem.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct linkedListNode {
	int data;
	struct linkedListNode * next;
} Node;

int heapSize = 4096;
int slabSize = 16;

Node * slabTest() {

	Node * head = NULL;
	Node * tmp;

	printf("SLAB TEST\n");

	int i;
	for(i = 0; i < ((heapSize / 4) / slabSize); i++) {
		tmp = (Node *)Mem_Alloc(slabSize);
		assert(tmp != NULL);
		tmp->data = i;
		tmp->next = head;
		head = tmp;
	}

	return head;
}

void freeSlabTest(Node * head) {

	Node * tmp;
	tmp = head;

	printf("SLAB FREE TEST\n");

	int i;
	for(i = 0; i < ((heapSize / 4) / slabSize); i++) {
		head = tmp->next;
		Mem_Free(tmp);
		tmp = head;
	}

}

void nextFitTest(void * pointers[]){

	srand(time(NULL));

	printf("NEXT FIT TEST\n");

	int i;
	for(i = 0; i < 400; i++) {
		pointers[i] = Mem_Alloc(rand() % 500);
		if(pointers[i] == NULL)
			break;
	}

}

void freeNextFitTest(void ** pointers) {

	printf("NEXT FIT FREE TEST\n");

	int i;
	void * tmp;
	for(i = 0; i < 400; i++) {
		tmp = pointers[i];
		if(tmp == NULL)
			break;
		Mem_Free(tmp);
	}	
}

int main(int charc, char *argv[]) {

	if(charc == 3){
		heapSize = atoi(argv[1]);
		slabSize = atoi(argv[2]);
	}

 	assert(Mem_Init(heapSize, slabSize) != NULL);

	Node * head;

	printf("MEM INIT TEST\n");
	Mem_Dump();

	head = slabTest();
	Mem_Dump();

	freeSlabTest(head);
	Mem_Dump();

	void * pointers[400];

	nextFitTest(pointers);
	Mem_Dump();

	freeNextFitTest(pointers);
	Mem_Dump();

	return 0;
}
