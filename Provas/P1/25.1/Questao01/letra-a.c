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
  long tamanho;
  int numTotal;

  f = fopen("beneficios.dat", "rb");
  fseek(f, 0, SEEK_END);

  tamanho = ftell(f);
  numTotal = tamanho / sizeof(Pagamento);

  printf("Tamanho em bytes: %ld\n", tamanho);
  printf("Número total de registros: %d\n", numTotal);

  fclose(f);
  return 0;
}