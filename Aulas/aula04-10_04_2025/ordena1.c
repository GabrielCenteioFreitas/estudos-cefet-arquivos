#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char logradouro[72], bairro[72], cidade[72], uf[72], sigla[2], cep[8], lixo[2];
} Endereco;

int compara(const void *e1, const void *e2) {
  return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main(int argc, char**argv) {
	FILE *f, *saida;
	Endereco *e;
	long posicao, qtd, metade;

	f = fopen("../_gerais/cep.dat", "rb");

	fseek(f, 0, SEEK_END);
	posicao = ftell(f);
	qtd = posicao / sizeof(Endereco);
	rewind(f);

	e = (Endereco*) malloc(qtd * sizeof(Endereco));

	fread(e, sizeof(Endereco), qtd, f);

	qsort(e, qtd, sizeof(Endereco), compara);

	saida = fopen("cep_ordenado.dat", "wb");
	fwrite(e, sizeof(Endereco), qtd, saida);

	fclose(saida);
	free(e);

	return 0;
}

