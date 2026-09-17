#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include "executor.h"
#include "builtin.h"

int execute_command(char** args){
    if(args[0] == NULL){
        return 1; //no command to execute
    }

    if(strcmp(args[0], "exit") == 0){
        return shell_exit(args); 
    }
    else if(strcmp(args[0], "cd") == 0){
        return shell_cd(args);
    }

    //output redirection finding the command
    int redirect_index = -1;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) { 
            redirect_index = i;
            break;
        }
    }

    pid_t pid = fork();

    if(pid == 0){

        struct sigaction sa_default;
        sa_default.sa_handler = SIG_DFL;
        sigemptyset(&sa_default.sa_mask);
        sa_default.sa_flags = 0;

        sigaction(SIGINT, &sa_default, NULL);
        sigaction(SIGTSTP, &sa_default, NULL);

        //output redirection
        if(redirect_index != -1){
            char* filename = args[redirect_index + 1]; //taking the filename

            if(filename == NULL){
                fprintf(stderr, "Syntax Error: File name not provided");
                exit(EXIT_FAILURE);
            }

            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); //0644 is file permissions
            if(fd < 0){
                perror("Failed to open file!");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);

            args[redirect_index] = NULL;
        }

        if(execvp(args[0], args) == -1){
            perror("Failed to Execute command.\n");
            exit(EXIT_FAILURE);
        }
    }

    else if(pid < 0){
            perror("Forking failed.\n");
    }

    else{
        int status; //basically telemetry data of the child processes death.
            
        do{
            waitpid(pid, &status, WUNTRACED); // wake up if paused
        }while(!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status)); //if dies a natural death or killed by os or stopped/paused with ctr+z

    }

    return 1;
}