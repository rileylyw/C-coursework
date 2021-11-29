#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define SIZE 100

int isogram(char *s);

int main(int argc, char* argv[]){
    FILE* fp;
    if(argc==2){
        fp=fopen(argv[1], "r");
        if(!fp){
            exit(EXIT_FAILURE);
        }
    }
    else{
        fprintf(stderr, "missing file name");
        exit(EXIT_FAILURE);
    }
    char temp[SIZE];
    char longest[SIZE];
    unsigned int templen=0;
    while(fscanf(fp, "%s", temp)!=EOF){
        int x=isogram(temp);
        if(x==1){
            unsigned int length=strlen(temp);
            if(length>templen){
                templen=length;
                strcpy(longest,temp);
            }
        }
    }
    printf("%s (%d)\n", longest, templen);
    assert(isogram("programming")==0);
    assert(isogram("housewarmings")==0);
    assert(isogram("abductions")==1);
    assert(isogram("housewarming")==1);
    assert(isogram("hydromagnetics")==1);
    assert(isogram("uncopyrightable")==1);
    fclose(fp);
    return 0;
}

int isogram(char *s){
    char scopy[SIZE];
    char temp[SIZE];
    for(unsigned int i=0; i<strlen(s); i++){
        scopy[i]=s[i];
        temp[i]=s[i];
    }
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