#include "common.h"

bool adj(const bool a[15][15], int n)
{
    for(int j=0; j<n; j++){
        for(int i=0; i<n; i++){
            if(i!=(n-1) && j!=(n-1)){
                if(a[j][i]==a[j][i+1]){
                    return false;
                }
                if(a[j][i]==a[j+1][i]){
                    return false;
                }
            }
            if(i==(n-1)){
                if(a[j][i]==a[j][i-1]){
                    // printf("%d", a[j][i+1]);
                    return false;
                }
            }
            if(j==(n-1)){
                if(a[j][i]==a[j-1][i]){
                    return false;
                }
            }
            // printf("%d %d\n", a[j][i], a[j][i+1]);

        }
    }
    return true;
}
