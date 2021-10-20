#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

int main(void)
{
    srand(time(NULL));

    int n;
    printf("How many songs required? ");
    scanf("%d", &n);

    int arr[n];

    for (int i = 0; i < n; i++) //fill array
    {
        arr[i] = i+1;
        //printf("%d\n", arr[i]);
    }

    for (int i = 0; i < n; i++)
    {
        int temp = arr[i];
        int ran = rand() % n; //not larger than n
        arr[i] = arr[ran];
        arr[ran] = temp;

        //printf("%d\n", arr[i]);
    }

    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    return 0;
}