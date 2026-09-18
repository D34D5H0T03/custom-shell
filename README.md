# Custom Tactical Shell

A custom, stealth-oriented Linux shell written in C. This project was built to explore systems programming, process management, and offensive red-team primitives. 

## Core Capabilities

* **Process Masking (Stealth):** Automatically hides the shell from system administrators by rewriting the `argv` memory block and using `prctl` to spoof the kernel thread name as `[kworker/u4:2]` upon execution.
* **Execution Engine:** Implements the classic POSIX fork-execvp-waitpid architecture for command execution.
* **Pipelines (`|`):** Supports chaining two commands together in memory, cross-wiring standard output to standard input using `pipe()` and `dup2()`.
* **I/O Redirection (`<`, `>`):** Handles input reading and output truncation to files by dynamically reassigning file descriptors (0 and 1).
* **Signal Management:** Intercepts `SIGINT` (Ctrl+C) and `SIGTSTP` (Ctrl+Z) to prevent accidental shell termination, while restoring default lethal signals to child processes. Deadlocks from suspended background jobs are handled via `WIFSTOPPED` evaluations.
* **Dynamic Parsing:** Utilizes `getline` and `strtok` for safe, dynamic memory allocation that prevents buffer overflows regardless of input length.
* **Built-in Commands:** Directly modifies the parent process environment for commands like `cd` and `exit`.

## Project Architecture

The shell is modularized into distinct C components:
* `main.c`: Initializes stealth masking, configures signal handlers, and runs the REPL loop.
* `parser.c`: Handles raw string reading and tokenization[cite: 3].
* `executor.c`: Manages forking, standard execution, pipelines, and file redirection.
* `builtins.c`: Houses commands that must execute within the parent process context.
* `stealth.c`: Contains offensive primitives for process masquerading.

## Build and Run

The project includes a `Makefile` that compiles the binary into a dedicated, git-ignored `bin/` directory.

```bash
# Compile the project
make

# Run the shell
./bin/custom_shell

# Clean the build directory
make clean
