#include <stdio.h>
#include <string.h>

typedef struct {
	char logradouro[72], bairro[72], cidade[72], uf[72], sigla[2], cep[8], lixo[2];
} Endereco;

typedef struct {
  char cep[8];
  long posicao;
} Indice;

int main(int argc, char **argv)  {
  FILE *entrada, *indices;
  Indice indice;
	Endereco endereco;
	int inicio = 0, fim, meio, comparacao;

	if(argc != 2) {
    fprintf(stderr, "Erro na chamada do comando. \n");
		fprintf(stderr, "Uso: %s [CEP]\n", argv[0]);
		return 1;
	}

	entrada = fopen("cep.dat", "rb");
	
  if (!entrada) {
    fprintf(stderr, "Arquivo cep.dat não pode ser aberto para leitura.\n");
    return 1;
  }

  indices = fopen("indice.dat", "rb");
	
  if (!indices) {
    fprintf(stderr, "Arquivo indice.dat não pode ser aberto para leitura.\n");
    return 1;
	}

	fseek(indices, 0, SEEK_END);
	fim = ftell(indices) / sizeof(indice);
	meio = (inicio + fim) / 2;
	fseek(indices, meio * sizeof(indice), SEEK_SET);

	fread(&indice, sizeof(Indice), 1, indices);
	while (inicio <= fim) {
		comparacao = strncmp(argv[1], indice.cep, 8);

		if(comparacao == 0) {
      fseek(entrada, indice.posicao * sizeof(Endereco), SEEK_SET);
      fread(&endereco, sizeof(Endereco), 1, entrada);

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
		fseek(indices, meio * sizeof(Indice), SEEK_SET);
		fread(&indice, sizeof(indice), 1, indices);
	}

	fclose(entrada);
	fclose(indices);

	return 0;
}
