#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  int c, qtds[256] = {0};

  if (argc != 2) {
    fprintf(stderr, "Erro na chamada do comando.\n");
    fprintf(stderr, "Uso: %s [ARQUIVO ORIGEM].\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "rb");
  if (!f) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para leitura.\n", argv[1]);
    return 1;
  }

  c = fgetc(f);
  while (!feof(f)) {
    qtds[c]++;
    c = fgetc(f);
  }

  fclose(f);

  return 0;
}