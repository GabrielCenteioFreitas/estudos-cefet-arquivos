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
  FILE *f;
  Pagamento p;

  f = fopen("beneficios.dat", "rb");

  // (a)
  fseek(f, 4 * sizeof(Pagamento), SEEK_SET);
  fread(&p, sizeof(Pagamento), 1, f);

  // (b)
  fseek(f, 7 * sizeof(Pagamento), SEEK_SET);
  fread(&p, sizeof(Pagamento), 1, f);
  printf("%.12s - %.2f\n", p.cpf, p.valor);

  // (c)
  fseek(f, -3 * sizeof(Pagamento), SEEK_CUR);
  fread(&p, sizeof(Pagamento), 1, f);
  printf("%.80s - %.2f\n", p.nome, p.valor);

  // (d)
  fseek(f, -sizeof(Pagamento), SEEK_END);
  fread(&p, sizeof(Pagamento), 1, f);
  printf("%.12s - %.2f\n", p.cpf, p.valor);

  fclose(f);
  return 0;
}