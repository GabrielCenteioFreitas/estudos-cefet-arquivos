#include <stdio.h>

int main() {
  char bloco[3] = {'\0'};
  FILE* f = fopen("letras.dat", "rb");
  
  if (f == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      return 1;
  }

  fseek(f, -2, SEEK_END);
  fread(bloco, sizeof(char), 3, f);
  printf("(a) %3.3s %ld %d\n", bloco, ftell(f), feof(f));

  fseek(f, -3, SEEK_END);
  fread(bloco, sizeof(char), 3, f);
  fread(bloco, sizeof(char), 3, f);
  printf("(b) %3.3s %ld %d\n", bloco, ftell(f), feof(f));

  fseek(f, 10, SEEK_SET);
  printf("(c) %3.3s %ld %d\n", bloco, ftell(f), feof(f));

  fread(bloco, sizeof(char), 3, f);
  printf("(d) %3.3s %ld %d\n", bloco, ftell(f), feof(f));

  fseek(f, -3, SEEK_CUR);
  fread(bloco, sizeof(char), 3, f);
  printf("(e) %3.3s %ld %d\n", bloco, ftell(f), feof(f));

  fclose(f);
  return 0;
}
