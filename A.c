#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void collatzSequence(int n){
    while (n != 1){
        printf("%d ",n);
        if(n % 2 == 0){
            n = n / 2;
        } else {
            n = n * 3 + 1;
        }
    }
    printf("%d ",n);
}

int main(int argc,char *argv[]){
    int input = 0;
    printf("Enter an Integer: ");
    scanf("%d",&input);
    if(input < 0){
        printf("Enter a positive integer: ");
        while(1){
            scanf("%d",&input);
            if(input > 0){
                return 0;
            }
        }
    }
    printf("Number = %d \n",input);
    
    /*Starting the child process*/
    pid_t child_pid = fork();

    if( child_pid < 0){
        perror("Forking failed.");
        return EXIT_FAILURE;
    }
    if( child_pid == 0){
        collatzSequence(input);
    } else {
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
