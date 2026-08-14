#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fsh_read_line() {
  int string_size = 1024;
  char *buffer = malloc(sizeof(char) * string_size);
  int position = 0;
  int c;

  if (!buffer) {
    fprintf(stderr, "fsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while ((c = getchar()) != EOF || c != '\n') {
    if (position >= string_size) {
      string_size += string_size;
      buffer = realloc(buffer, sizeof(char) * string_size);

      if (!buffer) {
        fprintf(stderr, "fsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    *(buffer + position) = c;
    position++;
  }

  *(buffer + position) = '\0';
  return buffer;
}

char **fsh_parse_line(char *line) {
  int size = 64;
  int position = 0;
  char **tokens = malloc(sizeof(char *) * size);
  char *token;

  if (!tokens) {
    fprintf(stderr, "fsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, "\t\r\n\a");
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= size) {
      size += size;
      tokens = realloc(tokens, sizeof(char *) * size);

      if (!tokens) {
        fprintf(stderr, "fsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, "\t\r\n\a");
  }

  tokens[position] = NULL;

  return tokens;
}

void fsh_loop() {
  printf(">>> ");
  char *string;
  string = fsh_read_line();
  char **line = fsh_parse_line(string);
}

int main(int argc, char **argv) {
  // configuration files

  printf("Welcome to Favour's Shell!\n");

  fsh_loop();

  return EXIT_SUCCESS;
}
