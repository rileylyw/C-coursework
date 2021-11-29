#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define SIZE 100

int isogram(char *s);

int main(void){
    // int x=isogram("programming");
    // printf("x: %d\n", x);
    // int y=isogram("housewarmings");
    // printf("y: %d\n", y);
    // int z=isogram("abductions");
    // printf("z: %d\n", z);
    assert(isogram("programming")==0);
    assert(isogram("housewarmings")==0);
    assert(isogram("abductions")==1);
    assert(isogram("housewarming")==1);
    assert(isogram("hydromagnetics")==1);
    assert(isogram("uncopyrightable")==1);
    return 0;
}

int isogram(char *s){
    char scopy[SIZE];
    char temp[SIZE];

    // printf("s: %ld\n", strlen(s));
    for(unsigned int i=0; i<strlen(s); i++){
        scopy[i]=s[i];
        temp[i]=s[i];
    }
    
    // for(unsigned int i=0; i<strlen(s); i++){
    //     printf("scopy: %c\n", scopy[i]);
    //     printf("temp: %c\n", temp[i]);
    // }

    unsigned int count=0;
    for(unsigned int j=0; j<strlen(s); j++){
        for(unsigned int i=0; i<strlen(s); i++){
            if(scopy[j]==temp[i]){
                count++;
            }
        }
    }
    if(count>strlen(s)){
        return 0;
    }
    return 1;
}