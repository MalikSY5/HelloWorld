#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9
#define NUM_THREADS 27

int board[SIZE][SIZE] = {
    // ... (as provided in the question)
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

typedef struct {
    int row;
    int col;
} ThreadArgs;

void printBoard(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* checkRow(void* args) {
    ThreadArgs* arguments = (ThreadArgs*)args;
    int row = arguments->row;
    int col = arguments->col;
    bool used[SIZE + 1] = {false};

    for (int i = 0; i < SIZE; i++) {
        int num = board[row][i];
        if (used[num]) {
            row_check[row] = false;
            pthread_exit(NULL);
        }
        used[num] = true;
    }
    row_check[row] = true;
    pthread_exit(NULL);
}

void* checkCol(void* args) {
    ThreadArgs* arguments = (ThreadArgs*)args;
    int row = arguments->row;
    int col = arguments->col;
    bool used[SIZE + 1] = {false};

    for (int i = 0; i < SIZE; i++) {
        int num = board[i][col];
        if (used[num]) {
            col_check[col] = false;
            pthread_exit(NULL);
        }
        used[num] = true;
    }
    col_check[col] = true;
    pthread_exit(NULL);
}

void* checkBox(void* args) {
    ThreadArgs* arguments = (ThreadArgs*)args;
    int row = arguments->row;
    int col = arguments->col;
    bool used[SIZE + 1] = {false};

    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            int num = board[i][j];
            if (used[num]) {
                box_check[row / 3 * 3 + col / 3] = false;
                pthread_exit(NULL);
            }
            used[num] = true;
        }
    }
    box_check[row / 3 * 3 + col / 3] = true;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];

    printf("Board:\n");
    printBoard(board);

    // Create threads for each row, column, and box
    int thread_count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            // Create threads for rows
            thread_args[thread_count].row = i;
            thread_args[thread_count].col = j;
            pthread_create(&threads[thread_count], NULL, checkRow, &thread_args[thread_count]);
            thread_count++;

            // Create threads for columns
            thread_args[thread_count].row = i;
            thread_args[thread_count].col = j;
            pthread_create(&threads[thread_count], NULL, checkCol, &thread_args[thread_count]);
            thread_count++;

            // Create threads for boxes
            thread_args[thread_count].row = i;
            thread_args[thread_count].col = j;
            pthread_create(&threads[thread_count], NULL, checkBox, &thread_args[thread_count]);
            thread_count++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the results
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }

    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }

    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }

    return 0;
}
