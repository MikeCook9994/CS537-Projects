#include "cmstructs.h"
#include "defs.h"

int parsecli(char * firstarg, char * cli) {

	char * currToken;

	Node * head;
	head = malloc(sizeof(Node));

	Node * curr;
	curr = head;
	
	head->token = firstarg;
	head->next = NULL;

	/* constructs the linked list of tokens */
	while(1) {
		currToken = strtok(NULL, " \t");
		if(currToken == NULL)
			break;
		Node * tmp;
		tmp = malloc(sizeof(Node));
		
		tmp->token = currToken;
		tmp->next = NULL;
		curr->next = tmp;
		curr = tmp;
	}

	if(validateTokens(head) == -1)
		return -1;

	/* builds the commands to be executed and then passes them off to be 
 	 * executed. 
 	 */
	if(executeProcesses(buildCommands(head)) == -1)
		return -1;

	return 0;
}

CmNode * buildCommands(Node * tokenList) {

	/* creates another reference to the token list to maintain the current token */
	Node * currTokenNode;
	currTokenNode = tokenList;

	/* these variables will maintain the value of the token and it's type */
	char * currToken;
	int currTokenType;
	currToken = currTokenNode->token;

	/* creates the head of the CmNode list that will contain references to the
 	* commands. Ititializes it and sets its next field to NULL.
 	*/
	CmNode * head;
	head = malloc(sizeof(CmNode));
	head->next = NULL;

	/* A CmNode reference that keeps track of the current CmNode. It is assigned
 	* to the head node. 
 	*/
	CmNode * curr;
	curr = head;	
	
	/* Initializes a command struct that will contain the IO source/destination,
 	* and the argument list. It is set as the CmNode's command.
 	*/
	Command * firstCommand;
	firstCommand = malloc(sizeof(Command));
	head->command = firstCommand;
	
	/* Initializes an Argument struct which will be a linked list of all of a
 	* commands argument. It's argument is set equal to the current token.
 	* It becomes the  begining of the argument list of the first command.
 	*/	
	ArgNode * firstArg;
	firstArg = malloc(sizeof(ArgNode));
	firstArg->argument = currToken; 
	firstCommand->arguments = firstArg;
	
	ArgNode * currArg;
	currArg = firstArg;

	/* increments the current token Node */
	currTokenNode = currTokenNode->next;

	/* enters a while loop to handle the rest of the tokens */
	while(currTokenNode != NULL) {
	
		/* Checks the type of the current token*/
		currToken = currTokenNode->token;
		currTokenType = isSpecialChar(currToken);		

		/* If the current token is not a special char, it is added to the
 		* current comamnds argument list.
 		*/
		if(currTokenType == 0) {
			ArgNode * tmpArg;
			tmpArg = malloc(sizeof(ArgNode));
			tmpArg->argument = currToken;
			currArg->next = tmpArg;
			currArg = currArg->next;	
		}
		else if(currTokenType == 1) {

		}
		else if(currTokenType == 2) {

		}
		else if(currTokenType == 3) {

		}
		else {

		}
		/* incriments the current node*/
		currTokenNode = currTokenNode->next;
	}

	return head;
}

int validateTokens(Node * head) {

	/* counters for the maxinum  permitted number of any one special char */
	int teeCount, pipeCount, redirectCount, redirectAppendCount;
	teeCount = 1;
	redirectCount = 1;
	redirectAppendCount = 1;
	pipeCount = 2;

	/* The pointer to the current element in the list */
	Node * curr;
	curr = head;

	/* Maintains the type of the next character */
	int charType;
	int nextCharType;

	/* if the first token is a special char */
	if(isSpecialChar(curr->token) != 0)
		return -1;	

	/* Checks the linked list of token for validity */
	while(curr != NULL) {

		/* grabs the type of the current token */
		charType = isSpecialChar(curr->token);
		
		/* if the next token is not NULL, it's type is grabbed as well */
		if(curr->next != NULL)
			nextCharType = isSpecialChar(curr->next->token);

	 	/* if the current token and the one following it are both special operations
 		 * an error is returned 
 		 */
		if((charType != 0) && (nextCharType != 0))
			return -1;

		/* handles special operator syntax for the whole command */
		if(charType != 0) {	

			/* if the token is a | it verifies that no more than 2 pipes or one tee has
 			 * already been read. If so, an error is returned. If not, the counts are
       * decremented. 
       */
			if(charType == 1) {
				if((pipeCount <= 0) && (teeCount <= 0))
					return -1;
				else {
					teeCount--;
					pipeCount--;
				}
			}	
		
			else if(charType == 2) {
				if((pipeCount <= 0) || (teeCount <= 0))
					return -1;
				else {
					pipeCount -= 2;
					teeCount--;
				}
			}

			/* handles > and >> operators by making sure not more than one instance
 			 * of either has appeared in the input. If so an error is returned. If it
 			 * is the first instance, the program verifies the next token is not NULL 
 			 */
			else {
				if((redirectCount > 0) && (redirectAppendCount > 0)) {
					redirectCount--;
					redirectAppendCount--;
					if(curr->next == NULL) 
						return -1;
				}
			
				else return -1;
			}
		}

		/* increments to the next token in the list. */
		curr = curr->next;
	}

	return 0;
}

/* determines which, if any, special character a token is. */
int isSpecialChar(char * token) {
	if(strcmp(token, "|") == 0) return 1;
	else if(strcmp(token, "%") == 0) return 2;
	else if(strcmp(token, ">") == 0) return 3;
	else if(strcmp(token, ">>") == 0) return 4;
	else return 0;
}
