#include <stdio.h>
#include <string.h>

#ifdef _WIN32
char* strtok_r(char *str, const char *delim, char **saveptr) {
  char *token;
  if (str == NULL) {
    str = *saveptr;
  }
  while (*str && strchr(delim, *str)) {
    str++;
  }
  if (*str == '\0') {
    *saveptr = str;
    return NULL;
  }
  token = str;
  while (*str && !strchr(delim, *str)) {
    str++;
  }
  if (*str) {
    *str = '\0';
    *saveptr = str + 1;
  } else {
    *saveptr = str;
  }
  return token;
}
#endif

#define MAX_LINE 4096

int main() {
  FILE *file = fopen("dados.csv", "r");

  if (file == NULL) {
    fprintf(stderr, "O arquivo CSV não pode ser aberto para leitura. \n");
    return 1;
  }

  char line[MAX_LINE], *saveptr, *token;
  int i = 0;

  while (fgets(line, sizeof(line), file)) {
    token = strtok_r(line, ";\n", &saveptr);

    if (i == 1) printf("---------------------|----------------------|----------------------|----------------\n");
    
    while (token) {
      if (!i && token[0] >= 97) token[0] -= 32; 
      printf("%-20s", token);
      token = strtok_r(NULL, ";\n", &saveptr);
      if (token) printf(" | ");
    }

    printf("\n");
    i++;
  }

  fclose(file);
  return 0;
}
