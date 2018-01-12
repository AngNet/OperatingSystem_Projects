/*
Nme: Natalie Ang
Course: CPSC 351 - 01, Operating Systems, Fall 2017
Instructor: Professor William McCarthy

To compile: g++ sudoku.cpp -pthread
To run: ./a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
using namespace std;

int board[9][9] = {
    {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

typedef struct {
    int col, row;
    int (* tempBoard)[9];
 
} data;
 
int result[11] = {0}, temp = {0}, threadIndex = 0;
int rowCount, colCount, temp_row, temp_col;
pthread_t threads[11]; void * beta; int counter = 0;

void *check_row(void *param);
void *check_col(void *param);
void *check_square(void *param);

int main() {
    
    /*check row and col*/
    data *tr = (data *) malloc(sizeof(data));
    (*tr).row = 0;
    (*tr).col = 0;
    (*tr).tempBoard = board;
    pthread_create(&threads[0], NULL, check_col, tr);
    pthread_create(&threads[1], NULL, check_row, tr);

    /*check squares*/
    threadIndex = 2;
    for(int x = 0; x<= 6; x += 3){
        for(int y = 0; y <= 6; y += 3) {
            data *boardValue = (data *) malloc(sizeof(data));
            (*boardValue).row = x;
            (*boardValue).col = y;
            (*boardValue).tempBoard = board;
            pthread_create(&threads[threadIndex++], NULL, check_square, boardValue);
        }
    }
    
    /*join threads*/
     for (int x = 0; x < 11; x++) {
        pthread_join(threads[x], &beta);
     }
        
    if(counter == 11){
        printf("The sudoku board is valid\n");
    } else {
        printf("The sudoku board invalid\n");
    }

    return 0;
}

void *check_row(void *param)
{
    data *my_data = (data *) param;
    int col = (*my_data).col;
    int row = (*my_data).row;
 
    for(int x = row; x < 9; x++){
        int temp[10] = {0};
        for(int y = col; y < 9; y++){
            int val = my_data->tempBoard[x][y];

            if(temp[val] != 0){
                return (void *) 0;
            } else{
                temp[val] = 1;
            }
        }
    }
    counter++;
    return (void*) 1;
}
 
void *check_col(void *param)
{
    data *my_data = (data *) param;
    int col = (*my_data).col;
    int row = (*my_data).row;
 
    for(int x = col; x < 9; x++){
        int temp[10] = {0};
        for(int y = row; y < 9; y++){
            int val = my_data->tempBoard[x][y];

            if(temp[val] != 0){
                return (void *) 0;
            } else{
                temp[val] = 1;
            }
        }
    }
    counter++;
    return(void* ) 1;
 }
 
void *check_square(void * param)
{
    data *my_data = (data *) param;
    int row = (*my_data).row;
    int col = (*my_data).col;
    int temp[10] = {0};

    for (int x = row; x < row + 3; x++){
        for (int y = col; y < col + 3; y++){
            int val = (*my_data).tempBoard[x][y];
            if (temp[val] != 0){
                return(void *)0;
            }      
            else{
                temp[val] = 1;
            }   
        }
    }
    counter++;
    return (void *) 1;
}
