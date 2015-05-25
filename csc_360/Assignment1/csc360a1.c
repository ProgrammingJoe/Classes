#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "csc360list.h"


/* Execute
Forks the process and runs executable
If the child process fails it exits the process
The parent process adds the new command to the linked list for bglist
*/
void execute(char** commands, int bg, node_t** head, char* line){
	int status;
	pid_t pid = fork();
	if(pid < 0){ //child process unsuccessful
		printf("Error, child failed, lul");
		
	} else if(pid == 0){ //this is child process
		int error = execvp(commands[0], commands);
		if(error == -1){			
			exit(1);
		}
	} else if(bg == 1){ //this is main process
		node_t * temp = *head;
		while(temp->next != NULL){
			temp = temp->next;
		}
		node_t * processes = malloc(sizeof(node_t));
		processes->pid = pid;
		processes->command = line;
		temp->next = processes;
		processes->next = NULL;
	} else if(bg == 0){ 
		wait(&status);
	}
} //execute


/* change_dir
Changes directory to path specified after cd
*/
void change_dir(char** commands){
	if(strcmp(commands[1], "~") == 0){
		chdir(getenv("HOME"));
	} else {
		chdir(commands[1]);
	}	
} //change_dir


/* parse
Parses the input into an array
*/
char* parse(char** commands, char* token){
	int a = 0;
		
	for(a = 0; a < (sizeof(commands)-1); a++){
		commands[a] = token;
		token = strtok(NULL, " ");
	}
	commands[(sizeof(commands))-1] = NULL;
	return *commands;
} //parse


/*
Takes input from the user
*/
char* get_input(int size, char current[]){
	char* directory = getcwd(current, size);
	printf("RWD: %s> ", directory);
	char* line = readline(NULL);
} //get_input


/*
Checks to see which processes are still running
If not the process is deleted
*/
void handler(node_t * head){
	node_t * temp = head;
	node_t * temp2 = head;
	node_t * temp3 = head;
	if(temp->next != NULL){
		temp = temp->next;
	}
	while(temp != NULL){
		pid_t childPID = temp->pid;
		int status;
		pid_t result = waitpid(childPID, &status, WNOHANG);
		if (result == 0) {
  			// Child still alive
			temp2 = temp2->next;
			temp = temp->next;
		} else if (result == -1) {
  			// Error {
			temp2 = temp2->next;
			temp = temp->next;
		} else {
			// The Child has been killed....I mean, child process
  			printf("Child with PID %d has been terminated\n\n", childPID);		
			temp2->next = temp->next;
			temp3 = temp;
			temp = temp->next;
			free(temp3);	
		}
	}
} //handler



int main(void){
	int size = 100;
	char current[size];
	char *line; 
	int exit = 0;	
	node_t * head = malloc(sizeof(node_t));

	while(!exit){
		handler(head);
		int count = 2;
		line = get_input(size, current);
		int length = strlen(line);
		char* increment = line;
		int bg = 0;
		int x = 0;
		for(x = 0; x < length; x++){
			if(isspace(increment[x])){
				count++;
				while(isspace(increment[x])){
					x++;
				}
			x--;
			}
		}

		char *cmd = malloc(sizeof(char) * strlen(line));
		strcpy(cmd, line);

		char* token = strtok(line, " ");
		if(token == NULL){		
			continue;
		}	
		
		/*
		If the first argument is bg, flag it and then stop it from being added 			to the command array
		*/
		if(strcmp(token, "bg") == 0){
			bg = 1;			
			count--;
			token = strtok(NULL, " ");			
		}

		char* commands[count];
		*commands = parse(commands, token);
	
		/*
		This looks at the arguments and decides what function to call
		*/
		if(strcmp(commands[0], "exit") == 0){
			exit = 1;
		} else if(strcmp(commands[0], "cd") == 0){
			change_dir(commands);	
		} else if(strcmp(commands[0], "bglist") == 0){				
			handler(head);		
			print_list(head);	
		} else {
			execute(commands, bg, &head, cmd);
			handler(head);	
		}	
		free(cmd);
		free(head);
	}//while

	

	printf("\nThank you for using my command prompt\n");
	return 0;
}//main
