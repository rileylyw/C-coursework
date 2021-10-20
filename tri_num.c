#include <stdio.h>
#include <math.h>
#include <assert.h>

void test(void);
int tri(int nth);

int main(void){
    test();
    int tri_num;
    printf("How many triangle numbers would you like to see? ");
    scanf("%d", &tri_num);

    for(int nth=1; nth<=tri_num; nth++){
        int t = tri(nth);
        printf("T(%d) is %d\n", nth, t);
    }
    return 0;
}

int tri(int nth){
    int t = 1;
    int n = 1;
    for(int i=2; n<nth; i++){
        t = t + i;
        n++;
    }
    return t;
}

void test(void){
    assert(tri(6)==21);
    assert(tri(46)==1081);
    assert(tri(53)==1431);
    assert(tri(100)==5050);
}