#include <stdlib.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

/*
The base linked list code was taken from this website

http://www.learn-c.org/en/Linked_lists
*/

typedef struct node {
    int pid;
    char* command;
    struct node * next;
} node_t;

void print_list(node_t * head) {
    node_t * current = head;
    current = current->next;
    int count = 0;

    while (current != NULL) {
        printf("PID:%d  %s \n", current->pid, current->command);
        current = current->next;
	count++;
    }
    printf("Total background jobs: %d\n", count);
}



