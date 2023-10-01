#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

// Global variables
int alarm_count = 0;
time_t start_time, end_time;

// Signal handler for SIGINT (CTRL-C)
void sigint_handler(int signo) {
    end_time = time(NULL);
    double execution_time = difftime(end_time, start_time);

    printf("\nProgram executed for %.2lf seconds.\n", execution_time);

    exit(0);
}

void handler(int signum)
{ //signal handler
  while(1 == 1){
    wait(1);
    printf("Hello World!\n");
    printf("Turing was right!\n");
  }
}

int main() {
    // Register the SIGINT signal handler
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Failed to register SIGINT handler");
        return 1;
    }

    // Record the start time
    start_time = time(NULL);

  	signal(SIGALRM,handler); //register handler to handle SIGALRM
  	alarm(1); //Schedule a SIGALRM for 1 second
  	while(1); //busy wait for signal to be delivered
		sigalrm_handler(1);

    return 0;
}
