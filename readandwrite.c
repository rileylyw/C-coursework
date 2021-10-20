#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BIGSTR 500
#define FNAME "helloworld.txt"

int main(void){
    FILE* fp = fopen(FNAME, "w");
    if(fp==NULL){
        fprintf(stderr, "Cannot open file %s ?\n", FNAME);
        exit(EXIT_FAILURE);
    }
    else{
    fprintf(fp, "hello world!\n");
    }
    fclose(fp); //any way to test if close? it runs fine without fclose

    fp = fopen(FNAME, "r");
    if(fp==NULL){
        fprintf(stderr, "Cannot red file %s ?\n", FNAME);
        exit(EXIT_FAILURE);
    }
    char str[BIGSTR];
    if(fgets(str, BIGSTR, fp)==NULL){
        fprintf(stderr, "Cannot read 1st line of %s ?\n", FNAME);
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    if(strcmp(str, "hello world!\n")){
        fprintf(stderr, "1st line of %s not correct? \n", FNAME);
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}