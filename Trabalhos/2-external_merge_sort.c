#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char logradouro[72], bairro[72], cidade[72], uf[72], sigla[2], cep[8], lixo[2];
} Endereco;

FILE *f, *saida;
Endereco *e;
int NUM_PARTES = 8;

int compara(const void *e1, const void *e2) {
  return strncmp(((Endereco*) e1)->cep, ((Endereco*) e2)->cep, 8);
}

void intercala(char *f1, char *f2, char* fsaida) {
	printf("%s - %s\n", f1, f2);
	FILE *a, *b, *saida;
	Endereco ea, eb;

	a = fopen(f1, "rb");
	b = fopen(f2, "rb");
	saida = fopen(fsaida, "wb");

	fread(&ea,sizeof(Endereco), 1, a);
	fread(&eb,sizeof(Endereco), 1, b);

	while(!feof(a) && !feof(b)) {
		if (compara(&ea, &eb) < 0) {
			fwrite(&ea, sizeof(Endereco), 1, saida);
			fread(&ea, sizeof(Endereco), 1, a);
		} else {
			fwrite(&eb, sizeof(Endereco), 1, saida);
			fread(&eb, sizeof(Endereco), 1, b);
		}
	}

	while(!feof(a)) {
		fwrite(&ea, sizeof(Endereco), 1, saida);
		fread(&ea, sizeof(Endereco), 1, a);		
	}
	
	while(!feof(b))	{
		fwrite(&eb, sizeof(Endereco), 1, saida);
		fread(&eb, sizeof(Endereco), 1, b);		
	}

	fclose(a);
	fclose(b);
	remove(f1);
	remove(f2);
	fclose(saida);
}

void separaEOrdena() {
	char nomeSaida[20];
	long posicao, qtd, qtdParte, resto;

	fseek(f, 0, SEEK_END);
	posicao = ftell(f);
	qtd = posicao / sizeof(Endereco);
	qtdParte = qtd / NUM_PARTES;
	resto = qtd % NUM_PARTES;
	rewind(f);

	for (int i = 0; i < NUM_PARTES; i++) {
		if (i == NUM_PARTES - 1) {
			qtdParte += resto;
		}

		e = (Endereco*) malloc(qtdParte * sizeof(Endereco));

		fread(e, sizeof(Endereco), qtdParte, f);

		qsort(e, qtdParte, sizeof(Endereco), compara);

		sprintf(nomeSaida, "cep_%d.dat", i);
		saida = fopen(nomeSaida, "wb");
		fwrite(e, sizeof(Endereco), qtdParte, saida);

		fclose(saida);
		free(e);
	}
}

int main(int argc, char**argv) {
	f = fopen("cep.dat", "rb");
	
  if (!f) {
    fprintf(stderr, "Arquivo cep.dat não pode ser aberto para leitura.\n");
    return 1;
  }

  separaEOrdena();

	int qtdPartes = NUM_PARTES, prox = 0;
	char nomeSaida0[20], nomeSaida1[20], nomeSaida2[20];
	
	sprintf(nomeSaida0, "cep_%d.dat", prox);
	sprintf(nomeSaida1, "cep_%d.dat", prox + 1);
	sprintf(nomeSaida2, "cep_%d.dat", qtdPartes);
	while (qtdPartes <= (NUM_PARTES * 2) - 2) {
		intercala(nomeSaida0, nomeSaida1, nomeSaida2);
		qtdPartes++;
		prox += 2;

		sprintf(nomeSaida0, "cep_%d.dat", prox);
		sprintf(nomeSaida1, "cep_%d.dat", prox + 1);
		sprintf(nomeSaida2, "cep_%d.dat", qtdPartes);
	}

	rename(nomeSaida0, "cep_ordenado.dat");

	fclose(f);

	return 0;
}
