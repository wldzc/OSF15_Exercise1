#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "command.h"

#define MAX_CMD_COUNT 50
#define MAX_CMD_LEN 25


	//TODO FUNCTION COMMENT
/*
 * Purpose:  
 * 		Error check the input and if there is no error, parse the user input and then store in an array.
 * Inputs:
 * 	 	input: the user input string
 * 		cmd: pointer that stores all the commands 
 * Return:
 * 		True if no error happens during the process
 * 		Otherwise, return false 
*/ 
bool parse_user_input (const char* input, Commands_t** cmd) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS
	//Incoming parameters error check
	if (input == NULL){
		printf("The input is empty!\n");
		return false;
	}
	else if (cmd == NULL){
		printf("No command stored in the array!\n");
		return false;
	}
	
	char *string = strdup(input);
	
	*cmd = calloc (1,sizeof(Commands_t));
	(*cmd)->cmds = calloc(MAX_CMD_COUNT,sizeof(char*));

	unsigned int i = 0;
	char *token;
	token = strtok(string, " \n");
	for (; token != NULL && i < MAX_CMD_COUNT; ++i) {
		(*cmd)->cmds[i] = calloc(MAX_CMD_LEN,sizeof(char));
		if (!(*cmd)->cmds[i]) {
			perror("Allocation Error\n");
			return false;
		}	
		strncpy((*cmd)->cmds[i],token, strlen(token) + 1);
		(*cmd)->num_cmds++;
		token = strtok(NULL, " \n");
	}
	free(string);
	return true;
}

	//TODO FUNCTION COMMENT
/*
 * Purpose:
 *  	Deallocate the memory stored in the command for future use
 * Inputs: 
 * 		cmd: pointer that stores all the commands
 * Return:
 * 	 	If cmd is not cleared then print the error message
 * 	 	Return nothing
*/
void destroy_commands(Commands_t** cmd) {

	//ERROR CHECK INCOMING PARAMETERS
	if (!cmd){
		printf("No commands stored!\n");
		return;
	}
	
	for (int i = 0; i < (*cmd)->num_cmds; ++i) {
		free((*cmd)->cmds[i]);
	}
	free((*cmd)->cmds);
	free((*cmd));
	*cmd = NULL;
}

