#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char cpf[12];
  char nome[80];
  int mes;
  int ano;
  float valor;
} Pagamento;

float total_pago_mes_ano(int mes, int ano) {
  FILE *f;
  int inicio = 1, fim, meio, chave1, chave2, posicao = -1;
  float valorTotal = 0;
  Pagamento p;

  f = fopen("beneficios.dat", "rb");

  chave1 = ano * 100 + mes;

  fseek(f, 0, SEEK_END);
  fim = ftell(f) / sizeof(Pagamento);
  meio = (inicio + fim) / 2;
  fseek(f, meio * sizeof(Pagamento), SEEK_SET);

  fread(&p, sizeof(Pagamento), 1, f);
  while (inicio <= fim) {
    chave2 = p.ano * 100 + p.mes;
    if (chave1 == chave2) {
      posicao = ftell(f) / sizeof(Pagamento) - 1;
      break;
    } else if (chave1 > chave2) {
      inicio = meio + 1;
    } else {
      fim = meio - 1;
    }

    meio = (inicio + fim) / 2;
    fseek(f, meio * sizeof(Pagamento), SEEK_SET);
    fread(&p, sizeof(Pagamento), 1, f);
  }

  if (posicao == -1) return valorTotal;

  while (!feof(f) && chave1 == chave2) {
    valorTotal += p.valor;
    fread(&p, sizeof(Pagamento), 1, f);
    if (!feof(f)) {
      chave2 = p.ano * 100 + p.mes;
    }
  }

  posicao--;
  if (posicao == -1) return valorTotal;

  fseek(f, posicao * sizeof(Pagamento), SEEK_SET);
  fread(&p, sizeof(Pagamento), 1, f);
  chave2 = p.ano * 100 + p.mes;
    
  while (posicao >= 0 && chave1 == chave2) {
    valorTotal += p.valor;
    posicao--;
    if (posicao >= 0) {
      fseek(f, posicao * sizeof(Pagamento), SEEK_SET);
      fread(&p, sizeof(Pagamento), 1, f);
      chave2 = p.ano * 100 + p.mes;
    }
  }

  fclose(f);
  return valorTotal;
}

int main() {
  printf("Total pago: %.2f\n", total_pago_mes_ano(5, 2025));

  return 0;
}