#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char palavra[11];
  int qtd;
} Registro;

int main(int argc, char** argv) {
  FILE *f1, *f2, *saida;
  int c;

  f1 = fopen("resultado.dat", "rb");
  if (!f1) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para leitura. \n", "resultado.dat");
    return 1;
  }

  f2 = fopen("stopwords.dat", "rb");
  if (!f2) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para leitura. \n", "stopwords.dat");
    return 1;
  }

  saida = fopen("resultado2.dat", "wb");
  if (!saida) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para escrita. \n", "resultado2.dat");
    return 1;
  }

  char palavra[11], stopword[100];
  int qtdPalavras = 0, achou = 0;
  Registro registro;
	int qtd, inicio = 0, fim, meio, comparacao;

	fread(&registro, sizeof(Registro), 1, f1);
  while (!feof(f1)) { 
    achou = 0;

    while(fscanf(f2, " %[^ ]s", stopword) != -1) {
      if (!strcmp(stopword, registro.palavra)) {
        achou = 1;
        break;
      }
    }

    if (!achou) {
      fwrite(&registro, sizeof(Registro), 1, saida);
    }
    fread(&registro, sizeof(Registro), 1, f1);
  }

  fclose(f1);
  fclose(f2);
  fclose(saida);

  return 0;
}