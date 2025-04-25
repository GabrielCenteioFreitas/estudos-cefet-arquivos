#include <stdio.h>
#include <string.h>

#define MAX_LINE 4096

int main() {
  FILE *file = fopen("dados.csv", "r");

  if (file == NULL) {
    fprintf(stderr, "O arquivo CSV não pode ser aberto para leitura. \n");
    return 1;
  }

  char line[MAX_LINE], *saveptr, *token;

  while (fgets(line, sizeof(line), file)) {
    token = strtok_r(line, ";\n", &saveptr);
    
    while (token) {
      printf("%-20s", token);
      token = strtok_r(NULL, ";\n", &saveptr);
      if (token) printf(" | ");
    }

    printf("\n");
  }

  fclose(file);
  return 0;
}
