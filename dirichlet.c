#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define N 10000

void test(void);
bool isfactor(int n, int f);
bool isprime(int n);

int main(void){
    test();
    int count = 0;
    int maybe_prime = 2;
    int prime = 2;
    int num = 0;

    while(count<N){
        if(isprime(maybe_prime)){
            prime = maybe_prime;
            count++;
            //printf("prime: %d\n", prime);
            
            if((prime-3)%10==0){
                num++;
                //printf("num: %d\n", num);
            }
        }
        maybe_prime++;
    }
    printf("Fraction of primes ending with 3: %d/%d\n", num, count);
    return 0;
}

bool isprime(int n){
    assert(n>1);
    if(n==2){
        return true;
    }
    if(n%2==0){   //even numbers are not prime check
        return false;
    }
    for(int i=3; i<n; i++){
        if(isfactor(n,i)){
            return false;
        }
    }
    return true;
}

bool isfactor(int n, int f){
    return (n%f == 0);
}

void test(void){
    assert(isprime(2));
    assert(isprime(3));
    assert(isprime(5));
    assert(isprime(7919));
    assert(isfactor(4,2));
    assert(isfactor(36,6));
    assert(isfactor(1024,2));
    //printf("test complete\n");
}