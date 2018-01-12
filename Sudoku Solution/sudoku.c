/*
Nme: Natalie Ang
Course: CPSC 351 - 01, Operating Systems, Fall 2017
Instructor: Professor William McCarthy

To compile: gcc sudoku.c -lpthread
To run: ./a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

typedef struct 
{
    int row;
    int col;
    int (* lineBoard)[9];
} data;

pthread_t thread[11]; void * beta; int y, z;

void *walk_row(void * param);
void *walk_col(void * param);
void *check_square(void * param);


int main(void) 
{
    data *tr = (data*)malloc(sizeof(data));
    tr->row = 0; tr->col = 0;
    tr->lineBoard = board;  
    pthread_create(&thread[0], NULL, walk_row, (void*)tr);
    pthread_create(&thread[1], NULL, walk_col, (void*)tr);

    int i = 2;
    for(y = 0; y <= 6; y += 3){
        for(z = 0;z <= 6;z += 3){
            data *sudoku = (data*)malloc(sizeof(data));
            sudoku->row = y;
            sudoku->col = z;
            sudoku->lineBoard = board;

            pthread_create(&thread[i++], NULL, check_square, (void*)sudoku);
        }
    }

    int s = 0;
    for(i = 0;i < 11;i++){
        pthread_join(thread[i], &beta);
        if ((int)beta == 1){
            s++;
        }
    }

    /*display to user if board is valid or not*/
    if(s == 11){
        printf("The sudoku board is valid\n");
    } else{
        printf("The sudoku board invalid\n");
    }
    
    return 0;
}

void * walk_row(void * param) {
    data * my_data = (data *) param;
    
    int startRow = my_data->row;
    int startCol = my_data->col;

    for (int x = startCol; x < 9; x++) {
        int col[10] = {0};
        for (int y = startRow; y < 9; y++) {
            int val = my_data->lineBoard[y][x];
            if (col[val] != 0) {
                return (void *) 0;
            }
            else{
                col[val] = 1;
            }
        }
    }
    return (void *) 1;
}


void * walk_col(void * param) {
    data * my_data = (data *) param;
    
    int startRow = my_data->row;
    int startCol = my_data->col;

    for (int x = startCol; x < 9; x++) {
        int col[10] = {0};
        for (int y = startRow; y < 9; y++) {
            int val = my_data->lineBoard[y][x];
            if (col[val] != 0) {
                return (void *) 0;
            }
            else{
                col[val] = 1;
            }
        }
    }
    return (void *) 1;
}

void * check_square(void * param) {
    data *my_data = (data *)param;
    int rowCount = my_data->row;
    int colCount = my_data->col;

    int temp[10] = {0};
    for (int i = rowCount; i < rowCount + 3; ++i) {
        for (int j = colCount; j < colCount + 3; ++j) {
            int val = my_data->lineBoard[i][j];
            if (temp[val] != 0) {
                return (void *) 0;
            }
            else{
                temp[val] = 1;
            }
        }
    }
    return (void *) 1;
}