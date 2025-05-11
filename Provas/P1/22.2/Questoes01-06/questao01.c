#include <stdio.h>
#include <string.h>

typedef struct {
  char logradouro[72], bairro[72], cidade[72], uf[72], sigla[2], cep[8], lixo[2];
} Endereco;

typedef struct {
  char cep[8];
  unsigned int posicao;
} IndiceCEP;

int main() {
  FILE *f1, *f2;
  Endereco e;
  IndiceCEP i;

  f1 = fopen("../../../../Aulas/_gerais/cep.dat", "rb");
  if (!f1) {
    fprintf(stderr, "Arquivo cep.dat não pode ser aberto para leitura. \n");
    return 1;
  }

  f2 = fopen("indice-cep.dat", "wb");
  if (!f2) {
    fprintf(stderr, "Arquivo indice-cep.dat não pode ser aberto para escrita. \n");
    return 1;
  }

  fread(&e, sizeof(Endereco), 1, f1);
  while (!feof(f1)) {
    strncpy(i.cep, e.cep, 8);
    i.posicao = (ftell(f1) / sizeof(Endereco)) - 1;

    fwrite(&i, sizeof(IndiceCEP), 1, f2);

    fread(&e, sizeof(Endereco), 1, f1);
  }

  fclose(f1);
  fclose(f2);

  return 0;
}