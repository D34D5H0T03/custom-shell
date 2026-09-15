#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "executor.h"
#include "builtin.h"

int main(int argc, char** argv){

    char* line;
    char** args;
    int status = 1;

    //REPL
    do{
        printf("Prompt$> ");

        line = read_line();

        args = tokenize(line);

        if(strcmpi(args[0], "exit")){
            return 0;
        }

        if(args[0] != NULL){
            status = execute_command(args);
        }

        free(line);
        line = NULL;
        free(args);
        args = NULL;

    }while(status);

    return EXIT_SUCCESS;
}