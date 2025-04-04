#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco {
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2]; // espaço no final da linha + quebra de linha
};

int main(int argc, char **argv) {
	FILE *f;
	Endereco e;
	int qt, contador;

	if(argc != 2) {
		fprintf(stderr, "Uso: %s [CEP]\n", argv[0]);
		return 1;
	}

	contador = 0;
	printf("Tamanho da Estrutura: %ld\n", sizeof(Endereco));
	f = fopen("../_gerais/cep.dat","rb");

	qt = fread(&e, sizeof(Endereco), 1, f);
	while(qt > 0)	{
		contador++;
		// argv[1] < e.cep  => strcmp(argv[1],e.cep) < 0
		// argv[1] > e.cep  => strcmp(argv[1],e.cep) > 0
		// argv[1] == e.cep  => strcmp(argv[1],e.cep) == 0
		if(strncmp(argv[1], e.cep, 8) == 0) {
			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);
			break;
		}

		qt = fread(&e, sizeof(Endereco), 1, f);		
	}

	printf("Total Lido: %d\n", contador);

	fclose(f);
	return 0;
}

