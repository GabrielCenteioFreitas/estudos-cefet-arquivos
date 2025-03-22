#include <stdio.h>

#define TAMANHO 8192 // 8KB

int main(int argc, char* *argv) {
  FILE *entrada, *saida;
  char buffer[TAMANHO];
  int qtd;

  if (argc != 3) {
    fprintf(stderr, "Erro na chamada do comando. \n");
    fprintf(stderr, "Uso: %s [ARQUIVO ORIGEM] [ARQUIVO DESTINO]. \n", argv[0]);
    return 1;
  }

  entrada = fopen(argv[1], "rb");
  if (!entrada) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para leitura. \n", argv[1]);
    return 1;
  }
  
  saida = fopen(argv[2], "wb");
  if (!saida) {
    fclose(entrada);
    fprintf(stderr, "Arquivo %s não pode ser aberto para escrita. \n", argv[2]);
    return 1;
  }

  qtd = fread(buffer, sizeof(char), TAMANHO, entrada);
  while (qtd > 0) {
    fwrite(buffer, sizeof(char), qtd, saida);
    qtd = fread(buffer, sizeof(char), TAMANHO, entrada);
  }

  fclose(entrada);
  fclose(saida);

  return 0;
}