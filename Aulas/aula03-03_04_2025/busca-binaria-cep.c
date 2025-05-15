#include <stdio.h>
#include <string.h>

typedef struct {
	char logradouro[72], bairro[72], cidade[72], uf[72], sigla[2], cep[8], lixo[2];
} Endereco;

int main(int argc, char **argv)  {
  FILE *entrada;
	Endereco endereco;
	int inicio = 0, fim, meio, comparacao;

	if(argc != 2) {
    fprintf(stderr, "Erro na chamada do comando. \n");
		fprintf(stderr, "Uso: %s [CEP]\n", argv[0]);
		return 1;
	}

	entrada = fopen("cep_ordenado.dat", "rb");
	
  if (!entrada) {
    fprintf(stderr, "Arquivo %s não pode ser aberto para leitura. \n", argv[1]);
    return 1;
  }

	fseek(entrada, 0, SEEK_END);
	fim = ftell(entrada) / sizeof(Endereco);
	meio = (inicio + fim) / 2;
	fseek(entrada, meio * sizeof(Endereco), SEEK_SET);

	fread(&endereco, sizeof(Endereco), 1, entrada);
	while (inicio <= fim) {
		comparacao = strncmp(argv[1], endereco.cep, 8);

		if(comparacao == 0) {
			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",
				endereco.logradouro, 
				endereco.bairro,
				endereco.cidade,
				endereco.uf,
				endereco.sigla,
				endereco.cep
			);
			break;
		} else if (comparacao > 0) {
			inicio = meio + 1;
		} else {
			fim = meio - 1;
		}

		meio = (inicio + fim) / 2;
		fseek(entrada, meio * sizeof(Endereco), SEEK_SET);
		fread(&endereco, sizeof(Endereco), 1, entrada);
	}

	fclose(entrada);

	return 0;
}

