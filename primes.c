#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

void test(void);
bool isfactor(int n, int f);
bool isprime(int n);
int nthprime(int n);

int main(void){
    test();
    int num;
    printf("Which prime would you like to see? ");
    scanf("%d", &num);
    int ans = nthprime(num);
    printf("answer = %d\n", ans);
    return 0;
}

int nthprime(int n){
    int count = 0;
    int prime = 2;
    int maybe_prime = 2;
    
    while(count < n){
        if(isprime(maybe_prime)){
            count++;
            prime = maybe_prime;
        }
        maybe_prime++;
    }
    return prime;
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
    assert(nthprime(2)==3);
    assert(nthprime(100)==541);
    //printf("test complete\n");
}