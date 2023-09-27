#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
  srandom(time(NULL));
	int i;

	for (i = 0; i < 2; i++){
		pid_t child_pid = fork();

		if (child_pid == -1){
			printf("Fork Failed!!!");
			exit(1);
		}
		else if(child_pid == 0){
			printf("Fork Successful, Now in child process!");
			int loop = (random() % 30) + 1;
			int x;
			for (x = 0; x < loop; x++){
				printf("Child Pid: %d is going to sleep!\n", getpid());
				sleep((random() % 10) + 1);
        printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
			}
			exit(0);
		}
	}
	int cooldown;
	int j;
	for (j = 0; i < 2; i++){
		wait(&cooldown);
	}

	printf("Parent process ending\n");
	return 0;
	}

