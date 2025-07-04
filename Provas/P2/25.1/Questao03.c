#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int codigo;
  char descricao[40];
  float preco;
} Produto;

FILE *arquivo_dados;
void *arvore_bplus;
long root_offset;

int bplus_insert(void *arvore, int chave, long posicao);
int bplus_search(void *arvore, int chave, long *posicao);
void bplus_traverse(void *arvore, void (*callback)(int codigo, long posicao));
long new_node(int is_leaf);

void imprimir_produto(int codigo, long posicao) {
  Produto p;
  fseek(arquivo_dados, posicao, SEEK_SET);
  fread(&p, sizeof(Produto), 1, arquivo_dados);
  printf("%d - %s - R$%.2f\n", p.codigo, p.descricao, p.preco);
}

int insere(Produto p) {
  long posicao;

  if (bplus_search(arvore_bplus, p.codigo, &posicao)) {
    return 0;
  }

  fseek(arquivo_dados, 0, SEEK_END);
  posicao = ftell(arquivo_dados);
  fwrite(&p, sizeof(Produto), 1, arquivo_dados);

  return bplus_insert(arvore_bplus, p.codigo, posicao);
}

void busca(int codigo) {
  long posicao;

  if (!bplus_search(arvore_bplus, codigo, &posicao)) {
    printf("Produto de código %d não foi encontrado.\n", codigo);
    return;
  }

  imprimir_produto(codigo, posicao);
}

void lista() {
  bplus_traverse(arvore_bplus, imprimir_produto);
}

//Função "imprimir_produto" incrementada:
//void imprimir_produto(int codigo, long posicao) {
//  long posicao_atual = ftell(arquivo_dados);
//  Produto p;
//
//  fseek(arquivo_dados, posicao, SEEK_SET);
//  fread(&p, sizeof(Produto), 1, arquivo_dados);
//
//  if (p.codigo != codigo) {
//    printf("Código e produto na posição não coincidem.\n");
//  } else {
//    printf("%d - %s - R$%.2f\n", p.codigo, p.descricao, p.preco);
//  }
//  
//  fseek(arquivo_dados, posicao_atual, SEEK_SET);
//}

int main() {
  arquivo_dados = fopen("produtos.dat", "r+b");
  if (!arquivo_dados) arquivo_dados = fopen("produtos.dat", "w+b");

  arvore_bplus = fopen("indice.dat", "r+b");
  if (!arvore_bplus) arvore_bplus = fopen("indice.dat", "w+b");

  fseek((FILE *)arvore_bplus, 0, SEEK_END);
  if (ftell((FILE *)arvore_bplus) == 0) {
    root_offset = new_node(1);
  } else {
    root_offset = 0;
  }

  Produto p1 = {101, "Arroz", 20.5};
  Produto p2 = {103, "Feijao", 10.0};
  Produto p3 = {102, "Macarrao", 5.75};

  insere(p1);
  insere(p2);
  insere(p3);

  lista();
  busca(103);
  busca(999);

  fclose(arquivo_dados);
  fclose((FILE *)arvore_bplus);
  return 0;
}