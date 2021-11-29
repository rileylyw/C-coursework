#include <stdio.h>

#define SIZE 3
#define START_NUM 1
#define INCR(A) A+1
#define DECR(A) A-1


/* A program to recursively find and print the valid paths from 
1 - 9 that start from the top left corner */


// Recursively print valid paths from 1 - 9 in the given array
void print_paths(int arr[SIZE][SIZE], int num, int y, int x);
// Print the values in the given array
void print_arr(int arr[SIZE][SIZE]);

int main (void){
    int arr[SIZE][SIZE] = {0};

    print_paths(arr, START_NUM, 0, 0);

    return 0;
}

void print_paths(int arr[SIZE][SIZE], int num, int y, int x){
    // Out of bounds check
    if(y<0||x<0||y>=SIZE||x>=SIZE){
        return;
    }
    // Check if cell already has value
    if(arr[y][x]){
        return;
    }
    // Assign number to cell
    arr[y][x] = num;
    // End case
    if(num == 9){
        print_arr(arr);
        // Reset
        arr[y][x] = 0;

        return;
    }
    // Look up
    print_paths(arr, INCR(num), DECR(y), x);
    // Look right
    print_paths(arr, INCR(num), y, INCR(x));
    // Look down
    print_paths(arr, INCR(num), INCR(y), x);
    // Look left
    print_paths(arr, INCR(num), y, DECR(x));
    // Reset
    arr[y][x] = 0;

}

void print_arr(int arr[SIZE][SIZE]){
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}