#include <stdlib.h>
#include <math.h>

int main(void){
    double x;
    printf("Input a real number: ");
    scanf("%lf", &x);

    printf("sin(%.2lf)^2 + cos(%.2lf)^2 = %.2lf\n", x, x, (pow(sin(x), 2)) + (pow(cos(x), 2)));
    
    return 0;
}