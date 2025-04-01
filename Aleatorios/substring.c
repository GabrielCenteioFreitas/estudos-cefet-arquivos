#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

int main(int argc, char** argv) {
  FILE *entrada;
  int c, estaPresente = 0, caracteresIguais = 0;
  char *stringProcurada;

  if (argc < 3) {
    fprintf(stderr, "Erro na chamada do comando. \n");
    fprintf(stderr, "Uso: %s [ARQUIVO ORIGEM] [STRING]. \n", argv[0]);
    return 1;
  }

  entrada = fopen(argv[1], "rb");
  if (!entrada) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para leitura. \n", argv[1]);
    return 1;
  }

  stringProcurada = (char *) malloc((strlen(argv[2]) + 1) * sizeof(char));
  strcpy(stringProcurada, argv[2]);
  for (int i = 3; i < argc; i++) {
    stringProcurada = (char *) realloc(stringProcurada, strlen(stringProcurada) + strlen(" ") + strlen(argv[i]));
    strcat(stringProcurada, " ");
    strcat(stringProcurada, argv[i]);
  }

  c = fgetc(entrada);
  while(c != EOF) {
    if (c == stringProcurada[caracteresIguais]) {
      caracteresIguais++;
    } else {
      if (caracteresIguais == strlen(stringProcurada)) {
        estaPresente = 1;
        break;
      }

      caracteresIguais = 0;
    }

    c = fgetc(entrada);
  }

  fclose(entrada);

  if (estaPresente) {
    fprintf(stdout, "%s", GREEN);
    fprintf(stdout, "A string \"%s\" está presente no arquivo.\n", stringProcurada);
  } else {
    fprintf(stdout, "%s", RED);
    fprintf(stdout, "A string \"%s\" não está presente no arquivo.\n", stringProcurada);
  }
  fprintf(stdout, "%s", RESET);

  return 0;
}