#include "common.h"

int numdiff(const int l[], const int n)
{
    int count=0;
    for(int j=0; j<n; j++){
        for(int i=j+1; i<n; i++){
            if(l[j]==l[i]){
                count++;
                i=n;
                // printf("count: %d\n", count);
            }
        }
    }
    count=n-count;
    return count;
}          
