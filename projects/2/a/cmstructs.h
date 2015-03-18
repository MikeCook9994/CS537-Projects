/* These structs will come together to define the structure for processing
 * commands. At the highest level, there will be a list of CommandNodes
 * (CmNode) which will each represent a different command provided to the 
 * command line seperated by a special character of sorts (%, |, >, >>). 
 * Each CmNode contains a pointer to the next command, as well as a pointer
 * to the command itself.
 *
 * Each command is wrapped in a CommandStruct (Command). Each Command Struct
 * contains all the data necessary for carrying out a command as provided to
 * the command line. It includes a field for input and output types defined
 * by an enumerated value. 
 *
 * These enumerated values are:
 * 	STDIN - Input from the command line
 * 	STDOUT - Output printed to the command line
 * 	REDIRECT - 	Denotes that input will be sent somewhere other than expected.
 * 							This generally denotes another file that will either be read
 * 							from or written to.
 *
 * The command type also has a field to a pointer of an ArumentNode struct.
 *
 * Each ArgumentNode (ArgNode), contains a pointer to a pointer of chars, which
 * contains the actual content of the command line argument. There also exists
 * a field for the next argument in the list.
 */ 

#ifndef CMSTRUCTS_H_INCLUDED
#define	CMSTRUCTS_H_INCLUDED

typedef enum { STDIN, STDOUT, PIPE, TEE, REDIRECT, REDIRECTAPPEND } IOType;

typedef struct ArgumentNode {

	char * argument;
	struct ArgumentNode * next;

} ArgNode;

typedef struct CommandStruct {
	
	IOType input;
	char * inputName;
	IOType output;
	char * outputName;
	ArgNode * arguments; 

} Command;

typedef struct CommandNode {

	Command * command;
	struct CommandNode * next;

} CmNode;

#endif
