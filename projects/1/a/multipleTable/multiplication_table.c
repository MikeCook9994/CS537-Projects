#include <stdio.h>
#include <stdlib.h>

void usageAndQuit() {
		printf("Usage: multable [tableRange]\n[tableRange]: An integer value between 1 and 10 (inclusive)\n");
		exit(0);
}

void fillTable(int * table, int tableRange, int tableArea) {
	
	int i;
	for(i = 0; i < tableArea; i++) {
		*(table + i) = ((i % tableRange + 1) * (i / tableRange + 1));
	}

}

void printTable(int * table, int tableRange, int tableArea) {
	
	int i;
	for(i = 0; i < tableArea; i++) {
		printf("%d\t", *(table + i));	
		if(i % tableRange == (tableRange - 1))
			printf("\t\n");
	} 	

}

int main(int argc, char * argv[]) {

	int * table;
	int tableRange;

	if(argc == 1)
		tableRange = 10;
	else {
		tableRange = atoi(argv[1]);
		if(tableRange < 1 || tableRange > 10)
			usageAndQuit();
	}	

	table = (int *) malloc(sizeof(int) * (tableRange * tableRange));
	if(table == NULL)
		printf("Memory allocation for table failed.");
	
	fillTable(table, tableRange, (tableRange * tableRange));  

	printTable(table, tableRange, (tableRange * tableRange));

	free(table);

	return 0;	

}
