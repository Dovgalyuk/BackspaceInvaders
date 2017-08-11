#include <stdio.h>

int main(int argc, char **argv)
{
  if (argc != 2)
    return 0;
  FILE *fin = fopen(argv[1], "rb");
  int count = 0;
  while (!feof(fin))
  {
      unsigned char b;
      if (fread(&b, 1, 1, fin) != 1)
          break;
      printf("0x%02x, ", b);
      if (++count % 16 == 0)
        printf("\n");
  }
  fclose(fin);
}
