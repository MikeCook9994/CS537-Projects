#include "cmstructs.h"

#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* globals */
char * cli;
size_t maxcli;

/* clprogs.c */
int	checkCLProgs(char * token);
int executeCLProg(char * token, int prognum);

/* parsecl.c */
typedef struct LinkedListNode {
	char * token;
	struct LinkedListNode * next;
} Node;

int parsecli(char * firstarg, char * cli);
int validateTokens(Node * head);
int isSpecialChar(char * token);
CmNode * buildCommands(Node * tokenList);

/* commandhandler.c*/
int executeProcesses(CmNode * command);
char ** buildArgList(CmNode * command);

#endif
