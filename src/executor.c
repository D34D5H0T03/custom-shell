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

    //search and execute piped command
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            return execute_piped_command(args, i);
        }
    }

    if(strcmp(args[0], "exit") == 0){
        return shell_exit(args); 
    }
    else if(strcmp(args[0], "cd") == 0){
        return shell_cd(args);
    }

    pid_t pid = fork();

    if(pid == 0){

        struct sigaction sa_default;
        sa_default.sa_handler = SIG_DFL;
        sigemptyset(&sa_default.sa_mask);
        sa_default.sa_flags = 0;

        sigaction(SIGINT, &sa_default, NULL);
        sigaction(SIGTSTP, &sa_default, NULL);

        handle_redirection(args); //redirection of I/O

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

void handle_redirection(char** args){

    //finding the index
    int in_redirect_index = -1;
    int out_redirect_index = -1;
    
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            in_redirect_index = i;
        } else if (strcmp(args[i], ">") == 0) {
            out_redirect_index = i;
        }
    }

    //input redirection
    if (in_redirect_index != -1) {
        char *filename = args[in_redirect_index + 1];
        if (filename == NULL) {
            fprintf(stderr, "custom_shell: syntax error near unexpected token `newline'\n");
            exit(EXIT_FAILURE);
        }
        int fd_in = open(filename, O_RDONLY);
        if (fd_in < 0) {
            perror("custom_shell: failed to open input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
        args[in_redirect_index] = NULL;
    }

    //output redirection
    if (out_redirect_index != -1) {
        char *filename = args[out_redirect_index + 1];
        if (filename == NULL) {
            fprintf(stderr, "custom_shell: syntax error near unexpected token `newline'\n");
            exit(EXIT_FAILURE);
        }
        int fd_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out < 0) {
            perror("custom_shell: failed to open output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
        args[out_redirect_index] = NULL;
    }
}

int execute_piped_command(char** args, int pipe_index){
    //split the argument into two
    args[pipe_index] = NULL;
    char** left_args = args;
    char** right_args = &args[pipe_index + 1];

    if(right_args == NULL){
        fprintf(stderr, "Custom Shell: syntax error near unexpected token `|'\n");
        return 1;
    }

    int pipefd[2]; //pipefd[0] for reading and pipefd[1] for writing
    if (pipe(pipefd) < 0){
        perror("Pipe failed!");
        return 1;
    }

    pid_t pid1, pid2;

    //left command
    pid1 = fork();
    
    if(pid1 == 0){
        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[0]);
        close(pipefd[1]);

        handle_redirection(left_args);

        if (execvp(left_args[0], left_args) == -1) {
            perror("Failed to execute left command");
            exit(EXIT_FAILURE);
        }
    }

    //right command
    pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        
        close(pipefd[0]);
        close(pipefd[1]);

        handle_redirection(right_args);

        if (execvp(right_args[0], right_args) == -1) {
            perror("Failed to execute right command");
            exit(EXIT_FAILURE);
        }
    }

    close(pipefd[0]);
    close(pipefd[1]);

    //wait for child processes
    int status;
    do{
        waitpid(pid1, &status, WUNTRACED);
    }while(!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));

    do{
        waitpid(pid2, &status, WUNTRACED);
    }while(!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));

    return 1;
}