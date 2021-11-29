#include "common.h"

char* strnodupes(const char* s)
{
    int l = strlen(s);
    bool unique;
    char* no_dupes = malloc(sizeof(char) * l+1);
    // Create an index for where to store the unique characters
    int pos = 0;
    if(!no_dupes){
        printf("Couldn't malloc space");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<l; i++){
        unique = true;
        for(int j=i-1; j>=0; j--){
            if(s[j]==s[i]){
                unique = false;
            }
        }
        if(unique){
            no_dupes[pos] = s[i];
            pos++;
        }
    }
    // Before we hand it back it needs the nullchar otherwise strcmp() won't work
    no_dupes[pos] = '\0';
    return no_dupes;
}




    
    /*
    char* scopy;
    scopy=(char*)malloc(sizeof(char)*(strlen(s)+1));
    strcpy(scopy, s);
    int len=strlen(s);

    for(int j=0; j<len; j++){
        for(int i=j+1; i<len;){
            if(scopy[j]==scopy[i]){
                for(int x=i; x<len; x++){
                    scopy[x]=scopy[x+1];
                }
                len--;
            }
            else{
                i++;
            }
        }
    }
    return scopy;
    */
// }
