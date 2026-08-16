#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define TOK_SIZE 64

char *fsh_read_line() {
  printf(">>> ");
  fflush(stdout);

  int position = 0;
  int c;
  int buffer_size = BUFF_SIZE;
  char *buffer = malloc(sizeof(char) * buffer_size);

  if (!buffer) {
    fprintf(stderr, "fsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();

    if (c == EOF) {
      free(buffer);
      exit(EXIT_SUCCESS);
    }

    if (c == '\n') {
      *(buffer + position) = '\0';
      return buffer;
    }

    *(buffer + position) = c;
    position++;

    if (position >= buffer_size) {
      buffer_size += BUFF_SIZE;
      buffer = realloc(buffer, buffer_size);

      if (!buffer) {
        fprintf(stderr, "fsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **fsh_parse_line(char *line) {
  int token_size = TOK_SIZE;
  int position = 0;
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

    if (position >= token_size - 1) {
      token_size += TOK_SIZE;
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

int fsh_launch(char **args) {
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
int fsh_pwd(char **args);
// int fsh_echo(char **args);
int fsh_help(char **args);
int fsh_exit(char **args);

char *builtin_str[] = {"cd", "pwd", "help", "exit"};

int fsh_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

int (*builtin_func[])(char **args) = {&fsh_cd, &fsh_pwd, &fsh_help, &fsh_exit};

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

int fsh_pwd(char **args) {
  int buffer_size = BUFF_SIZE;
  char *buffer = malloc(sizeof(char) * buffer_size);

  if (!buffer) {
    fprintf(stderr, "fsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  if (getcwd(buffer, buffer_size) != NULL) {
    printf("%s\n", buffer);
  } else {
    perror("fsh: pwd");
  }

  free(buffer);

  return 1;
}

// fsh_echo() {}

int fsh_help(char **args) {
  int i;
  printf("Favour's Shell (fsh)!\n");
  printf("Type program names and arguments, and hit enter.\n");

  printf("The following commands are built-in: \n");
  for (i = 0; i < fsh_num_builtins(); i++) {
    printf("  %s\n", *(builtin_str + i));
  }

  printf("Use the 'man' command for information on other programs.\n");

  return 1;
}

int fsh_exit(char **args) { return 0; }

int fsh_execute(char **args) {
  if (args[0] == NULL) {
    return 1;
  }

  for (int i = 0; i < fsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return fsh_launch(args);
}

void fsh_loop() {
  char *line;
  char **args;
  int status;

  do {
    line = fsh_read_line();
    args = fsh_parse_line(line);
    status = fsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

int main(int argc, char **argv) {
  printf("Welcome Favour's Shell! Enjoy your stay! \n");

  fsh_loop();

  exit(EXIT_SUCCESS);
}
