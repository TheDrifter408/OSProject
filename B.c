#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3

void child_process(int child_number, int write_pipe) {
    char buffer[2]; /*To store a single character and the NULL terminator*/
    char read_char; /*to store the keystroke from the keyboard*/

    while (1) {
        read(STDIN_FILENO, &read_char, 1);

        if (read_char == '\n' && buffer[0] == '\n') {
            break;  /*Terminate when two consecutive newline characters are received*/ 
        }

        buffer[0] = read_char;
        write(write_pipe, buffer, 1);
    }

    close(write_pipe);
    exit(EXIT_SUCCESS);
}

int main() {
    // 2D array to store read and write ends of pipes
    int pipes[NUM_CHILDREN][2];  
    pid_t child_pids[NUM_CHILDREN];

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }

        child_pids[i] = fork();

        if (child_pids[i] == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (child_pids[i] == 0) {
            // Child process
            close(pipes[i][0]);  
            child_process(i, pipes[i][1]);
        } else {
            // Parent process
            close(pipes[i][1]);  
        }
    }

    char buffer;
    int bytes_read;

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        while ((bytes_read = read(pipes[i][0], &buffer, 1)) > 0) {
            write(STDOUT_FILENO, &buffer, 1);
        }
        close(pipes[i][0]);
    }

    // Wait for all child processes to terminate
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        waitpid(child_pids[i], NULL, 0);
    }

    return EXIT_SUCCESS;
}