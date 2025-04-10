#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char logradouro[72], bairro[72], cidade[72], uf[72], sigla[2], cep[8], lixo[2];
} Endereco;

int compara(const void *e1, const void *e2) {
  return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main(int argc, char** argv) {
	FILE *a, *b, *saida;
	Endereco ea, eb;

	a = fopen("cep_a.dat", "rb");
	b = fopen("cep_b.dat", "rb");
	saida = fopen("cep_ordenado.dat", "wb");

	fread(&ea,sizeof(Endereco), 1, a);
	fread(&eb,sizeof(Endereco), 1, b);

	while(!feof(a) && !feof(b)) {
		if (compara(&ea, &eb) < 0) {
			fwrite(&ea,sizeof(Endereco),1,saida);
			fread(&ea,sizeof(Endereco),1,a);
		} else {
			fwrite(&eb,sizeof(Endereco),1,saida);
			fread(&eb,sizeof(Endereco),1,b);
		}
	}

	while(!feof(a)) {
		fwrite(&ea,sizeof(Endereco),1,saida);
		fread(&ea,sizeof(Endereco),1,a);		
	}
	
	while(!feof(b))	{
		fwrite(&eb,sizeof(Endereco),1,saida);
		fread(&eb,sizeof(Endereco),1,b);		
	}

	fclose(a);
	fclose(b);
	fclose(saida);
}
