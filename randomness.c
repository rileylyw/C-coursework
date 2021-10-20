#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int random_number(void);

int main(void){
    int ans = random_number();
    return 0;
}

int random_number(void){
    int minus_cnt = 0;
    int plus_cnt = 0;
    
    for(int i=0; i<500; i++){
        int ran_num = rand();
        if(ran_num < RAND_MAX/2){
            minus_cnt++;
            printf("difference: %d\n", plus_cnt - minus_cnt);
        }
        else{
            plus_cnt++;
            printf("difference: %d\n", plus_cnt - minus_cnt);
        }
    }
    printf("greater than %d\n", plus_cnt);
    printf("less than %d\n", minus_cnt);
}