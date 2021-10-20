#include <time.h>
#include <stdio.h>

int main(void){
    int x, y, a, b;

    printf("Enter two times e.g. 23:00 04:15: ");
    scanf("%d:%d %d:%d", &x, &y, &a, &b);

    int hour, minute;

    if(a < x){
        hour = 24 + a - x;
    }
    else{
        hour = a - x;
    }

    if(b < y){
        minute = 60 + b -y;
    }
    else{
        minute = b - y;
    }

    printf("Difference is: %d:%d\n", hour, minute);

    return 0;
}
