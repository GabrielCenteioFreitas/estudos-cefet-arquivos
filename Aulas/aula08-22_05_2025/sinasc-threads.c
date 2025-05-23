#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX 2048
#define HASH_QNT 31
#define NUM_THREADS 2

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

HashElement tabela[HASH_QNT];
pthread_mutex_t locks[HASH_QNT]; 
int qtd, qtdCadaThread, resto;

void acumula(int uf, float peso) {
  int pos = uf % HASH_QNT;
  
  pthread_mutex_lock(&locks[pos]);
  HashElement *atual = &tabela[pos];

  while (atual) {
    if (atual->uf == uf) {
      atual->soma += peso;
      atual->qtd++;
      pthread_mutex_unlock(&locks[pos]);
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
    
  pthread_mutex_unlock(&locks[pos]);
}

void processa(DadosNascimento* dn) {
  int uf = dn->CODMUNRES / 10000;
  acumula(uf, dn->PESO);
}

void printarResultado() {
  HashElement *atual;
  float media;

  printf("%2s | %16s | %8s | %10s |\n", "UF", "Soma", "Qtd", "Media");
  printf("---|------------------|----------|------------|\n");
  for (int i = 0; i < HASH_QNT; i++) {
    if (tabela[i].uf) {
      atual = &tabela[i];
      while (atual) {
        media = atual->soma / atual->qtd;
        printf("%2d | %13.2f Kg | %8d | %7.2f Kg |\n", atual->uf, atual->soma / 1000, atual->qtd, media / 1000);
        atual = atual->next;
      }
    }
  }
}

void* executar(void* arg) {
  int id = *((int*)arg);

  char *prox = NULL;
  int linesProcessed = 0;
  char buffer[MAX];
  DadosNascimento dn;
  char* linha[18];
  int i;

  FILE *f = fopen("../_gerais/sinasc.csv","r");
  if (!f) {
    printf("ERRO: Thread %d não conseguiu abrir o arquivo\n", id);
    pthread_exit(NULL);
  }

  int qtdASerLida = id == NUM_THREADS - 1
    ? qtdCadaThread + resto
    : qtdCadaThread;

  fgets(buffer, MAX, f);
  for (i = 0; i < id * qtdCadaThread; i++) {
    if (!fgets(buffer, MAX, f)) break;
  }

  for (i = 0; i < qtdASerLida; i++) {
    if (!fgets(buffer, MAX, f)) break;

    linha[0] = strtok_r(buffer, ",\n", &prox);
    for(int j = 1; j < 18; j++) {
      linha[j] = strtok_r(NULL, ",\n", &prox);
      if(linha[j] == NULL) {
        linha[j] = "";
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
  }

  fclose(f);
  pthread_exit(NULL);
  return NULL;
}

int main() {
  struct timeval inicio, fim;
  gettimeofday(&inicio, NULL);
  
  pthread_t threads[NUM_THREADS];
  int ids[NUM_THREADS];
  
  for (int i = 0; i < HASH_QNT; i++) {
    tabela[i].uf = 0;
    tabela[i].qtd = 0;
    tabela[i].soma = 0;
    tabela[i].next = NULL;
    pthread_mutex_init(&locks[i], NULL);
  }

  FILE *f = fopen("../_gerais/sinasc.csv","r");
  if (!f) {
    printf("ERRO: Não foi possível abrir o arquivo\n");
    return 1;
  }
  
  int qtdLida = 0;
  char buffer[MAX];
  fgets(buffer, MAX, f);
  while (fgets(buffer, MAX, f) != NULL) {
    qtdLida++;
  }
  rewind(f);

  qtd = qtdLida;
  qtdCadaThread = qtd / NUM_THREADS;
  resto = qtd % NUM_THREADS;

  for (int i = 0; i < NUM_THREADS; i++) {
    ids[i] = i;
    if (pthread_create(&threads[i], NULL, executar, &ids[i]) != 0) {
      fprintf(stderr, "Erro ao criar thread %d\n", i);
      return 1;
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printarResultado();
  
  HashElement *atual, *prox;
  for (int i = 0; i < HASH_QNT; i++) {
    pthread_mutex_destroy(&locks[i]);
    atual = tabela[i].next;
    while (atual) {
      prox = atual->next;
      free(atual);
      atual = prox;
    }
  }

  gettimeofday(&fim, NULL);
  double tempo_execucao = (fim.tv_sec - inicio.tv_sec) + 
                         (fim.tv_usec - inicio.tv_usec) / 1000000.0;
  printf("\nTempo de execucao: %.4f segundos\n", tempo_execucao);

  fclose(f);

  return 0;
}