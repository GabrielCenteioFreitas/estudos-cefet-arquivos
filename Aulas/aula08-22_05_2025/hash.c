#include <stdio.h>
#include <stdlib.h>

typedef struct _HashElement HashElement;

struct _HashElement {
  int uf;
  float soma;
  int qtd;
  HashElement *next;
};

void acumula(HashElement tabela[], int uf, float peso) {
  int pos = uf % 31;
  HashElement *x = &tabela[pos];

  while (x) {
    if (x->uf == uf) {
      x->soma += peso;
      x->qtd++;
      return;
    }
    x = x->next;
  }

  x = (HashElement*) malloc(sizeof(HashElement));
  x->uf = uf;
  x->qtd = 1;
  x->soma = peso;
  if (tabela[pos].uf == 0) {
    x->next = NULL;
    tabela[pos] = *x;
  } else {
    x->next = tabela[pos].next;
    tabela[pos].next = x;
  }
}

int main() {
  HashElement tabela[31], e;
  e.qtd = 0;
  e.soma = 0;
  e.uf = 0;
  e.next = NULL;
  for (int i = 0; i < 31; i++) {
    tabela[i] = e;
  }

  acumula(tabela, 1, 10);
  acumula(tabela, 32, 20);

  for (int i = 0; i < 31; i++) {
    printf("%d - %2.f - %d\n", tabela[i].uf, tabela[i].soma, tabela[i].qtd);
  }
}