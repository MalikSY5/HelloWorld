#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <argument>\n", argv[0]);
        return 1;
    }

    // Create two pipes
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }

    // Fork the first child process (P1 - cat scores)
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(1);
    } else if (pid1 == 0) {
        // Child process (P1)
        // Close unnecessary pipe ends
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        // Redirect stdout to pipe1
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);

        // Execute 'cat scores'
        execlp("cat", "cat", "scores", NULL);
        perror("exec");
        exit(1);
    }

    // Fork the second child process (P2 - grep <argument>)
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(1);
    } else if (pid2 == 0) {
        // Child process (P2)
        // Close unnecessary pipe ends
        close(pipe1[1]);
        close(pipe2[0]);

        // Redirect stdin from pipe1 and stdout to pipe2
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[1]);

        // Execute 'grep <argument>'
        execlp("grep", "grep", argv[1], NULL);
        perror("exec");
        exit(1);
    }

    // Fork the third child process (P3 - sort)
    pid_t pid3 = fork();
    if (pid3 < 0) {
        perror("fork");
        exit(1);
    } else if (pid3 == 0) {
        // Child process (P3)
        // Close unnecessary pipe ends
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        // Redirect stdin from pipe2
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]);

        // Execute 'sort'
        execlp("sort", "sort", NULL);
        perror("exec");
        exit(1);
    }

    // Parent process
    // Close all pipe ends
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // Wait for the child processes to finish
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}