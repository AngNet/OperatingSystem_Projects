/*
Nme: Natalie Ang
Course: CPSC 351 - 01, Operating Systems, Fall 2017
Instructor: Professor William McCarthy

To compile the program, use the command "gcc thread.c -lpthread" 
and then ./a.out 

Sample Output: 
0, 1, 18, 24, 28, 31, 42, 43, 55, 81, 92, 102
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *runner(void *param);  
void mergeTogether(int arr[], int left, int middle, int right);
void mergeSort(int arr[], int left, int right);
void printResults();


int array[12] = {18, 92, 43, 28, 81, 42, 102, 0, 31, 24, 1, 55};
int length = 12, integer;
int x, y, z;

int main()
{
    pthread_t tid[3]; pthread_attr_t attr[3];
    char *thread1 = "one", *thread2 = "two", *thread3 = "three";

    //create attributes
    for(x = 0; x < 3; x++){
        pthread_attr_init(&attr[x]); 
    }

    //create threads  
    pthread_create(&tid[0], &attr[0], runner, thread1);
    pthread_create(&tid[1], &attr[1], runner, thread2);
    pthread_create(&tid[2], &attr[2], runner, thread3);

    //join threads
    for(x = 0; x < 3; x++){
        pthread_join(tid[x], NULL);
    }
        
    printResults();
    pthread_exit(0);    
    
    return 0;
}

void mergeTogether(int arr[], int left, int middle, int right) {
    
    int firstHalf = middle - left + 1;
    int secondHalf = right - middle;

    int first[firstHalf], second[secondHalf]; 

    for (x = 0; x < firstHalf; x++) {
        first[x] = arr[left + x];
    }

    for (y = 0; y < secondHalf; y++) {
        second[y] = arr[middle + 1 + y];
    }
    
    x = 0, y = 0; z = left;

    while (x < firstHalf && y < secondHalf) {
        
        if (first[x] <= second[y]) {
            arr[z] = first[x];
            ++x;
        } else {
            arr[z] = second[y];
            y++;
        }

        z++;
    }

    while (x < firstHalf) {
        arr[z] = first[x];
        x++; z++;
    }

    while (y < secondHalf) {
        arr[z] = second[y];
        y++; z++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int middle = (left + (right - 1)) / 2;
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);
        mergeTogether(arr, left, middle, right);
    }
}
void *runner(void *param)
{
    int midpoint = length / 2;
    if (strcmp(param, "one") == 0) {
        mergeSort(array, 0, midpoint);
    }

    if (strcmp(param, "two") == 0) {
        mergeSort(array, midpoint + 1, length - 1);
    }

    if (strcmp(param, "three") == 0) {
        mergeTogether(array, 0, (length / 2), length - 1);
    }

    pthread_exit(0);
}
void printResults(){
        for (y = 0; y < length; y++) {
        if (y == length - 1) {
            printf("%d", array[y]);
        } else {
            printf("%d, ", array[y]);
        }
    } printf("\n");
}