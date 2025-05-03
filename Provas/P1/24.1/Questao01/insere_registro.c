#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char nome[18];
  int quantidade;
  float custo;
} Registro;

int main(int argc, char** argv) {
  FILE *f = fopen("ferramentas.dat", "r+");
  Registro r1, r2;
  int linha;

  if (argc != 5) {
    fprintf(stderr, "Erro na chamada do comando. \n");
    fprintf(stderr, "Uso: %s [ID] [NOME] [QTD] [CUSTO]. \n", argv[0]);
    return 1;
  }

  linha = atoi(argv[1]);
  strcpy(r1.nome, argv[2]);
  r1.quantidade = atoi(argv[3]);
  r1.custo = atof(argv[4]);

  fseek(f, linha * sizeof(Registro), SEEK_SET);
  fread(&r2, sizeof(Registro), 1, f);

  printf("%s", r2.nome);
  if (strcmp(r2.nome, "")) {
    fprintf(stderr, "A linha %d já está preenchida.\n", linha);
    return 1;
  }

  fseek(f, -sizeof(Registro), SEEK_CUR);
  fwrite(&r1, sizeof(Registro), 1, f);

  fclose(f);
  return 0;
}
