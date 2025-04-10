#include <stdio.h>

int main() {
  remove("cep_a.dat");
  remove("cep_b.dat");
  char nomeArquivo[20];
  for (int i = 0; i < 1000; i++) {
    sprintf(nomeArquivo, "cep_%d.dat", i);
    remove(nomeArquivo);
  }
  remove("cep_ordenado.dat");

  return 0;
}
