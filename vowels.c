#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

bool isvowel(char string);
void test(void);
char uppercase(char string[]);

int main(void){
    test();
    char string[50];

    printf("Convert vowels to upper case: ");
    fgets(string, 50, stdin);
    uppercase(string);
    printf("%s", string);
    return 0;
}


char uppercase(char string[]){
    int count = 0;
    for (int i = 0; string[i]!='\0'; i++) {
        if ((isvowel(string[i])==true)){
            string[i] = toupper(string[i]);
            count++;
        }
    }
    return count;
}

bool isvowel(char string){
    if (string == 'a' || string == 'e'
        || string == 'i' || string == 'o'
        || string == 'u'){
        return true;
    }
    else{
        return false;
    }
}

void test(void){
    assert(isvowel('a')==true);
    assert(isvowel('x')==false);
    assert((isvowel('y'))==false);

    char string1[3] = {'a', 'e', 'i'};
    assert((uppercase(string1))==3);
    char string2[5] = {'a', 'u', 'o', 'l', 'p'};
    assert((uppercase(string2))==3);

    printf("test done");
}