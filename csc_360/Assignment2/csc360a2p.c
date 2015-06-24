#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>

struct Train{
	int priority;
	char* direction;
	int loadtime;
	int crosstime;
	pthread_t thread;
	struct Train * next;
	int id;
};

/*
Declare Train stations for queues

Declare function prototypes

Declare pthread variables
*/
struct Train * higheasttrain;
struct Train * loweasttrain;
struct Train * highwesttrain;
struct Train * lowwesttrain;
char dir = 'e';
char* direc = &dir;

void load_train(struct Train* tr);
void add_train(struct Train * newtrain);
void getinput(int argc, struct Train* trainp, char *argv[]);
void choochoo(struct Train* curtr);
void *create_thread(void *arg);
void print_queue(struct Train* tr);

pthread_barrier_t barrier;
pthread_mutex_t maintrack;
pthread_mutex_t highwest;
pthread_mutex_t higheast;
pthread_mutex_t lowwest;
pthread_mutex_t loweast;
pthread_cond_t chighwest;
pthread_cond_t chigheast;
pthread_cond_t clowwest;
pthread_cond_t cloweast;
pthread_cond_t cmaintrack;

/*
Add a train to it's train station linked list
*/
void add_train(struct Train * newtrain){
	struct Train * temp;
	struct Train * temp2;
	if(newtrain->priority == 1){
		if(*newtrain->direction == 'e'){


			//while(pthread_mutex_lock(&higheast) != 0);
			temp = higheasttrain;
			temp2 = higheasttrain;		
			if(temp == NULL){
				temp = newtrain;
				return;
			}	
			while(temp->loadtime < newtrain->loadtime && temp->next != NULL){
				temp2 = temp;		
				temp = temp->next;
			} //while
			if(temp->loadtime == newtrain->loadtime){
				if(temp->id < newtrain->id){
					if(temp->next == NULL){
						temp->next = newtrain;
					} else {
						temp2 = temp->next;
						temp->next = newtrain;
						newtrain->next = temp2;
					} //if
				} else {
					temp2->next = newtrain;
					newtrain->next = temp;
				} 
			} else {
				temp2->next = newtrain;
				newtrain->next = temp;
			} //equal loadtime
			//pthread_cond_signal(&chigheast);
			//pthread_mutex_unlock(&higheast);


		} else {
			//while(pthread_mutex_lock(&higheast) != 0);
			temp = highwesttrain;
			temp2 = highwesttrain;	
			if(temp == NULL){
				temp = newtrain;
				return;
			}	
			while(temp->loadtime < newtrain->loadtime && temp->next != NULL){
				temp2 = temp;		
				temp = temp->next;
			} //while
			if(temp->loadtime == newtrain->loadtime){
				if(temp->id < newtrain->id){
					if(temp->next == NULL){
						temp->next = newtrain;
					} else {
						temp2 = temp->next;
						temp->next = newtrain;
						newtrain->next = temp2;
					} //if
				} else {
					temp2->next = newtrain;
					newtrain->next = temp;
				} 
			} else {
				temp2->next = newtrain;
				newtrain->next = temp;
			} //equal loadtime
			//pthread_cond_signal(&chigheast);
			//pthread_mutex_unlock(&higheast);
		}
	}else if(newtrain->priority == 0){
		if(*newtrain->direction == 'w'){
			//while(pthread_mutex_lock(&higheast) != 0);
			temp = lowwesttrain;
			temp2 = lowwesttrain;	
			if(temp == NULL){
				temp = newtrain;
				return;
			}	
			while(temp->loadtime < newtrain->loadtime && temp->next != NULL){
				temp2 = temp;		
				temp = temp->next;
			} //while
			if(temp->loadtime == newtrain->loadtime){
				if(temp->id < newtrain->id){
					if(temp->next == NULL){
						temp->next = newtrain;
					} else {
						temp2 = temp->next;
						temp->next = newtrain;
						newtrain->next = temp2;
					} //if
				} else {
					temp2->next = newtrain;
					newtrain->next = temp;
				} 
			} else {
				temp2->next = newtrain;
				newtrain->next = temp;
			} //equal loadtime
			//pthread_cond_signal(&chigheast);
			//pthread_mutex_unlock(&higheast);	
		} else {
			//while(pthread_mutex_lock(&higheast) != 0);
			temp = loweasttrain;
			temp2 = loweasttrain;	
			if(temp == NULL){
				temp = newtrain;
				return;
			}	
			while(temp->loadtime < newtrain->loadtime && temp->next != NULL){
				temp2 = temp;		
				temp = temp->next;
			} //while
			if(temp->loadtime == newtrain->loadtime){
				if(temp->id < newtrain->id){
					if(temp->next == NULL){
						temp->next = newtrain;
					} else {
						temp2 = temp->next;
						temp->next = newtrain;
						newtrain->next = temp2;
					} //if
				} else {
					temp2->next = newtrain;
					newtrain->next = temp;
				} 
			} else {
				temp2->next = newtrain;
				newtrain->next = temp;
			} //equal loadtime
			//pthread_cond_signal(&chigheast);
			//pthread_mutex_unlock(&higheast);
		}
	}
} //function

/*
Parse the input and create each train with it's details
*/
void getinput(int argc, struct Train* trainp, char *argv[]){
	FILE *file = fopen(argv[1], "r");
	int numtrains = atoi(argv[2]);
	
	if(file == NULL){
		printf("\n Error opening file");
	} //checkfile

	int x;
	
	for(x = 0; x < numtrains; x++){
		char str1;
		int str2, str3;
		fscanf(file, "%c:%d,%d\n", &str1, &str2, &str3);
		trainp[x].id = x;
		if(isupper(str1)){
			trainp[x].priority = 1;
		} else {
			trainp[x].priority = 0;
		} 
		if(str1 == 'E' || str1 == 'e'){
			trainp[x].direction = "e";
		} else {
			trainp[x].direction = "w";
		}
		trainp[x].loadtime = str2;
		trainp[x].crosstime = str3;
	} //for
}

/*
Print when the train starts crossing, and ends crossing
*/
void choochoo(struct Train* curtr){
	if(*curtr->direction == 'e'){
		printf("Train %d has started on the main track heading East\n", curtr->id);
	} else {
		printf("Train %d has started on the main track heading West\n", curtr->id);
	}
	usleep(curtr->crosstime*100000);
	if(*curtr->direction == 'e'){
		printf("Train %d has crossed the main track heading East\n", curtr->id);
	} else {
		printf("Train %d has crossed the main track heading West\n", curtr->id);
	}
	direc = curtr->direction;
	return;
	//curtr = curtr->next->next;
	//broadcast
	//unlock mutex
}

/*
The main functionality of each thread, includes:
load_train()
add_train()
choochoo()
*/
void *create_thread(void *arg){
	struct Train* tr;
	tr = (struct Train*) arg;
	pthread_barrier_wait(&barrier);
	load_train(tr);
	add_train(tr);
	//while(pthread_mutex_lock(&maintrack) != 0);
	choochoo(tr);
	print_queue(higheasttrain);
	return NULL;
}

/*
Load the train at the station, print when complete
*/
void load_train(struct Train* tr){
	usleep(tr->loadtime*100000);
	if(*tr->direction == 'e'){
		printf("Train %d is ready to go East\n", tr->id);
	} else {
		printf("Train %d is ready to go West\n", tr->id);
	}
}

void print_queue(struct Train* test){
	printf("<<%d>>", test->id);
	while(test != NULL){
		printf("Train %d ->", test->id);
		test = test->next;
	}
}

/*void scheduler(int numtrains){
	int donetrains = 0;
	while(donetrains < numtrains){
		while(pthread_mutex_lock(&higheast) != 0);
		printf("locked higheast\n");
		if(higheasttrain != NULL){
			pthread_cond_wait(&chigheast, &higheast);
		}
	
		while(pthread_mutex_lock(&highwest) != 0);
		printf("locked highwest\n");
	
		while(pthread_mutex_lock(&loweast) != 0);
		printf("locked loweast\n");

		while(pthread_mutex_lock(&lowwest) != 0);
		printf("locked lowwest\n");
	
		while(pthread_mutex_lock(&maintrack) != 0);
		printf("locked maintrack\n");

		if(higheasttrain != NULL || highwesttrain != NULL || loweasttrain != NULL ||     lowwesttrain != NULL){

			if(highwesttrain != NULL || higheasttrain != NULL){
				if(higheasttrain == NULL){
					choochoo(highwesttrain);
				} else if(highwesttrain == NULL){
					choochoo(higheasttrain);
				}else if(higheasttrain->loadtime > highwesttrain->loadtime){
					choochoo(highwesttrain);
				}else if(higheasttrain->loadtime < highwesttrain->loadtime){
					choochoo(higheasttrain);
				}else if(higheasttrain->loadtime == highwesttrain->loadtime){
					if(*direc == 'e'){
						choochoo(highwesttrain);	
					}else if(*direc == 'w'){
						choochoo(higheasttrain);
					}
				}

			} else if (highwesttrain == NULL && higheasttrain == NULL){
				if(loweasttrain == NULL){
					choochoo(lowwesttrain);
				} else if(lowwesttrain == NULL){
					choochoo(loweasttrain);
				}else if(loweasttrain->loadtime > lowwesttrain->loadtime){
					choochoo(lowwesttrain);
				}else if(loweasttrain->loadtime < lowwesttrain->loadtime){
					choochoo(higheasttrain);
				}else if(loweasttrain->loadtime == lowwesttrain->loadtime){
					if(*direc == 'e'){
						choochoo(lowwesttrain);	
					}else if(*direc == 'w'){
						choochoo(loweasttrain);
					}
				}
			}
		
		} //main if
	} //main while
}*/

/*
MAIN

Calls getinput and spawns all the child threads
Waits for child threads to finish before finishing
*/
int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Please try again sir, not enough inputs\n");
		exit(1);
	} //testing input
	int numtrains = atoi(argv[2]);
	struct Train trains[numtrains];
	struct Train* trainp = trains;
	getinput(argc, trainp, argv);

	if (pthread_barrier_init (&barrier, NULL, numtrains) != 0){
		printf("\n barrier init failed\n");
		return 1;
	}

	if (pthread_mutex_init(&maintrack, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}
	if (pthread_mutex_init(&highwest, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}
	if (pthread_mutex_init(&higheast, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}
	if (pthread_mutex_init(&lowwest, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}
	if (pthread_mutex_init(&loweast, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}
	if (pthread_cond_init(&cmaintrack, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}
	if (pthread_cond_init(&chighwest, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}
	if (pthread_cond_init(&chigheast, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}
	if (pthread_cond_init(&clowwest, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}
	if (pthread_cond_init(&cloweast, NULL) != 0){
       		printf("\n mutex init failed\n");
        	return 1;
    	}

	int x = 0;
	for(x = 0; x < numtrains; x++){
		if(pthread_create(&trains[x].thread, NULL, create_thread, &trains[x])){
			fprintf(stderr, "Thread creation failed\n");
			return 1;
		}
	}

	//scheduler(numtrains);
	
	int a = 0;
	for(a = 0; a < numtrains; a++){
		if(pthread_join(trains[a].thread, NULL)){
			fprintf(stderr, "Thread join failed\n");
		}
	}	

	pthread_mutex_destroy(&maintrack);
	pthread_mutex_destroy(&lowwest);
	pthread_mutex_destroy(&loweast);
	pthread_mutex_destroy(&highwest);
	pthread_mutex_destroy(&higheast);
	return 0;
}




