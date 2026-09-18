#ifndef EXECUTOR_H_INCULDED
#define EXECUTOR_H_INCLUDED

int execute_command(char** args);
void handle_redirection(char** args);
int execute_piped_command(char** args, int pipe_index);

#endif