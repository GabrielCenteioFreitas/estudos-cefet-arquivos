#include <stdio.h>

int main(int argc, char** argv) {
  FILE *f;
  int c, qtdAbertos = 0;

  if (argc != 2) {
    fprintf(stderr, "Erro na chamada do comando. \n");
    fprintf(stderr, "Uso: %s [ARQUIVO]. \n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "rb");
  if (!f) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para leitura. \n", argv[1]);
    return 1;
  }

  c = fgetc(f);
  while(!feof(f)) {
    if (c == '{') {
      qtdAbertos++;
    } else if (c == '}') {
      qtdAbertos--;
    }

    if (qtdAbertos < 0) break;

    c = fgetc(f);
  }

  if (qtdAbertos < 0) {
    fprintf(stdout, "As chaves não estão balanceadas!\n");
  } else {
    fprintf(stdout, "Chaves balanceadas!\n");
  }

  fclose(f);

  return 0;
}