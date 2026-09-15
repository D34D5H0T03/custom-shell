#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define TOKEN_BUFFSIZE 64
#define TOKEN_DELIMITERS " \t\r\n\a" 

char* read_line(){
    char* line = NULL;
    size_t buffer = 0;

    if(getline(&line, &buffer, stdin) == -1){ //reading input here with getline
        if(feof(stdin)){ //in the case the user wants to exit
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        else{ //in the case of crash/ running out of RAM
            perror("Getline Failed!");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char** tokenize(char* input){
    int buffsize = TOKEN_BUFFSIZE;
    int index = 0;

    char **tokens = malloc(buffsize * sizeof(char*));
    char *token;

    if(!tokens){
        perror("Memory Allocation Failure!");
        exit(EXIT_FAILURE);
    }

    token = strtok(input, TOKEN_DELIMITERS);

    while(token != NULL){
        tokens[index] = token;
        index++;

        if(index >= buffsize){
            buffsize += TOKEN_BUFFSIZE;
            tokens = realloc(tokens, buffsize * sizeof(char*));
            if(!tokens){
                perror("Meomory Reallocation Error!");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }

    tokens[index] = NULL; //execvp needs NULL at the end

    return tokens;
}