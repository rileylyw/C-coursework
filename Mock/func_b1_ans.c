#include "common.h"

/* 
   Return the count of numbers in the array l (of size n)
   which are unique (i.e. duplicates are not counted)
*/

int numdiff(const int l[], const int n)
{
    bool unique;
    int unique_count = 0;
    // Go through each item in the array
    for(int i=0; i<n; i++){
        // Start with the assumption it's unique
        unique = true;
        // Check every item before it in the list for a match
        for(int j=i-1; j>=0; j--){
            // If the item before matches, it is a duplicate and so we must set the current number to not unique
            if(l[i] == l[j]){
                unique = false;
            }
        }
        if(unique){
            // We only count the numbers that are not duplicates of a number already counted
            unique_count++;
        }
    }
    return unique_count;
}          
