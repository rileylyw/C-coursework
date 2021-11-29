#include "common.h"

/*
   Return which number in the array l (of size n) is numerically closest
   to the average (mean) of all the numbers. In the case of a tie (i.e.
   1 or more of them are equally distant from the average) then the first
   such number in the list is returned.
*/

int closestmean(const int l[], const int n)
{
    float avg = 0;
    int total = 0;
    float closest_dist = INFINITY;
    int closest_num = l[0];
    float current;
    // Calculate average
    for(int i=0; i<n; i++){
        total += l[i];
    }
    avg = (float)total / (float)n;

    // Find number closest
    // Will need to check every array element
    for(int i=0; i<n; i++){
        // The current distance from the average can be calculated by taking the average from the current number
        current = l[i] - avg;
        // If the current number is less than the average, then the distance will be less than 0. We multiply by -1 for our comparison
        if(current<0){
            current *= -1;
        }
        // Once we have our distance in positive number form, we can see if it's better than our previous best distance
        // We do a less than here so that in the event of a tie then the first item in the list is chosen
        if(current < closest_dist){
            closest_dist = current;
            closest_num = l[i];
        }
    }
    return closest_num;
}          
