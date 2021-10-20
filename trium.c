#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define ALPHABET 26
#define INITIALLETTER 'a'

void test(void);
bool isvowel(char string);
char increase_character(char string);

int main(void){
    test();

    int name[3] = {'a', 'a', 'a'};
    int count = 0;
    for(int i=0; i<ALPHABET; i++){
        for(int j=0; j<ALPHABET; j++){
            for(int k=0; k<ALPHABET; k++){
                name[0] = INITIALLETTER + i;
                name[1] = INITIALLETTER + j;
                name[2] = INITIALLETTER + k;
                if(isvowel(name[0])==false && isvowel(name[1])==true && isvowel(name[2])==false){
                    count++;
                    printf("%d %c %c %c\n", count, name[0], name[1], name[2]);
                }
            }
        }
    }
    return 0;
}

char increase_character(char string){
    string++;
    return string;
}

bool isvowel(char string){
    if(string == 'a' || string == 'e'
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
    assert(isvowel('e')==true);
    assert(isvowel('b')==false);

    assert(increase_character('a')=='b');
    assert(increase_character('g')=='h');
    assert(increase_character('E')=='F');
    //printf("test done\n");
}