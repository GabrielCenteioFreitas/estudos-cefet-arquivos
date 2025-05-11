#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char cep[8];
  unsigned int posicao;
} IndiceCEP;

int compara(const void *i1, const void *i2) {
  return strncmp(((IndiceCEP*) i1)->cep, ((IndiceCEP*) i2)->cep, 8);
}

int main() {
  FILE *f1, *f2;
  int qnt = 0;
  IndiceCEP i, *is = NULL;

  f1 = fopen("indice-cep.dat", "rb");
  if (!f1) {
    fprintf(stderr, "Arquivo indice-cep.dat não pode ser aberto para leitura. \n");
    return 1;
  }

  f2 = fopen("indice-cep-ordenado.dat", "wb");
  if (!f2) {
    fprintf(stderr, "Arquivo indice-cep-ordenado.dat não pode ser aberto para escrita. \n");
    return 1;
  }

  fread(&i, sizeof(IndiceCEP), 1, f1);
  while (!feof(f1)) {
    is = realloc(is, (++qnt) * sizeof(IndiceCEP));
    is[qnt - 1] = i;

    fread(&i, sizeof(IndiceCEP), 1, f1);
  }

  qsort(is, qnt, sizeof(IndiceCEP), compara);
  fwrite(is, sizeof(IndiceCEP), qnt, f2);

  fclose(f1);
  fclose(f2);

  return 0;
}