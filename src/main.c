#include <stdio.h>
#include <stdlib.h>

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

        //execution step

        free(line);
        line = NULL;
        free(args);
        args = NULL;

    }while(status);

    return EXIT_SUCCESS;
}