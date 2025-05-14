#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char cpf[15], curso[20], dataNascimento[11], sexo, email[40], opacaoQuadro;
} Registro;

typedef struct {
  char nome[20];
  int qtd;
} Curso;

int main() {
  FILE *f;
  Registro r;
  Curso *cursos = NULL;
  int qtdCursos = 0, index = -1;

  f = fopen("candidatos.dat", "rb");
  if (!f) {
    fprintf(stderr, "Arquivo candidatos.dat não pode ser aberto para leitura.\n");
    return 1;
  }

  fread(&r, sizeof(Registro), 1, f);
  while (!feof(f)) {
    index = -1;

    for (int i = 0; i < qtdCursos; i++) {
      if (strncmp(cursos[i].nome, r.curso, 20) == 0) {
        index = i;
        break;
      }
    }

    if (index == -1) {
      qtdCursos++;
      cursos = realloc(cursos, qtdCursos * sizeof(Curso));
      strncpy(cursos[qtdCursos - 1].nome, r.curso, 20);
      cursos[qtdCursos - 1].qtd = 1;
    } else {
      cursos[index].qtd++;
    }

    fread(&r, sizeof(Registro), 1, f);
  }

  printf("Cursos e qtds de candidatos:\n");
  for (int i = 0; i < qtdCursos; i++) {
    printf("%s: %d\n", cursos[i].nome, cursos[i].qtd);
  }

  free(cursos);

  fclose(f);

  return 0;
}