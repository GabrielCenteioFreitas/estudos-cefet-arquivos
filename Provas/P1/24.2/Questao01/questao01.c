#include <stdio.h>
#include <string.h>

typedef struct {
  int id;
  char nome[30];
  char especie[20];
  int idade;
  float peso;
} Animal;

void buscarRegistro(FILE *f) {
  Animal a;
  int id;

  fprintf(stdout, "ID: ");
  fscanf(stdin, "%d", &id);

  fread(&a, sizeof(Animal), 1, f);
  while (!feof(f)) {
    if (a.id == id) {
      fprintf(stdout, "%d - %.30s - %.20s - %d ano(s) - %.2fkg\n", a.id, a.nome, a.especie, a.idade, a.peso);
      return;
    }

    fread(&a, sizeof(Animal), 1, f);
  }

  fprintf(stdout, "Animal de ID %d não encontrado!\n", id);
}

void incluirRegistro(FILE *f) {
  Animal a, b;

  fprintf(stdout, "ID: ");
  fscanf(stdin, "%d", &a.id);
  fprintf(stdout, "Nome: ");
  getchar();
  fgets(a.nome, sizeof(a.nome), stdin);
  a.nome[strcspn(a.nome, "\n")] = 0;
  fprintf(stdout, "Espécie: ");
  fgets(a.especie, sizeof(a.especie), stdin);
  a.especie[strcspn(a.especie, "\n")] = 0;
  fprintf(stdout, "Idade: ");
  fscanf(stdin, "%d", &a.idade);
  fprintf(stdout, "Peso: ");
  fscanf(stdin, "%f", &a.peso);

  fread(&b, sizeof(Animal), 1, f);
  while (!feof(f)) {
    if (b.id == a.id) {
      fprintf(stdout, "ID já existente!\n");
      return;
    }

    fread(&b, sizeof(Animal), 1, f);
  }

  fseek(f, 0, SEEK_END);
  fwrite(&a, sizeof(Animal), 1, f);
}

void alterarRegistro(FILE *f) {
  Animal a, b;
  int id, posicao = -1;

  fprintf(stdout, "ID: ");
  fscanf(stdin, "%d", &id);

  fread(&b, sizeof(Animal), 1, f);
  while (!feof(f)) {
    if (b.id == id) {
      posicao = ftell(f) / sizeof(Animal) - 1;
      break;
    }

    fread(&b, sizeof(Animal), 1, f);
  }

  if (posicao == -1) {
    fprintf(stdout, "Animal de ID %d não encontrado!\n", id);
    return;
  } else {
    fprintf(stdout, "%d - %.30s - %.20s - %d ano(s) - %.2fkg\n", b.id, b.nome, b.especie, b.idade, b.peso);
  }

  fprintf(stdout, "ID: ");
  fscanf(stdin, "%d", &a.id);
  fprintf(stdout, "Nome: ");
  getchar();
  fgets(a.nome, sizeof(a.nome), stdin);
  a.nome[strcspn(a.nome, "\n")] = 0;
  fprintf(stdout, "Espécie: ");
  fgets(a.especie, sizeof(a.especie), stdin);
  a.especie[strcspn(a.especie, "\n")] = 0;
  fprintf(stdout, "Idade: ");
  fscanf(stdin, "%d", &a.idade);
  fprintf(stdout, "Peso: ");
  fscanf(stdin, "%f", &a.peso);

  fseek(f, posicao * sizeof(Animal), SEEK_SET);
  fwrite(&a, sizeof(Animal), 1, f);
}

int main() {
  FILE *f;

  f = fopen("animais.dat", "r+");
	
  if (!f) {
    fprintf(stderr, "Arquivo animais.dat não pode ser aberto para leitura.\n");
    return 1;
  }

  buscarRegistro(f);
  incluirRegistro(f);
  alterarRegistro(f);

  return 0;
}