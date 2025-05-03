#include <stdio.h>
#include <string.h>

typedef struct {
  int matricula;
  char nome[30];
} Registro;

int main() {
  FILE *f1, *f2, *saida;
  Registro r1, r2, primeiro, ultimo;
  int encontrado, inicio = 0, fim, meio;

  f1 = fopen("Arquivo1.dat", "rb");
	
  if (!f1) {
    fprintf(stderr, "Arquivo Arquivo1.dat não pode ser aberto para leitura.\n");
    return 1;
  }

  f2 = fopen("Arquivo2.dat", "rb");
	
  if (!f2) {
    fprintf(stderr, "Arquivo Arquivo2.dat não pode ser aberto para leitura.\n");
    return 1;
  }

  saida = fopen("Resultado.dat", "wb");
	
  if (!saida) {
    fprintf(stderr, "Arquivo Resultado.dat não pode ser aberto para escrita.\n");
    return 1;
  }

  fread(&primeiro, sizeof(Registro), 1, f2);
  fseek(f2, -sizeof(Registro), SEEK_END);
  fread(&ultimo, sizeof(Registro), 1, f2);

  fseek(f2, 0, SEEK_END);
	fim = ftell(f2) / sizeof(Registro);
	meio = (inicio + fim) / 2;

  fread(&r1, sizeof(Registro), 1, f1);
  while (!feof(f1)) {
    if (r1.matricula < primeiro.matricula || r1.matricula > ultimo.matricula) {
      fwrite(&r1, sizeof(Registro), 1, saida);
      fread(&r1, sizeof(Registro), 1, f1);
      continue;
    }

    if (r1.matricula == primeiro.matricula || r1.matricula == ultimo.matricula) {
      fread(&r1, sizeof(Registro), 1, f1);
      continue;
    }

    encontrado = 0;
    fseek(f2, meio * sizeof(Registro), SEEK_SET);
    fread(&r2, sizeof(Registro), 1, f2);
    while (inicio <= fim) {
      if(r2.matricula == r1.matricula) {
        encontrado = 1;
        break;
      } else if (r1.matricula > r2.matricula) {
        inicio = meio + 1;
      } else {
        fim = meio - 1;
      }
  
      meio = (inicio + fim) / 2;
      fseek(f2, meio * sizeof(Registro), SEEK_SET);
      fread(&r2, sizeof(Registro), 1, f2);
    }

    if (!encontrado) {
      fwrite(&r1, sizeof(Registro), 1, saida);
    }

    fread(&r1, sizeof(Registro), 1, f1);
  }

  return 0;
}