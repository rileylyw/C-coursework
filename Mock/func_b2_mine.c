#include "common.h"

int closestmean(const int l[], const int n)
{
    int sum=0;
    for(int i=0; i<n; i++){
        sum=sum+l[i];
    }
    int mean=sum/n;

    int temp[100];
    for(int j=0; j<n; j++){
        temp[j]=abs(l[j]-mean);
        // if(temp[j]<0){
        //     temp[j]=temp[j]*(-1);
        // }
    }
    int closest=0;
    for(int j=0; j<n; j++){
        for(int i=j+1; i<n; i++){
            if(temp[i]<temp[j]){
                closest=i;
                i=n;
            }
        }
        j=n;
    }
    // printf("l[i]: %d\n", l[closest]);


    return l[closest];
}          
