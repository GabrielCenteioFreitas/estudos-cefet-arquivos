#include <stdio.h>

int main(int argc, char** argv) {
  FILE *f1, *f2;
  int c;

  if (argc != 2) {
    fprintf(stderr, "Erro na chamada do comando. \n");
    fprintf(stderr, "Uso: %s [ARQUIVO ORIGEM].\n", argv[0]);
    return 1;
  }

  f1 = fopen(argv[1], "rb");
  if (!f1) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para leitura. \n", argv[1]);
    return 1;
  }

  f2 = fopen("dos.txt", "wb");
  if (!f2) {
    fprintf(stderr, "Arquivo dos.txt não pode ser aberto para escrita. \n");
    return 1;
  }

  c = fgetc(f1);
  while (!feof(f1)) {
    if (c == '\n') {
      fputc('\r', f2);
    }

    fputc(c, f2);

    c = fgetc(f1);
  }

  fclose(f1);
  fclose(f2);

  return 0;
}