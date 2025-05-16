#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char cpf[12];
  char nome[80];
  int mes;
  int ano;
  float valor;
} Pagamento;

int main() {
  FILE *f1, *f2;
  int qtdNovos;
  Pagamento *novos;

  f1 = fopen("beneficios-copy.dat", "r+");
  f2 = fopen("novos.dat", "rb");

  fseek(f2, 0, SEEK_END);
  qtdNovos = ftell(f2) / sizeof(Pagamento);
  rewind(f2);

  novos = (Pagamento*) malloc(qtdNovos * sizeof(Pagamento));

  fread(novos, sizeof(Pagamento), qtdNovos, f2);

  fseek(f1, 0, SEEK_END);
  fwrite(novos, sizeof(Pagamento), qtdNovos, f1);

  fclose(f1);
  fclose(f2);
  return 0;
}