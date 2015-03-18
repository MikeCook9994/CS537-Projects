#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "p1a2.h"

/* This is an array of 10 POINTERS to KeyboardElements */
KEPtr keypad[10];
char * letters[10] = { "", "ABC", "DEF", "GHI", "JKL", "MNO", "PQRS", "TUV", "WXYZ", "" };

void setupKeypad() {
	int i;
	for(i = 1; i <= 10; i++) {
		KEPtr key;
		key = malloc(sizeof(KEPtr));
		key->counter = 0;
		key->letters = letters[i - 1];
		keypad[i] = key;
	}	
}

void PrintFunction(char c) {
	printf("%c", c);
}

int parseInput(int keyIndex, int letterIndex) {	
	if(keyIndex >= 2 && keyIndex <= 9) {
		if((letterIndex  >= 0 && letterIndex <= 2) || ((keyIndex == 7 || keyIndex == 9) && letterIndex == 3)) {
			PrintFunction(keypad[keyIndex] -> letters[letterIndex]);
			(keypad[keyIndex] -> counter)++;
		}
		else
			return 0;
	}
	else
		return 0;
	return 1;
}

void printResults() {	
	int i;
	for(i = 2; i < 10; i++) {
		printf("%d\t%d\n", i, keypad[i]->counter);	
	}
}

int main(int argc, char * argv[]) {

	int keyIndex, letterIndex;

	if (argc <= 1) {
		printf("Usage: p1a2 string1 [stringN]\n");
		exit(1);
	}

	setupKeypad();
	
	int i,j;
	for(i = 1; i < argc; i++) {
		for(j = 0; j < strlen(argv[i]); j+=2) {
			if(((argv[i] + j)   == NULL) || ((argv[i] + (j + 1)) == NULL)) {
				PrintFunction('-');
				break;
			}
			else {
				keyIndex = (argv[i][j]) - '0';
				letterIndex = (argv[i][j + 1]) - '0' - 1;
				if(parseInput(keyIndex, letterIndex) == 0) {
					PrintFunction('-');	
					break;
				}
			}
			break;
		}
		PrintFunction('\n');
	}	

	printResults();

	for(i = 0; i < 10; i++) {
		free(keypad[i]);
	}

	return 0;
}
