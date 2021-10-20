#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

void test(void);
int RoundUp(int toRound);

int main(void){
    test();
    int buttons[3] = {10, 60, 600};
    int s, count = 0;

    printf("Type the time required in seconds: ");
    scanf("%d", &s);

    while(s>0){
        printf("%d\n", s);
        count++;
        if(s>=600){
            s -= buttons[2];
        }
        else if(s>=60){
            s -= buttons[1];
        }
        else{
            s -= buttons[0];
        }
    }
    printf("Presses: %d\n", count);
    return 0;
}



int RoundUp(int toRound){
    if(toRound % 10 == 0){
        return toRound;
    }
    toRound = (10 - toRound % 10) + toRound;
    return toRound;
}

void test(void){
    assert(RoundUp(26)==30);
    assert(RoundUp(154)==160);
    assert(RoundUp(1563)==1570);
    //printf("test done\n");
}