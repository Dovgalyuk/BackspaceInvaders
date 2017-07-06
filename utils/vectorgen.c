#include <math.h>
#include <stdio.h>

int main()
{
    int i, count = 32;
    for (i = 0 ; i < count ; ++i)
    {
        double a = 3.141592654 / 2 / count * i;
        double s = sin(a) * (1 << 16);
        double c = cos(a) * (1 << 16);
        printf("    {%d, %d},\n", (int)s, (int)c);
    }
}
