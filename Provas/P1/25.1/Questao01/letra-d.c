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
  int qtdNovos;
  Pagamento p, maior, menor;

  f = fopen("beneficios.dat", "rb");

  fread(&p, sizeof(Pagamento), 1, f);
  maior = p;
  menor = p;
  while (!feof(f)) {
    if (p.ano != maior.ano) {
      printf("%d\n", maior.ano);
      printf("Maior: %.12s - %.80s - %d - %.2f\n", maior.cpf, maior.nome, maior.mes, maior.valor);
      printf("Menor: %.12s - %.80s - %d - %.2f\n", menor.cpf, menor.nome, menor.mes, menor.valor);
      maior = p;
      menor = p;
    } else {
      if (p.valor > maior.valor) {
        maior = p;
      } else if (p.valor < menor.valor) {
        menor = p;
      }
    }

    fread(&p, sizeof(Pagamento), 1, f);
  }

  printf("%d\n", maior.ano);
  printf("Maior: %.12s - %.80s - %d - %.2f\n", maior.cpf, maior.nome, maior.mes, maior.valor);
  printf("Menor: %.12s - %.80s - %d - %.2f\n", menor.cpf, menor.nome, menor.mes, menor.valor);

  fclose(f);
  return 0;
}