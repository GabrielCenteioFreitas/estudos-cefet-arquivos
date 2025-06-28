#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 2048
#define QTD 5700

typedef struct {
  int index;
  int CODMUNRES;
  char data_nasc[11];
  int PESO;
  int SEMAGESTAC;
  float adeq_peso_id_gestac;
  int SEXO;
  int IDANOMAL;
  int RACACOR;
  int ESCMAEAGR1;
  int ESTCIVMAE;
  int IDADEMAE;
  int CONSULTAS;
  int PARTO;
  int STTRABPART;
  int KOTELCHUCK;
  int APGAR5;
  int TPROBSON;
} Registro;

typedef struct {
  unsigned int codigo;
  unsigned int posicao;
} Indice;

Registro* leRegistro(FILE* f) {
  static char buffer[MAX];
  static char* linha[18];
  static char* prox = NULL;

  if (!fgets(buffer, MAX, f)) return NULL;

  prox = NULL;
  linha[0] = strtok_r(buffer, ",\n", &prox);

  for (int i = 1; i < 18; i++) {
    linha[i] = strtok_r(NULL, ",\n", &prox);
    if (linha[i] == NULL) {
      linha[i] = "";
    }
  }

  Registro* dn = malloc(sizeof(Registro));
  if (!dn) return NULL;

  dn->index = atoi(linha[0]);
  dn->CODMUNRES = atoi(linha[1]);
  strncpy(dn->data_nasc, linha[2], 10);
  dn->data_nasc[10] = '\0'; // garante null-termination
  dn->PESO = atoi(linha[3]);
  dn->SEMAGESTAC = atoi(linha[4]);
  dn->adeq_peso_id_gestac = atof(linha[5]);
  dn->SEXO = atoi(linha[6]);
  dn->IDANOMAL = atoi(linha[7]);
  dn->RACACOR = atoi(linha[8]);
  dn->ESCMAEAGR1 = atoi(linha[9]);
  dn->ESTCIVMAE = atoi(linha[10]);
  dn->IDADEMAE = atoi(linha[11]);
  dn->CONSULTAS = atoi(linha[12]);
  dn->PARTO = atoi(linha[13]);
  dn->STTRABPART = atoi(linha[14]);
  dn->KOTELCHUCK = atoi(linha[15]);
  dn->APGAR5 = atoi(linha[16]);
  dn->TPROBSON = atoi(linha[17]);

  return dn;
}

unsigned int getCodMunicipio(Registro* r) {
  return (unsigned int) r->CODMUNRES;
}

int compara(const void *i1, const void *i2) {
  return ((Indice*)i1)->codigo - ((Indice*)i2)->codigo;
}

int main() {
  FILE *entrada, *saida;
  Registro *r;
  Indice *indices;
  unsigned int posicao, codigoAnt = -1, codigo;
  int i = 0;

	entrada = fopen("../../Aulas/_gerais/sinasc.csv", "rb");
	saida = fopen("indice.dat", "wb");

  indices = (Indice*) malloc(QTD * sizeof(Indice));

  posicao = ftell(entrada);
  r = leRegistro(entrada);
  while (r) {
    codigo = getCodMunicipio(r);

    if (codigo != codigoAnt) {
      indices[i].codigo = codigo;
      indices[i].posicao = posicao;
      i++;
    }

    codigoAnt = getCodMunicipio(r);
    posicao = ftell(entrada);
    r = leRegistro(entrada);
  }

  qsort(indices, QTD, sizeof(Indice), compara);

  fwrite(indices, sizeof(Indice), QTD, saida);

  fclose(entrada);
  fclose(saida);

  return 0;
}