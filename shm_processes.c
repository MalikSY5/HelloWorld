#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_TRANSACTIONS 25

int BankAccount = 0;
int Turn = 0;

// Mutexes and condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t parent_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t child_cond = PTHREAD_COND_INITIALIZER;

void* parent_function(void* arg) {
    srand(time(NULL));

    for (int i = 0; i < NUM_TRANSACTIONS; i++) {
        sleep(rand() % 6);  // Sleep for a random time (0-5 seconds)
        
        int account;
        pthread_mutex_lock(&mutex);
        account = BankAccount;

        while (Turn != 0) {
            pthread_cond_wait(&parent_cond, &mutex);
        }

        int balance = rand() % 101;  // Generate a random balance amount (0-100)
        if (balance % 2 == 0) {
            BankAccount += balance;
            printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, BankAccount);
        } else {
            printf("Dear old Dad: Doesn't have any money to give\n");
        }

        Turn = 1;
        pthread_cond_signal(&child_cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void* child_function(void* arg) {
    srand(time(NULL));

    for (int i = 0; i < NUM_TRANSACTIONS; i++) {
        sleep(rand() % 6);  // Sleep for a random time (0-5 seconds)

        int account;
        pthread_mutex_lock(&mutex);
        account = BankAccount;

        while (Turn != 1) {
            pthread_cond_wait(&child_cond, &mutex);
        }

        int balance = rand() % 51;  // Generate a random balance amount (0-50)
        printf("Poor Student needs $%d\n", balance);

        if (balance <= account) {
            BankAccount -= balance;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, BankAccount);
        } else {
            printf("Poor Student: Not Enough Cash ($%d)\n", account);
        }

        Turn = 0;
        pthread_cond_signal(&parent_cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t parent_thread, child_thread;

    pthread_create(&parent_thread, NULL, parent_function, NULL);
    pthread_create(&child_thread, NULL, child_function, NULL);

    pthread_join(parent_thread, NULL);
    pthread_join(child_thread, NULL);

    return 0;
}