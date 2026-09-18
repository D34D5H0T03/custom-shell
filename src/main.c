#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "parser.h"
#include "executor.h"
#include "builtin.h"
#include "stealth.h"

int main(int argc, char** argv){

    mask_process(argv, "[kworker/u4:2]");

    char* line;
    char** args;
    int status = 1;

    //ctr+c signal interrupt handling
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGINT, &sa, NULL) == -1){
        perror("Failed to interrupt SIGINT"); //ctr+c
    }
    
    if(sigaction(SIGTSTP, &sa, NULL) == -1){
        perror("Failed to ignore SIGTSTP"); //ctr+z
    }

    //REPL
    do{
        printf("Prompt$> ");

        line = read_line();

        args = tokenize(line);

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