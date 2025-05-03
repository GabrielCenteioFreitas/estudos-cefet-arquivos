#include <stdio.h>
#include <string.h>

typedef struct {
  char nome[18];
  int quantidade;
  float custo;
} Registro;

int main() {
  FILE *f = fopen("ferramentas.dat", "wb");
  Registro r;

  strcpy(r.nome, "");
  r.quantidade = 0;
  r.custo = 0;

  for (int i = 0; i < 100; i++) {
    fwrite(&r, sizeof(Registro), 1, f);
  }

  fclose(f);
  return 0;
}
