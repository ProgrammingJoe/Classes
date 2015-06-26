#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <sys/sysinfo.h>

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
Displaying the time help was taken from:

//http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/

Pthread help was taken from linux man pages
*/

/*
Declare Train stations for linked list queues
Declare Last direction variable
Declare function prototypes
Declare pthread variables
*/
struct Train * higheasttrain = NULL;
struct Train * loweasttrain = NULL;
struct Train * highwesttrain = NULL;
struct Train * lowwesttrain = NULL;

struct timespec monotime;
char dir = 'e';
char* direc = &dir;

void load_train(struct Train* tr); 				//Loads trains for their loadtime
void add_train(struct Train * newtrain); 			//Locks mutex and adds train to station
void getinput(int argc, struct Train* trainp, char *argv[]); 	//Take stdin
void choochoo(struct Train* curtr); 				//Locks mutex and train crosses track
void *create_thread(void *arg); 				//Thread function
void print_queue(); 						//Prints a station queue
struct Train* poptrain(struct Train * head); 			//Pops a train off a station queue
struct timespec converttime(struct timespec);					//Convert difference between start and current time

pthread_barrier_t barrier;
pthread_mutex_t maintrack;
pthread_mutex_t highwest;
pthread_mutex_t higheast;
pthread_mutex_t lowwest;
pthread_mutex_t loweast;
pthread_cond_t cmaintrack;

/*
Add a train to it's train station linked list
Set temp and temp2 pointers to the correct train station
If the train station is empty add the train to the head of the station
*/
void add_train(struct Train * newtrain){
	struct Train * temp;
	struct Train * temp2;
	newtrain->next = NULL;

	if(newtrain->priority == 1){
		if(*newtrain->direction == 'e'){
			while(pthread_mutex_lock(&higheast) != 0);
			temp = higheasttrain;
			temp2 = higheasttrain;
			if(higheasttrain == NULL){
				higheasttrain = newtrain;
				pthread_cond_signal(&cmaintrack);
				pthread_mutex_unlock(&higheast);
				return;
			}
		}else{
			while(pthread_mutex_lock(&highwest) != 0);
			temp = highwesttrain;
			temp2 = highwesttrain;
			if(highwesttrain == NULL){
				highwesttrain = newtrain;
				pthread_cond_signal(&cmaintrack);
				pthread_mutex_unlock(&highwest);
				return;
			}
		}
	}else{
		if(*newtrain->direction == 'e'){
			while(pthread_mutex_lock(&loweast) != 0);
			temp = loweasttrain;
			temp2 = loweasttrain;
			if(loweasttrain == NULL){
				loweasttrain = newtrain;
				pthread_cond_signal(&cmaintrack);
				pthread_mutex_unlock(&loweast);
				return;
			}
		}else{
			while(pthread_mutex_lock(&lowwest) != 0);
			temp = lowwesttrain;
			temp2 = lowwesttrain;
			if(lowwesttrain == NULL){
				lowwesttrain = newtrain;
				pthread_cond_signal(&cmaintrack);
				pthread_mutex_unlock(&lowwest);
				return;
			}
		}
	}

	/*
	Add the new train to the correct spot in the station
	*/
	while(temp != NULL && temp->loadtime < newtrain->loadtime){
		temp2 = temp;		
		temp = temp->next;
	}
	while(temp != NULL && temp->id < newtrain->id && temp->loadtime == newtrain->loadtime){	
		temp2 = temp;
		temp = temp->next;
	}

	if(temp == NULL){
		temp2->next = newtrain;
	} else if(temp2 == temp){
		if(temp == highwesttrain){
			highwesttrain = newtrain;
			newtrain->next = temp;
		} else if(temp == higheasttrain){
			higheasttrain = newtrain;
			newtrain->next = temp;
		} else if(temp == lowwesttrain){
			lowwesttrain = newtrain;
			newtrain->next = temp;
		} else {
			loweasttrain = newtrain;
			newtrain->next = temp;
		}
	} else {
		temp2->next = newtrain;
		newtrain->next = temp;
	}

	/*
	Signal a train has been added to the dispatcher and unlock the mutex
	*/
	if(newtrain->priority == 1){
		if(*newtrain->direction == 'e'){
			pthread_cond_signal(&cmaintrack);
			pthread_mutex_unlock(&higheast);
		}else{
			pthread_cond_signal(&cmaintrack);
			pthread_mutex_unlock(&highwest);
		}
	}else{
		if(*newtrain->direction == 'e'){
			pthread_cond_signal(&cmaintrack);
			pthread_mutex_unlock(&loweast);
		}else{
			pthread_cond_signal(&cmaintrack);
			pthread_mutex_unlock(&lowwest);
		}
	}
} //Add Train


/*
Parse the input and create each train with their details
*/
void getinput(int argc, struct Train* trainp, char *argv[]){
	FILE *file = fopen(argv[1], "r");
	int numtrains = atoi(argv[2]);
	
	if(file == NULL){
		printf("\n Error opening file");
	} 

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
} //getinput


/*
Print when the train starts crossing, and ends crossing
Update the direction of the last train
*/
void choochoo(struct Train* curtr){
	struct timespec realtime;
	struct timespec time;
	if(*curtr->direction == 'e'){
		clock_gettime(CLOCK_MONOTONIC, &realtime);
		time = converttime(realtime);
		printf("%02d:%02d:%02d:%1ld Train %d is ON the main track going East\n", (((int)time.tv_sec/3600)%24), (((int)time.tv_sec/60)%60), (((int)time.tv_sec)%60), (time.tv_nsec/100000000), curtr->id);
	} else {
		clock_gettime(CLOCK_MONOTONIC, &realtime);
		time = converttime(realtime);
		printf("%02d:%02d:%02d:%1ld Train %d is ON the main track going West\n", (((int)time.tv_sec/3600)%24), (((int)time.tv_sec/60)%60), (((int)time.tv_sec)%60), (time.tv_nsec/100000000), curtr->id);
	}
	usleep(curtr->crosstime*100000);
	if(*curtr->direction == 'e'){;
		clock_gettime(CLOCK_MONOTONIC, &realtime);
		time = converttime(realtime);
		printf("%02d:%02d:%02d:%1ld Train %d is OFF the main track after going East\n", (((int)time.tv_sec/3600)%24), (((int)time.tv_sec/60)%60), (((int)time.tv_sec)%60), (time.tv_nsec/100000000), curtr->id);
	} else {
		clock_gettime(CLOCK_MONOTONIC, &realtime);
		time = converttime(realtime);
		printf("%02d:%02d:%02d:%1ld Train %d is OFF the main track after going West\n", (((int)time.tv_sec/3600)%24), (((int)time.tv_sec/60)%60), (((int)time.tv_sec)%60), (time.tv_nsec/100000000), curtr->id);
	}
	direc = curtr->direction;
	return;
} //choochoo


/*
Find the difference from start time to end time
*/
struct timespec converttime(struct timespec end){
	struct timespec temp;
	if ((end.tv_nsec-monotime.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-monotime.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-monotime.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-monotime.tv_sec;
		temp.tv_nsec = end.tv_nsec-monotime.tv_nsec;
	}
	return temp;
} //converttime


/*
Each thread starts in this function, tr is the train
*/
void *create_thread(void *arg){
	struct Train* tr;
	tr = (struct Train*) arg;
	pthread_barrier_wait(&barrier);
	load_train(tr);
	add_train(tr);
	return NULL;
} //create_thread


/*
Load the train at the station, print when complete
*/
void load_train(struct Train* tr){
	struct timespec realtime;
	struct timespec time;
	usleep(tr->loadtime*100000);
	if(*tr->direction == 'e'){
		clock_gettime(CLOCK_MONOTONIC, &realtime);
		time = converttime(realtime);
		printf("%02d:%02d:%02d:%1ld Train %d is ready to go East\n", (((int)time.tv_sec/3600)%24), (((int)time.tv_sec/60)%60), (((int)time.tv_sec)%60), (time.tv_nsec/100000000), tr->id);
	} else {
		clock_gettime(CLOCK_MONOTONIC, &realtime);
		time = converttime(realtime);
		printf("%02d:%02d:%02d:%1ld Train %d is ready to go West\n", (((int)time.tv_sec/3600)%24), (((int)time.tv_sec/60)%60), (((int)time.tv_sec)%60), (time.tv_nsec/100000000), tr->id);
	}
} //load_train


/*
Prints the queues priority, direction, nodes specified by temp4
*/
void print_queue(struct Train * temp4){
	if(temp4 != NULL){
		printf("##%d##%c##", temp4->priority, *temp4->direction);
	}
	while(temp4 != NULL){
		printf("(Train %d)->", temp4->id);
		temp4 = temp4->next;
	}
	printf("\n");
} //print_queue


/*
Given a train station, pops the top train off
*/
struct Train* poptrain(struct Train * head){
	struct Train * popped = head;
	if(head->priority == 1 && *head->direction == 'w'){
		highwesttrain = highwesttrain->next;
	} else if(head->priority == 1 && *head->direction == 'e'){
		higheasttrain = higheasttrain->next;
	} else if(head->priority == 0 && *head->direction == 'w'){
		lowwesttrain = lowwesttrain->next;
	} else if(head->priority == 0 && *head->direction == 'e'){
		loweasttrain = loweasttrain->next;
	}
	return popped;
} //poptrain


/*
Locks the maintrack
Is signaled if a train is added to a queue
If a train is added to a queue, lock the stations, pop the front train, unlock the stations, run the train
Follow the correct train logic
Continue running trains until all have been run
*/
void scheduler(int numtrains){
	int donetrains = 0;
	while(donetrains < numtrains){
		while(pthread_mutex_lock(&maintrack) != 0);
		while(higheasttrain == NULL && highwesttrain == NULL && loweasttrain == NULL && lowwesttrain == NULL){
			pthread_cond_wait(&cmaintrack, &maintrack);
			usleep(100);
		}

		while(pthread_mutex_lock(&higheast) != 0);
		while(pthread_mutex_lock(&highwest) != 0);
		while(pthread_mutex_lock(&loweast) != 0);
		while(pthread_mutex_lock(&lowwest) != 0);

		struct Train * readytrain;
		if(highwesttrain != NULL || higheasttrain != NULL){
			if(higheasttrain == NULL){
				pthread_mutex_unlock(&loweast);
				pthread_mutex_unlock(&lowwest);
				readytrain = poptrain(highwesttrain);
				pthread_mutex_unlock(&highwest);
				pthread_mutex_unlock(&higheast);
			} else if(highwesttrain == NULL){
				pthread_mutex_unlock(&loweast);
				pthread_mutex_unlock(&lowwest);
				readytrain = poptrain(higheasttrain);
				pthread_mutex_unlock(&higheast);
				pthread_mutex_unlock(&highwest);
			}else {
				if(*direc == 'e'){
					pthread_mutex_unlock(&loweast);
					pthread_mutex_unlock(&lowwest);
					readytrain = poptrain(highwesttrain);
					pthread_mutex_unlock(&higheast);
					pthread_mutex_unlock(&highwest);
				}else if(*direc == 'w'){
					pthread_mutex_unlock(&loweast);
					pthread_mutex_unlock(&lowwest);
					readytrain = poptrain(higheasttrain);
					pthread_mutex_unlock(&higheast);
					pthread_mutex_unlock(&highwest);
				}
			}

		} else {
			if(loweasttrain == NULL){
				pthread_mutex_unlock(&higheast);
				pthread_mutex_unlock(&highwest);
				readytrain = poptrain(lowwesttrain);
				pthread_mutex_unlock(&lowwest);
				pthread_mutex_unlock(&loweast);
			} else if(lowwesttrain == NULL){
				pthread_mutex_unlock(&higheast);
				pthread_mutex_unlock(&highwest);
				readytrain = poptrain(loweasttrain);
				pthread_mutex_unlock(&loweast);
				pthread_mutex_unlock(&lowwest);
			}else{
				if(*direc == 'e'){
					pthread_mutex_unlock(&higheast);
					pthread_mutex_unlock(&highwest);
					readytrain = poptrain(lowwesttrain);
					pthread_mutex_unlock(&lowwest);
					pthread_mutex_unlock(&loweast);
				}else if(*direc == 'w'){
					pthread_mutex_unlock(&higheast);
					pthread_mutex_unlock(&highwest);
					readytrain = poptrain(loweasttrain);
					pthread_mutex_unlock(&loweast);
					pthread_mutex_unlock(&lowwest);
				}
			}
		} //main if
		choochoo(readytrain);
		pthread_mutex_unlock(&maintrack);
		donetrains++;
	} //main while
} //Scheduler


/*
MAIN
Initiates all mutex's, convars, barriers
Calls getinput and spawns all the child threads
Calls the scheduler
Waits for child threads to finish before finishing
*/
int main(int argc, char *argv[]){	
	if(argc != 3){
		printf("Please try again sir, not enough inputs\n");
		exit(1);
	} //testing input
	int numtrains = atoi(argv[2]);
	struct Train trains[numtrains];
	struct Train * trainp = trains;
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
       		printf("\n convar init failed\n");
        	return 1;
    	}

	int x = 0;
	for(x = 0; x < numtrains; x++){
		if(pthread_create(&trains[x].thread, NULL, create_thread, &trains[x])){
			fprintf(stderr, "Thread creation failed\n");
			return 1;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &monotime);

	scheduler(numtrains);

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
} //MAIN

