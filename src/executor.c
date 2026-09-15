#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "executor.h"

int execute_command(char** args){
    if(args[0] == NULL){
        return 1; //no command to execute
    }

    pid_t pid = fork();

    if(pid == 0){
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
        }while(!WIFEXITED(status) && !WIFSIGNALED(status)); //if dies a natural death or killed by os

    }

    return 1;
}