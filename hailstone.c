#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 10000000
long hailstone_length(long i);
long hailstone_max(long i);
void test(void);

int main(void){
    test();

    long xnum,max_num=1,max_len=0,max_len_num;

    for(long len_num=1;len_num<N;len_num++){
        long len = hailstone_length(len_num);
        if(max_len<len){
            max_len=len;//longest
            max_len_num=len_num;//corresponding initial number
        }

        long num = hailstone_max(len_num);
        if(max_num<num){
            max_num=num;//largest
            xnum=len_num;//corresponding initial number
        }
    }
    printf("The initial number %ld leads to the longest hailstone sequence: %ld.\n", max_len_num, max_len);
    printf("The initial number %ld leads to the largest hailstone sequence: %ld.\n", xnum, max_num);
    
    return 0;
}

long hailstone_length(long i){
    int length=1;

    while(i!=1){
        if (i%2==0){
            i=i/2;
        }
        else{
    i=3*i+1;
        }
        length++;
    }
    return length;
}

long hailstone_max(long i){
    long max=1;

    while(i!=1){
        if (i%2==0){
            i=i/2;
        }
        else{
            i=3*i+1;
        }
        if(i>max){
            max=i;
        }
    }
    return max;
}

void test(void){
    assert(hailstone_length(6)==9);
    assert(hailstone_length(11)==15);
    assert(hailstone_length(234)==22);
    assert(hailstone_length(65331)==118);
    assert(hailstone_max(6)==16);
    assert(hailstone_max(11)==52);
    assert(hailstone_max(16283)==225988);
    assert(hailstone_max(345872)==172936);
}