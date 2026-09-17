#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

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