#include <stdio.h>
#include <assert.h>
#include <math.h>

#define PI 3.14159265358979323846

int main(void)
{
    double ans;
    long i, j;
    for (i = 1; i <= 100000; i++)
    {
        j = 1;
        while (j / i - PI <= 0){
            j++;
            ans = (double)j / i;
            //printf("%.20lg\n", ans);
        }
        printf("%.20lg\n", ans);
        //if ((ans - PI) < pow(10, -20))
        //{
        //    printf("%.20lg\n", ans);
        //}
    }
    return 0;
}
