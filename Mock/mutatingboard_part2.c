#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define N 21

void printBoard(int arr[N][N]);
void mutate(int arr[N][N]);
void swap(int *a, int *b);
float distance(int x1, int x2, int y1, int y2);

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


    int center;
    if(N%2==0){
        center=N/2;
    }
    else{
        center=(int)(N/2)+1;
    }

    for(int x=0; x<N*N*N*N; x++){
        int j=rand()%N;
        int i=rand()%N;
        int j2=rand()%N;
        int i2=rand()%N;
        float distanceA=distance(abs(j-center), center, abs(i-center), center);
        float distanceB=distance(abs(j2-center), center, abs(i2-center), center);

        if(distanceA < distanceB){
            if(arr[j][i] < arr[j2][i2]){
                swap(&arr[j][i], &arr[j2][i2]);
            }
        }
    }
}

float distance(int x1, int x2, int y1, int y2){
    float ans=sqrt(pow((x2-x1), 2)+pow((y2-y1), 2));
    return ans;
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
