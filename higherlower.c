#include <stdio.h>
#include <stdlib.h>

int ran(void);

int main(void){
    int x;
    int ran_num = ran();
    int count = 0;

    while(count < 10){
        printf("Select a number between 1 and 1000: ");
        scanf("%d", &x);
        if(x != ran_num){
            count++;
        }
        else{
            printf("Congrats! You've got it!\n");
        }
    }
    printf("The correct number is %d\n", ran_num);
    return 0;
}

int ran(void){
    int ran;    
    while(!(ran > 0 && ran < 1001)){
        ran = rand();
    }
    return ran;
}
