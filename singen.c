#include <math.h>
#include <stdio.h>

int main()
{
    int width;
    int height = 64;
    scanf("%d", &width);
    //int resx[height];
    //int resy[height];
    for (int i = 0 ; i < height ; ++i)
    {
        double a = 2 * 3.141592654 * i / height;
        double s = sin(a);
        double x = width / 2.0 * (s + 1);
        //resx[i] = (int)x;
        printf("%d, ", (int)x);
    }
}
