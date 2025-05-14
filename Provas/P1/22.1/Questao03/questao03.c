#include <stdio.h>
#include <string.h>

typedef struct {
  char cpf[15], curso[20], dataNascimento[11], sexo, email[40], opacaoQuadro;
} Registro;

int main() {
  FILE *f1, *f2, *f3;
  Registro r1, r2, primeiro, ultimo;
  int inicio = 0, fim, meio, comp;

  f1 = fopen("candidatos2021.dat", "rb");
  if (!f1) {
    fprintf(stderr, "Arquivo candidatos2021.dat não pode ser aberto para leitura.\n");
    return 1;
  }

  f2 = fopen("candidatos2022.dat", "rb");
  if (!f2) {
    fprintf(stderr, "Arquivo candidatos2022.dat não pode ser aberto para leitura.\n");
    return 1;
  }

  f3 = fopen("resultado.dat", "wb");
  if (!f1) {
    fprintf(stderr, "Arquivo resultado.dat não pode ser aberto para escrita.\n");
    return 1;
  }

  fread(&primeiro, sizeof(Registro), 1, f2);
  fseek(f2, -sizeof(Registro), SEEK_END);
  fread(&ultimo, sizeof(Registro), 1, f2);

  fread(&r1, sizeof(Registro), 1, f1);
  while (!feof(f1)) {
    if (strncmp(r1.cpf, primeiro.cpf, 15) < 0 || strncmp(r1.cpf, ultimo.cpf, 15) > 0) {
      fread(&r1, sizeof(Registro), 1, f1);
      continue;
    }

    if (strncmp(r1.cpf, primeiro.cpf, 15) == 0 || strncmp(r1.cpf, ultimo.cpf, 15) == 0) {
      fwrite(&r1, sizeof(Registro), 1, f3);
      fread(&r1, sizeof(Registro), 1, f1);
      continue;
    }

    fseek(f2, 0, SEEK_END);
    fim = ftell(f2) / sizeof(Registro);
    meio = (inicio + fim) / 2;
    fseek(f2, meio * sizeof(Registro), SEEK_SET);

    fread(&r2, sizeof(Registro), 1, f2);
    while (inicio <= fim) {
      comp = strncmp(r1.cpf, r2.cpf, 15);

      if(comp == 0) {
        fwrite(&r1, sizeof(Registro), 1, f3);
        break;
      } else if (comp > 0) {
        inicio = meio + 1;
      } else {
        fim = meio - 1;
      }

      meio = (inicio + fim) / 2;
      fseek(f2, meio * sizeof(Registro), SEEK_SET);
      fread(&r2, sizeof(Registro), 1, f2);
    }

    fread(&r1, sizeof(Registro), 1, f1);
  }

  fclose(f1);
  fclose(f2);
  fclose(f3);

  return 0;
}