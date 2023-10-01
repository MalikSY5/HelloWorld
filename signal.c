/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{ //signal handler
  while(1 == 1){
    wait(1);
    printf("Hello World!\n");
    printf("Turing was right!\n");
  }
  
}

int main(int argc, char * argv[])
{
  signal(SIGALRM,handler); //register handler to handle SIGALRM
  alarm(1); //Schedule a SIGALRM for 1 second
  while(1); //busy wait for signal to be delivered
  return 0; //never reached
}
