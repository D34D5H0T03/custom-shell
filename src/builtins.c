#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int shell_cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "Expected arguement to \"cd\"\n");
    }else{
        if(chdir(args[1]) != 0){
            perror("Failed to change directory!");
        }
    }
    return 1;
}

int shell_exit(char **args){
    return 0;
}