#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[MAX_COMMAND_LINE_LEN];
char delimiters[] = " \t\r\n";
extern char **environ;

void print_prompt() {
    char cwd[MAX_COMMAND_LINE_LEN];
    getcwd(cwd, sizeof(cwd));
    printf("%s:%s$ ", getenv("USER"), cwd);
    fflush(stdout);
}

int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];

    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];

    while (true) {
        do {
            // Print the shell prompt.
            print_prompt();

            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
        } while (command_line[0] == 0x0A); // while just ENTER pressed

        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // 1. Tokenize the command line input (split it on whitespace)
        int arg_count = 0;
        char *token = strtok(command_line, delimiters);
        while (token != NULL && arg_count < MAX_COMMAND_LINE_ARGS - 1) {
            arguments[arg_count] = token;
            token = strtok(NULL, delimiters);
            arg_count++;
        }
        arguments[arg_count] = NULL; // Null-terminate the argument list.

        // 2. Implement Built-In Commands
        if (arg_count > 0) {
            if (strcmp(arguments[0], "exit") == 0) {
                // Exit the shell
                return 0;
            }
            // Add more built-in commands here.

            // Handle input/output redirection
            int input_fd, output_fd;
            int pipe_pos = -1;
            int i = 0;
            for (i < arg_count; i++;) {
                if (strcmp(arguments[i], "<") == 0) {
                    if (i + 1 < arg_count) {
                        input_fd = open(arguments[i + 1], O_RDONLY);
                        if (input_fd == -1) {
                            perror("open");
                            exit(1);
                        }
                        dup2(input_fd, STDIN_FILENO);
                        close(input_fd);
                        arguments[i] = NULL;
                    } else {
                        fprintf(stderr, "Missing input file after <\n");
                        exit(1);
                    }
                } else if (strcmp(arguments[i], ">") == 0) {
                    if (i + 1 < arg_count) {
                        output_fd = open(arguments[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
                        if (output_fd == -1) {
                            perror("open");
                            exit(1);
                        }
                        dup2(output_fd, STDOUT_FILENO);
                        close(output_fd);
                        arguments[i] = NULL;
                    } else {
                        fprintf(stderr, "Missing output file after >\n");
                        exit(1);
                    }
                } else if (strcmp(arguments[i], "|") == 0) {
                    if (i > 0 && i < arg_count - 1) {
                        pipe_pos = i;
                        arguments[i] = NULL;
                    } else {
                        fprintf(stderr, "Invalid use of pipe\n");
                        exit(1);
                    }
                }
            }

            // 3. Create a child process which will execute the command line input
            pid_t child_pid = fork();
            if (child_pid == -1) {
                perror("fork");
            } else if (child_pid == 0) { // Child process
                if (pipe_pos >= 0) {
                    // Create a pipe for inter-process communication
                    int pipe_fds[2];
                    if (pipe(pipe_fds) == -1) {
                        perror("pipe");
                        exit(1);
                    }
                    pid_t sub_child_pid = fork();

                    if (sub_child_pid == -1) {
                        perror("fork");
                        exit(1);
                    }

                    if (sub_child_pid == 0) { // Sub-child process for the second command
                        // Close the write end of the pipe
                        close(pipe_fds[1]);
                        // Redirect the input from the pipe
                        dup2(pipe_fds[0], STDIN_FILENO);
                        close(pipe_fds[0]);
                        // Execute the second command
                        execvp(arguments[pipe_pos + 1], &arguments[pipe_pos + 1]);
                        perror("execvp");
                        exit(1);
                    } else {
                        // Close the read end of the pipe
                        close(pipe_fds[0]);
                        // Redirect the output to the pipe
                        dup2(pipe_fds[1], STDOUT_FILENO);
                        close(pipe_fds[1]);
                        // Execute the first command
                        execvp(arguments[0], arguments);
                        perror("execvp");
                        exit(1);
                    }
                } else {
                    // Execute the command using execvp
                    if (execvp(arguments[0], arguments) == -1) {
                        perror("execvp");
                        exit(1);
                    }
                }
            } else { // Parent process
                // 4. The parent process should wait for the child to complete
                int status;
                if (arg_count > 1 && strcmp(arguments[arg_count - 1], "&") == 0) {
                    // Background process, don't wait
                    arguments[arg_count - 1] = NULL;
                } else {
                    // Wait for the child to complete
                    waitpid(child_pid, &status, 0);
                }
            }
        }
    }
    // This should never be reached.
    return -1;
}