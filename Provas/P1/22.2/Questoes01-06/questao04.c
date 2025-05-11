#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char logradouro[72], bairro[72], cidade[72], uf[72], sigla[2], cep[8], lixo[2];
} Endereco;

typedef struct {
  char cep[8];
  unsigned int posicao;
} IndiceCEP;

int compara(const void *i1, const void *i2) {
  return strncmp(((IndiceCEP*) i1)->cep, ((IndiceCEP*) i2)->cep, 8);
}

int main(int argc, char** argv) {
  FILE *f1, *f2;
  int qnt = 0, inicio = 0, fim, meio, comparacao;
  Endereco e;
  IndiceCEP i, *is = NULL;

	if(argc != 2) {
    fprintf(stderr, "Erro na chamada do comando. \n");
		fprintf(stderr, "Uso: %s [CEP]\n", argv[0]);
		return 1;
	}

  f1 = fopen("indice-cep-ordenado.dat", "rb");
  if (!f1) {
    fprintf(stderr, "Arquivo indice-cep-ordenado.dat não pode ser aberto para leitura. \n");
    return 1;
  }

  f2 = fopen("../../../../Aulas/_gerais/cep.dat", "rb");
  if (!f2) {
    fprintf(stderr, "Arquivo cep.dat não pode ser aberto para escrita. \n");
    return 1;
  }

	fseek(f1, 0, SEEK_END);
	fim = ftell(f1) / sizeof(IndiceCEP);
	meio = (inicio + fim) / 2;
	fseek(f1, meio * sizeof(IndiceCEP), SEEK_SET);

  fread(&i, sizeof(IndiceCEP), 1, f1);
  while (!feof(f1)) {
    comparacao = strncmp(argv[1], i.cep, 8);

		if(comparacao == 0) {
      fseek(f2, i.posicao * sizeof(Endereco), SEEK_SET);
      fread(&e, sizeof(Endereco), 1, f2);

			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",
				e.logradouro,
				e.bairro,
				e.cidade,
				e.uf,
				e.sigla,
				e.cep
			);
			break;
		} else if (comparacao > 0) {
			inicio = meio + 1;
		} else {
			fim = meio - 1;
		}

		meio = (inicio + fim) / 2;
		fseek(f1, meio * sizeof(IndiceCEP), SEEK_SET);
    fread(&i, sizeof(IndiceCEP), 1, f1);
  }

  fclose(f1);
  fclose(f2);

  return 0;
}