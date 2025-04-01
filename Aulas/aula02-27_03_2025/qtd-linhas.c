#include <stdio.h>

int main(int argc, char** argv) {
  FILE *entrada;
  int c, qtdLinhas = 1;

  if (argc != 2) {
    fprintf(stderr, "Erro na chamada do comando. \n");
    fprintf(stderr, "Uso: %s [ARQUIVO ORIGEM]. \n", argv[0]);
    return 1;
  }

  entrada = fopen(argv[1], "rb");
  if (!entrada) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para leitura. \n", argv[1]);
    return 1;
  }

  c = fgetc(entrada);
  while(c != EOF) {
    if (c == '\n') {
      qtdLinhas++;
    }

    c = fgetc(entrada);
  }

  fclose(entrada);

  fprintf(stdout, "Quantidade de linhas: %d\n", qtdLinhas);

  return 0;
}