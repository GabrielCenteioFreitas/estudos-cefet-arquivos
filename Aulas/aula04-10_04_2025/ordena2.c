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
	qtd = posicao/sizeof(Endereco);
	metade = qtd/2;
	rewind(f);

	e = (Endereco*) malloc(metade*sizeof(Endereco));

	fread(e,sizeof(Endereco),metade,f);

	qsort(e,metade,sizeof(Endereco),compara);

	saida = fopen("cep_a.dat", "wb");
	fwrite(e,sizeof(Endereco),metade,saida);

	fclose(saida);
	free(e);

	e = (Endereco*) malloc((qtd-metade)*sizeof(Endereco));

	fread(e,sizeof(Endereco),qtd-metade,f);

	qsort(e,qtd-metade,sizeof(Endereco),compara);

	saida = fopen("cep_b.dat", "wb");
	fwrite(e,sizeof(Endereco),qtd-metade,saida);
	
	free(e);
	fclose(saida);
	fclose(f);

	return 0;
}
