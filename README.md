# Custom Unix Shell in C

An interactive Unix shell implementation written in C following a guided tutorial online to learn POSIX API calls, system calls, process management, and the core of operating system interfaces.

---

## Overview

This project implements the basic life of a Unix Shell: the Read-Eval-Print Loop. It handles reading lines from the user, input tokenization, and executes external binaries by managing child processes, and provides custom implementations for built-in shell commands.

### Core Systems Concepts Implemented

- Process Creation & Management: Using `fork()` to create duplicate child processes and `waitpid()` to manage the parent-child sync.
- Program Execution: Utilizing `execvp()` to search for the program, stop the child process, and start the binaries located in system `$PATH`
- Dynamic Memory Management: Managing buffer allocations for user input and token arrays using `malloc()`, `realloc()`, and `free()`.
- Built-in Commands: Interacting directly with the process environment (e.g. `chdir()` for navigating different directories)

---

## Features

### Built-in Commands

- `cd <dir>` - Changes the current working directory
- `pwd` - Displays the current working directory
- `help` - Displays information on available built-in commands
- `exit` - Exits the shell

### Limitations

- No piping
- No redirection
- No quoting or escaping whitespace

### Program Execution

- Supports running any standard system binaries and executable paths (e.g. `ls`, `grep`, `cat`).

---

## How It Works

The lifetime of a shell is a basic loop (Reading input -> Parsing a line -> Executing a line --> Repeat)

1. Reading commands from input
2. Splitting commands into programs and arguments (i.e. tokens)
3. Executing parsed commands:

- IF the command is a built-in argument, it runs the built-in function
- Otherwise it creates a new process (using `fork()`) and runs the new program (`execvp()`)

---

## Getting Started

### Prerequisites

- GCC / Clang
- POSIX-compliant environment (Linux/MacOS)

### Compilation

- Compile the source using `gcc`.

```bash
gcc -Wall -Wextra -pedantic -std=c99 main.c -o myshell
```

### Running the Shell

```bash
./myshell
```

---

## Next Steps

- [ ] Piping
- [ ] Redirection
- [ ] Quoting

---

## Acknowledgments

- Inspired by Stephen Brennan's Tutorial: [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)
