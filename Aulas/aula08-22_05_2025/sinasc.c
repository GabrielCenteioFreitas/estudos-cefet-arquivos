#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 2048

typedef struct _HashElement HashElement;

struct _HashElement {
  int uf;
  float soma;
  int qtd;
  HashElement *next;
};

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
} DadosNascimento;

void separaData(char* data, int *dia, int *mes, int *ano) {
  *ano = atoi(strtok(data,"-"));
  *mes = atoi(strtok(NULL,"-"));
  *dia = atoi(strtok(NULL,"-"));
}

HashElement tabela[31];

void acumula(int uf, float peso) {
  int pos = uf % 31;
  HashElement *atual = &tabela[pos];

  while (atual) {
    if (atual->uf == uf) {
      atual->soma += peso;
      atual->qtd++;
      return;
    }
    atual = atual->next;
  }

  atual = (HashElement*) malloc(sizeof(HashElement));
  atual->uf = uf;
  atual->qtd = 1;
  atual->soma = peso;
  if (tabela[pos].uf == 0) {
    atual->next = NULL;
    tabela[pos] = *atual;
  } else {
    atual->next = tabela[pos].next;
    tabela[pos].next = atual;
  }
}

void processa(DadosNascimento* dn) {
  int uf = dn->CODMUNRES / 10000;
  acumula(uf, dn->PESO);
}

int main() {
  char *prox = NULL;
  int limite = 0;
  static char buffer[MAX];
  DadosNascimento dn;
  char* linha[18];
  int i;

  HashElement e;
  e.qtd = 0;
  e.soma = 0;
  e.uf = 0;
  e.next = NULL;
  for (int i = 0; i < 31; i++) {
    tabela[i] = e;
  }

  FILE *f = fopen("../_gerais/sinasc.csv","r");
  fgets(buffer, MAX, f);
  fgets(buffer, MAX, f);
  while(!feof(f)) {
    linha[0] = strtok_r(buffer, ",\n", &prox);

    for(i = 1; i < 18; i++) {
      linha[i] = strtok_r(NULL, ",\n", &prox);
      if(linha[i] == NULL) {
        linha[i] = "";
      }
    }

    dn.index = atoi(linha[0]);
    dn.CODMUNRES = atoi(linha[1]);
    strcpy(dn.data_nasc, linha[2]);
    dn.PESO = atoi(linha[3]);
    dn.SEMAGESTAC = atoi(linha[4]);
    dn.adeq_peso_id_gestac = atof(linha[5]);
    dn.SEXO = atoi(linha[6]);
    dn.IDANOMAL = atoi(linha[7]);
    dn.RACACOR = atoi(linha[8]);
    dn.ESCMAEAGR1 = atoi(linha[9]);
    dn.ESTCIVMAE = atoi(linha[10]);
    dn.IDADEMAE = atoi(linha[11]);
    dn.CONSULTAS = atoi(linha[12]);
    dn.PARTO = atoi(linha[13]);
    dn.STTRABPART = atoi(linha[14]);
    dn.KOTELCHUCK = atoi(linha[15]);
    dn.APGAR5 = atoi(linha[16]);
    dn.TPROBSON = atoi(linha[17]);

    processa(&dn);
    //printf("DEBUG: Processando %d\n", dn.CODMUNRES);

    limite++;
    if(limite == 1000) {
      //break;
    }

    fgets(buffer, MAX, f);
  }
  
  HashElement *atual;
  float media;
  printf("%2s | %16s | %8s | %10s |\n", "UF", "Soma", "Qtd", "Media");
  printf("---|------------------|----------|------------|\n");
  for (int i = 0; i < 31; i++) {
    if (tabela[i].uf) {
      atual = &tabela[i];
      while (atual) {
        media = atual->soma / atual->qtd;
        printf("%2d | %13.2f Kg | %8d | %7.2f Kg |\n", atual->uf, atual->soma / 1000, atual->qtd, media / 1000);
        atual = atual->next;
      }
    }
  }
  
  fclose(f);

  return 0;
}