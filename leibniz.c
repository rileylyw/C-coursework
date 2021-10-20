#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.141592653

double absolute(double a);

int main(void){
    int sign = 1;
    double pi = 0;
    double precision = pow(10, -9);
    //printf("How many interations? ");
    //scanf("%d", &n);
    long i;

    for(i=1; absolute(pi - PI) > precision; i+=2){
        //if(i%2==1){
            pi += (double) 4/(i*sign);
            sign = sign * -1;
            //count++;
        //}
        //printf("%lg\n", precision);
        //printf("%lg\n", pi-PI);
    }
    //printf("%ld\n", count);
    printf("No. of iterations: %ld\n", i/2);
    printf("%.10lg\n", pi);
    return 0;
}

double absolute(double a){
    if(a<0){
        a = a * -1;
    }
    return a;
}