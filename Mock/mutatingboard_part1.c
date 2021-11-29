#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define N 20

void printBoard(int arr[N][N]);
void mutate(int arr[N][N]);
void swap(int *a, int *b);

int main(void){
    // srand(time(NULL));
    int arr[N][N];
    for(int j=0; j<N; j++){
        for(int i=0; i<N; i++){
            arr[j][i]=rand()%10;
        }
    }
    mutate(arr);
    printBoard(arr);
    return 0;
}

void mutate(int arr[N][N]){
    for(int x=0; x<N*N*N; x++){
        int i=rand()%N;
        int j=rand()%N;
        if(arr[j][i]>arr[j][i+1] && i<N-1 && j<N){
            swap(&arr[j][i], &arr[j][i+1]); //col
        }
        if(arr[j][i]>arr[j+1][i] && i<N && j<N-1){
            swap(&arr[j][i], &arr[j+1][i]); //row
        }
    }
}

void swap(int *a, int *b){
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}

void printBoard(int arr[N][N]){
    for(int j=0; j<N; j++){
        for(int i=0; i<N; i++){
            printf("%d", arr[j][i]);
        }
        printf("\n");
    }
}
