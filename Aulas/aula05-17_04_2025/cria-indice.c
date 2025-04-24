#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char logradouro[72], bairro[72], cidade[72], uf[72], sigla[2], cep[8], lixo[2];
} Endereco;

typedef struct {
  char cep[8];
  long posicao;
} Indice;

int compara(const void *i1, const void *i2) {
  return strncmp(((Indice*)i1)->cep, ((Indice*)i2)->cep, 8);
}

int main(int argc, char** argv) {
	FILE *entrada, *saida;
	Endereco e;
  Indice *indices;
  long posicao, qtd;

	entrada = fopen("../_gerais/cep.dat", "rb");
	saida = fopen("indice.dat", "wb");

  fseek(entrada, 0, SEEK_END);
	posicao = ftell(entrada);
	qtd = posicao / sizeof(Endereco);
	rewind(entrada);

  indices = (Indice*) malloc(qtd * sizeof(Indice));

  for (long i = 0; i < qtd; i++) {
    fread(&e, sizeof(Endereco), 1, entrada);
    strncpy(indices[i].cep, e.cep, 8);
    indices[i].posicao = i;
  }

  qsort(indices, qtd, sizeof(Indice), compara);

  fwrite(indices, sizeof(Indice), qtd, saida);
  
	fclose(entrada);
	fclose(saida);

  return 0;
}
