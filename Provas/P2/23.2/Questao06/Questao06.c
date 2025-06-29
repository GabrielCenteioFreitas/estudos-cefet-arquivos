#include <stdio.h>
#include <string.h>

#define MAX_LINE 80

int compara (char* s1, char* s2) {
  return strncmp(s1, s2, MAX_LINE);
}

int main() {
  FILE *f1 = fopen("a1.txt", "r"), *f2 = fopen("a2.txt", "r"), *f3 = fopen("a3.txt", "w");

  char line1[MAX_LINE], line2[MAX_LINE];
  int achou = 1;

  while (fgets(line1, sizeof(line1), f1)) {
    rewind(f2);
    
    while (fgets(line2, sizeof(line2), f2)) {
      if (compara(line1, line2) == 0) {
        achou = 1;
        break;
      } else if (compara(line1, line2) < 0) {
        achou = 0;
        break;
      }
    }

    if (!achou) {
      fputs(line1, f3);
    }

    achou = 1;
  }

  fclose(f1);
  fclose(f2);
  fclose(f3);

  return 0;
}