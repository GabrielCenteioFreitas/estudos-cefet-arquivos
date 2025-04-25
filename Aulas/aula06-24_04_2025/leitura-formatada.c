#include <stdio.h>
#include <string.h>

#define MAX_LINE 4096
#define DELIM ";\n"

int main() {
  FILE *file = fopen("dados.csv", "r");

  if (file == NULL) {
    fprintf(stderr, "O arquivo CSV não pode ser aberto para leitura. \n");
    return 1;
  }

  int qtdColunas = 0;
  char line[MAX_LINE];
  fgets(line, sizeof(line), file);
  char *saveptr, *token = strtok_r(line, DELIM, &saveptr);

  while (token) {
    if (token[0] >= 97) token[0] -= 32;
    qtdColunas++;
    printf("%-20s", token);
    token = strtok_r(NULL, DELIM, &saveptr);
    printf(" | ");
  }

  printf("\n");
  for (int i = 0; i < qtdColunas; i++) {
    for (int j = 0; j < 20; j++) {
      printf("-");
    }
    printf(" | ");
  }
  printf("\n");

  while (fgets(line, sizeof(line), file)) {
    token = strtok_r(line, DELIM, &saveptr);
    
    while (token) {
      printf("%-20s", token);
      token = strtok_r(NULL, DELIM, &saveptr);
      printf(" | ");
    }

    printf("\n");
  }

  fclose(file);
  return 0;
}
