#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define SIZE 100

int anagram(char s1[], char s2[]);

int main(void){
    assert(anagram("elvis", "lives")==1);
    assert(anagram("dreads", "sadder")==1);
    assert(anagram("replays", "parsley")==1);
    assert(anagram("listen", "silent")==1);
    assert(anagram("orchestra", "carthorse")==1);

    assert(anagram("elvis", "elvis")==0);
    assert(anagram("neill", "neil")==0);
    assert(anagram("neil", "neill")==0);
    assert(anagram("horse", "short")==0);

    return 0;
}



int anagram(char s1[], char s2[]){
    char s1copy[SIZE];
    char s2copy[SIZE];
    
    int len1=strlen(s1);
    int len2=strlen(s2);
    
    for(int i=0; i<len1; i++){
        s1copy[i]=s1[i];
    }
    for(int j=0; j<len2; j++){
        s2copy[j]=s2[j];
    }

    char temp;
    if(len1!=len2 || (strcmp(s1copy, s2copy)==0)){
        return 0;
    }
    else{
        for(int i=0; i<len1-1; i++){ //sort
            for(int j=i+1; j<len1; j++){
                if(s1copy[i]>s1copy[j]){
                    temp=s1copy[i];
                    s1copy[i]=s1copy[j];
                    s1copy[j]=temp;
                }
                if(s2copy[i]>s2copy[j]){
                    temp=s2copy[i];
                    s2copy[i]=s2copy[j];
                    s2copy[j]=temp;
                }
            }
        }
        for(int i=0; i<len1; i++){ //compare
            if(s1copy[i]!=s2copy[i]){
                return 0;
            }
        }
    }
    return 1;
}