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
void add_train(struct Train * head, struct Train * newtrain);
void getinput(int argc, struct Train* trainp, char *argv[]);
void choochoo(struct Train* curtr);
void *create_thread(void *arg);

pthread_barrier_t barrier;
pthread_mutex_t maintrack;

/*
Add a train to it's train station linked list
*/
void add_train(struct Train * head, struct Train * newtrain){
	struct Train * temp = head;	
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = newtrain;
}

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
	if(tr->priority == '1'){
		if(*tr->direction == 'e'){
			add_train(higheasttrain, tr);
		} else {
			add_train(highwesttrain, tr);
		}
	}else if(tr->priority == '0'){
		if(*tr->direction == 'w'){
			add_train(lowwesttrain, tr);
		} else {
			add_train(loweasttrain, tr);
		}
	}
	//lock before choochoo
	//while true
		//wait for a signal
		//if tr is in the front of any queue	
			//if tr is highwest
				//if higheast has train
					//if higheast loadingtime > tr loadingtime
						//lock with higheast pointer
						//choochoo
						//exit while
			//if tr is lowwest
				//if highwest or higheast has a train
					//quit
				//else
					//if loweast loadingtime > tr loadingtime
						//lock with lowwest pointer
						//choochoo
						//exit while
			//if tr is higheast
				//if highwest has train
					//if highwest loadingtime > tr loadingtime
						//lock with highwest pointer
						//choochoo
						//exit while
			//if tr is loweast
				//if highwest or higheast has a train
					//quit
				//else
					//if lowwest loadingtime > tr loadingtime
						//lock with loweast pointer
						//choochoo
						//exit while
	choochoo(tr);
	/*wait'
	remove;*/
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

	pthread_mutex_init (&maintrack, NULL);
	pthread_barrier_init (&barrier, NULL, numtrains);

	int x = 0;
	for(x = 0; x < numtrains; x++){
		if(pthread_create(&trains[x].thread, NULL, create_thread, &trains[x])){
			fprintf(stderr, "Thread creation failed\n");
			return 1;
		}
	}
	
	int a = 0;
	for(a = 0; a < numtrains; a++){
		if(pthread_join(trains[a].thread, NULL)){
			fprintf(stderr, "Thread join failed\n");
		}
	}
	return 0;
}






