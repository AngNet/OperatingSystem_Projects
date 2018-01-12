/*
Nme: Natalie Ang
Course: CPSC 351 - 01, Operating Systems, Fall 2017
Instructor: Professor William McCarthy

To compile the program, use the command "gcc bank.c -lpthread" 
and then run with "./a.out 10 5 7"

20 situations of customers requesting and releasing materials is done
right away. Ending is stated with "Banker Simulation Done". After
running the program, scroll up to see the situations that took place.

*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define resourceNum 3
#define customerNum 5

time_t t; //to generate random numbers
int work[resourceNum];
int countLower = 0, countBigger = 0; //count variables
int i = 0, j = 0, x = 0, k = 0; //index variables
pthread_mutex_t mutex;

//The Bank
int available[resourceNum];
int max[customerNum][resourceNum] = {
	{2,5,4},
	{5,7,3},
	{6,5,2},
	{5,1,4},
	{3,2,7}
};
int allocation[customerNum][resourceNum] = {
	{2,0,1},
	{3,1,0},
	{2,1,1},
	{0,1,3},
	{1,2,0}
};
int need[customerNum][resourceNum];

//The Customers
int requestResource(int processID, int random[]);
int releaseResource(int processID, int releaseVector[]);

int checkSafe();
void initMatrix();
void createAndJoinThread();

//Display functions
void displayNeed();
void displayAllocation();
void displayAvailable();


void *customer(void* custID);

int main(int argc, char const *argv[]){

	//initialie random number generator
	srand((unsigned) time(&t));

	//Stop program is 3 numbers aren't given
	if(argc < 4 || argc > 4){
		printf("Enter three numbers.\n");
		return 0;
	}

	//Get the input from the user when running program
	for(i = 0; i < resourceNum; i++){
		available[i] = atoi(argv[i+1]);
	}

	//Initialize matrix with the 3 numbers given by user
	initMatrix();

	//display current matrices
	displayAvailable();
	displayAllocation();
	displayNeed();

	//create the thread and join them
	createAndJoinThread();

	return 0;
}

void createAndJoinThread(){
	pthread_mutex_init(&mutex, NULL);
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_t *tid = malloc(sizeof(pthread_t) * customerNum);
	int *pid = malloc(sizeof(int) * customerNum);
	
	for(i = 0; i < customerNum; i++){
		*(pid + i) = i;
		pthread_create((tid+i), &attr, customer, (pid + i));
	}

	for(i = 0; i < customerNum; i++){
		pthread_join(*(tid + i), NULL);
	}

	//When simulation goes through 20 situations, this is displayed
	printf("\nBanker Simulation Done.\n");
	exit(1);
}

void initMatrix(){
	//initialize and display allocated and need matrix
	for (i = 0; i < customerNum; ++i){
		for (j = 0; j < resourceNum; ++j){
			need[i][j] = max[i][j] - allocation[i][j];
		}
	}
}
void *customer(void* custID){
	int processID = *(int*)custID;

	for(x = 0; x < 20; x++){
		int request[resourceNum];
		pthread_mutex_lock(&mutex);

		//generate random matrix
		for(i = 0; i < resourceNum; i++) {
			if(need[processID][i] != 0) {
				request[i] = rand() % need[processID][i];
			}
			else{
				request[i] = 0;
			}
		}

		printf("\nCustomer %d requesting: ", processID);
		for (i = 0; i < resourceNum; ++i){
			printf("%d  ", request[i]);
		}printf("\n");

		requestResource(processID, request);
		pthread_mutex_unlock(&mutex);
	
		int release[resourceNum];
		pthread_mutex_lock(&mutex);

		//creating the new release matrix
		for(i = 0; i < resourceNum; i++){
			if(allocation[processID][i] != 0){
				release[i] = rand() % allocation[processID][i];
			}
			else{
				release[i] = 0;
			}
		}
		
		printf("\nCustomer %d releasing: ", processID);
		for (i = 0; i < resourceNum; ++i){
			printf("%d  ", release[i]);
		}printf("\n");

		releaseResource(processID, release);
		pthread_mutex_unlock(&mutex);
	}
}

int requestResource(int processID,int random[]){
	//check if resource is bigger than asked

	for (i = 0; i < resourceNum; ++i){
		if (random[i] <= need[processID][i]){
			continue;
		}
		else{
			countBigger = 1;
		}
	}

	//display result
	if (countBigger == 1){
		printf("\nRequested resources is bigger than needed.\n");
		return -1;
	}
	printf("\nRequested resources are not more than needed.\n");

	//check if enough can be allocated
	int countEnough = 0;
	for (i = 0; i < resourceNum; ++i){
		if (random[i] <= available[i]){
			continue;
		}
		else{
			countEnough = 1;
		}
	}

	//display result
	if(countEnough == 1){
		printf("There is not enough resources for this process.\n");
		return -1;
	}

	//calculating the new allocated and need matrix
	for (i = 0; i < resourceNum; ++i){
		need[processID][i] = need[processID][i] - random[i];
		allocation[processID][i] = allocation[processID][i] + random[i];
		available[i] = available[i] - random[i];
	}
	
	//check if the system is safe or not
	if (checkSafe() == 0){
		printf("System is safe and allocation is done.\n");
		displayAvailable();
		displayAllocation();
		displayNeed();
		return 0;
	} else{
		printf("Not safe. System has retracted.\n");
		for (i = 0; i < resourceNum; ++i){
			need[processID][i] = need[processID][i] + random[i];
			allocation[processID][i] = allocation[processID][i] - random[i];
			available[i] = available[i] + random[i];
		}
		return -1;
	}
}

int releaseResource(int processID,int releaseVector[]){

	for (i = 0; i < resourceNum; ++i){
		if (releaseVector[i] <= allocation[processID][i]){
			continue;
		} else{
			countLower = 1;
		}
	}

	if(countLower == 1){
		printf("Not enough resource held by process to release.\n");
		return -1;
	}

	//calculate the new allocated and new matrix
	for(i = 0; i < resourceNum; i++){
		allocation[processID][i] = allocation[processID][i] - releaseVector[i];
		need[processID][i] = need[processID][i] + releaseVector[i];
		available[i] = available[i] + releaseVector[i];
	}

	//display matrices
	printf("Release complete.\n");
	displayAllocation();
	displayNeed();
	return 0;
}

void displayNeed(){
	printf("\nCurrent need matrix:\n");
	for (i = 0; i < customerNum; ++i){
		printf("\t{ ");
		for (j = 0; j < resourceNum; ++j){
			printf("%d  ", need[i][j]);
		}
		printf("}\n");
	}
	return;
}

void displayAllocation(){
	printf("\nCurrent allocated matrix:\n");
	for (i = 0; i < customerNum; ++i){
		printf("\t{ ");
		for (j = 0; j < resourceNum; ++j){
			printf("%d  ", allocation[i][j]);
		}
		printf("}\n");
	}
	return;
}

void displayAvailable(){
	printf("\nCurrent available resources: ");
	for (i = 0; i < resourceNum; ++i){
		printf("%d  ",available[i]);
	}printf("\n");
	return;
}
int checkSafe(){ 
	int finish[customerNum] = {0};

	for(i = 0; i < resourceNum; i++){
		work[i] = available[i];
	}

	for(i = 0; i < customerNum; i++) {
		if (finish[i] == 0){ //if finish is false
			for(j = 0; j < resourceNum; j++){
				if(need[i][j] <= work[j]){
					if(j == resourceNum - 1){
						finish[i] = 1; //if finish is true
						for (k = 0; k < resourceNum; ++k){
							work[k] += allocation[i][k];
						}
						i = -1;
						break; //go back to the beginning
					} else {
						continue;
					}
				} else {
					break;
				}
			}
		} else {
			continue;
		}
	}

	//if loop goes through, system is safe
	for(i = 0; i < customerNum; i++){
		if (finish[i] == 0){
			return -1;
		} else{
			continue;
		}
	}
	return 0;
}