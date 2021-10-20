#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(void) {

srand(time(NULL));
int n;
double x, y ,r ,pi;

printf("Number of iterations: ");
scanf("%d", &n);

int count = 0;

for(int i = 0; i < n; ++i) {
    x = (double)rand()/RAND_MAX; //generates num bewteen 0 and 1
    y = (double)rand()/RAND_MAX;
    //printf("%g", x);
    r = 1;
    if(r*r >= x*x + y*y){
        count++;
    }
}

pi = (double) count / n * 4;

printf("Approximate PI = %g", pi);

return 0;
}