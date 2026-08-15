#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_pid_t.h>
#include <sys/wait.h>
#include <unistd.h>

char *fsh_read_line() {
  printf(">>> ");

  int line_size = 1024, position = 0, c;
  char *buffer = malloc(sizeof(char) * line_size);

  if (!buffer) {
    fprintf(stderr, "fsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while ((c = getchar()) != EOF && c != '\n') {
    *(buffer + position) = c;
    position++;

    if (position >= line_size) {
      line_size += line_size;
      buffer = realloc(buffer, line_size);

      if (!buffer) {
        fprintf(stderr, "fsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  *(buffer + position) = '\0';
  return buffer;
}

char **fsh_parse_line(char *line) {
  int token_size = 64, position = 0;
  char **tokens = malloc(sizeof(char *) * token_size);
  char *token;

  if (!tokens) {
    fprintf(stderr, "fsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, " \t\r\n\a");
  while (token != NULL) {
    *(tokens + position) = token;
    position++;

    if (position >= token_size) {
      token_size += token_size;
      tokens = realloc(tokens, sizeof(char *) * token_size);

      if (!tokens) {
        fprintf(stderr, "fsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, " \t\r\n\a");
  }

  *(tokens + position) = NULL;
  return tokens;
}

int fsh_execute(char **args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("fsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("fsh");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int fsh_cd(char **args);
int fsh_exit(char **args);
int fsh_help(char **args);

char *builtin_str[] = {"cd", "exit", "help"};

int (*builtin_func[])(char **) = {&fsh_cd, &fsh_exit, &fsh_help};

int fsh_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

int fsh_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "fsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("fsh");
    }
  }

  return 1;
}

int fsh_exit(char **args) { return 0; }

int fsh_help(char **args) {
  int i;
  printf("Favour Alofe's FSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following commands are built in: \n");

  for (int i = 0; i < fsh_num_builtins(); i++) {
    printf("  %s\n", *(builtin_str + i));
  }

  printf("Use the 'man' command for information on other programs.\n");
  return 1;
}

void fsh_loop() {
  char *line;
  char **args;
  int status;

  do {
    line = fsh_read_line();
    args = fsh_parse_line(line);
    status = fsh_execute(args);
  } while (status);
}

int main(int argc, char **argv) {
  printf("Welcome to Favour's Shell (fsh)!\n");

  fsh_loop();

  exit(EXIT_SUCCESS);
}
