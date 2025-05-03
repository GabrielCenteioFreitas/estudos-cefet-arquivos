#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char nome[18];
  int quantidade;
  float custo;
} Registro;

int main() {
  FILE *f = fopen("ferramentas.dat", "rb");
  Registro r;
  int qtdLida = 0, qtdTotal = 0;

  fread(&r, sizeof(Registro), 1, f);
  while (!feof(f)) {
    if (strcmp(r.nome, "")) {
      qtdLida++;
      qtdTotal += r.quantidade;
    }

    fread(&r, sizeof(Registro), 1, f);
  }

  fprintf(stdout, "Qtd lida: %d\n", qtdLida);
  fprintf(stdout, "Qtd total: %d\n", qtdTotal);

  fclose(f);
  return 0;
}
