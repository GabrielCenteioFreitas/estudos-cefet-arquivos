#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char palavra[11];
  int qtd;
} Registro;

int compara(const void *e1, const void *e2) {
  return ((Registro*) e2)->qtd - ((Registro*) e1)->qtd;
}

int main(int argc, char** argv) {
  FILE *entrada, *saida;
  int c;

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

  saida = fopen("resultado.dat", "wb");
  if (!saida) {
    fclose(entrada);
    fprintf(stderr, "Arquivo %s não pode ser aberto para escrita. \n", "resultado.dat");
    return 1;
  }

  char palavra[11];
  int qtdPalavras = 0, achou = 0;
  Registro *registros = NULL;

  while (fscanf(entrada, " %[^ ]s", palavra) != -1) {
    achou = 0;
    for (int i = 0; i < qtdPalavras; i++) {
      if (!strcmp(palavra, registros[i].palavra)) {
        registros[i].qtd++;
        achou = 1;
      }
    }

    if (achou) continue;

    registros = realloc(registros, (qtdPalavras + 1) * sizeof(Registro));
    strcpy(registros[qtdPalavras].palavra, palavra);
    registros[qtdPalavras].qtd = 1;
    qtdPalavras++;
  }

  qsort(registros, qtdPalavras, sizeof(Registro), compara);

  fwrite(registros, sizeof(Registro), qtdPalavras, saida);

  free(registros);
  fclose(entrada);
  fclose(saida);

  return 0;
}