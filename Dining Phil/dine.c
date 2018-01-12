/*
Nme: Natalie Ang
Course: CPSC 351 - 01, Operating Systems, Fall 2017
Instructor: Professor William McCarthy

To compile the program, use the command "gcc dine.c -lpthread" 
Ignore the warnings and then run with "./a.out"

5 philosophers, 5 chopsticks, each philosopher eats with two chopsticks.
This program goes on forever until user terminates program themself.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_OF_PHILS 5

int i;

pthread_mutex_t chopsticks[NUM_OF_PHILS];
pthread_t phils[NUM_OF_PHILS];

void *phil(int PhilNum);

int main() {

	for (i = 0; i < NUM_OF_PHILS; ++i) {
		pthread_mutex_init(&chopsticks[i], NULL);
	}

	for (i = 0; i < NUM_OF_PHILS; ++i) {
		pthread_create(&phils[i], NULL, phil, (void *)i);
	}

	for (i = 0; i < NUM_OF_PHILS; ++i) {
		pthread_join(phils[i], NULL);
	}

	pthread_exit(NULL);
}

void *phil(int PhilNum) {
	while (1) {
		//thinking
		printf("Phil %d is now thinking\n", PhilNum);
		sleep(2);

		//pick up fork
		if (PhilNum & 1){
			printf("Phil %d is waiting for chopstick %d\n", PhilNum, (PhilNum + 5) % NUM_OF_PHILS);
			pthread_mutex_lock(&chopsticks[(PhilNum + 5) % NUM_OF_PHILS]);
			
			printf("Phil %d picked up chopstick %d\n", PhilNum, (PhilNum + 5) % NUM_OF_PHILS);
			printf("Phil %d is waiting for chopstick %d\n", PhilNum, (PhilNum + 1) % NUM_OF_PHILS);
			
			pthread_mutex_lock(&chopsticks[(PhilNum + 1) % NUM_OF_PHILS]);
			printf("Phil %d picked up chopstick %d\n", PhilNum, (PhilNum + 1) % NUM_OF_PHILS); 
		} else {
			printf("Phil %d is waiting for chopstick %d\n", PhilNum, (PhilNum + 5) % NUM_OF_PHILS);
			pthread_mutex_lock(&chopsticks[(PhilNum + 5) % NUM_OF_PHILS]);
			
			printf("Phil %d picked up chopstick %d\n", PhilNum, (PhilNum + 5) % NUM_OF_PHILS);
			printf("Phil %d is waiting for chopstick %d\n", PhilNum, (PhilNum + 1) % NUM_OF_PHILS);
			
			pthread_mutex_lock(&chopsticks[(PhilNum + 1) % NUM_OF_PHILS]);
			printf("Phil %d picked up chopstick %d\n", PhilNum, (PhilNum + 1) % NUM_OF_PHILS);
		}

		//eating
		printf("Phil %d is now eating\n", PhilNum);
		sleep(2);

		//put down fork
		printf("Phil %d puts down their chopsticks\n", PhilNum);
		pthread_mutex_unlock(&chopsticks[(PhilNum + 1) % NUM_OF_PHILS]);
		pthread_mutex_unlock(&chopsticks[(PhilNum + 5) % NUM_OF_PHILS]);
	}
}

