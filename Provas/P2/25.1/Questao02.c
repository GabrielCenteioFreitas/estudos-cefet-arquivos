#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 11

int hashQnt = HASH_SIZE;

typedef struct _HashElement HashElement;

struct _HashElement {
  int chave;
  char valor;
  int next;
};

void acumula(HashElement tabela[], int chave, char valor) {
  int index = chave % HASH_SIZE;
  HashElement *x = &tabela[index];

  if (x->chave == -1) {
    x->chave = chave;
    x->valor = valor;
    x->next = 0;
    return;
  }

  tabela = realloc(tabela, (hashQnt+1) * sizeof(HashElement));
  
  HashElement y;
  y.chave = chave;
  y.valor = valor;
  y.next = x->next;
  x->next = hashQnt;
  tabela[hashQnt] = y;
  hashQnt++;
}

int main() {
  HashElement e, *tabela = malloc(HASH_SIZE * sizeof(HashElement));
  e.chave = -1;
  e.next = 0;
  e.valor = '\0';
  for (int i = 0; i < HASH_SIZE; i++) {
    tabela[i] = e;
  }

  acumula(tabela, 43, 'n');
  acumula(tabela, 27, 'Y');
  acumula(tabela, 70, 'h');
  acumula(tabela, 21, 'e');
  acumula(tabela, 34, '4');
  acumula(tabela, 84, 'd');
  acumula(tabela, 55, '/');
  acumula(tabela, 38, 'U');

  printf("   | Chave | Valor | Próx |\n");
  printf("---------------------------\n");
  for (int i = 0; i < hashQnt; i++) {
    if (tabela[i].chave > -1) {
      printf("%2d | %-5d | %-5c | %-4d |\n", i, tabela[i].chave, tabela[i].valor, tabela[i].next);
    } else {
      printf("%2d |       |       |      |\n", i);
    }
  }
}