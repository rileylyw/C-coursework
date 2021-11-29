#include "common.h"


/*
   Returns a copy of string s1 with all of the duplicate
   characters removed.
*/
char* strnodupes(const char* s)
{
    int l = strlen(s);
    bool unique;
    // Allocate space for new string. Can only be at max the same length of the input string
    char* no_dupes = malloc(sizeof(char) * l+1);
    // Create an index for where to store the unique characters
    int pos = 0;
    // Check malloc worked
    if(!no_dupes){
        printf("Couldn't malloc space");
        exit(EXIT_FAILURE);
    }

    // We will need to loop through all the characters in the string
    for(int i=0; i<l; i++){
        // We will start off assuming the current character is unique each time and perform checks to disprove it
        unique = true;
        // To check if it's a duplicate, we look at every character behind the current one for a match
        for(int j=i-1; j>=0; j--){
            // When looking at the characters behind, we should set unique to false if we find a match
            if(s[j]==s[i]){
                unique = false;
            }
        }
        // Finished checking characters behind, now we see if uique was changed to false
        if(unique){
            // If unique was still true, we can add the current character into our string to return
            no_dupes[pos++] = s[i];
        }
    }
    // Before we hand it back it needs the nullchar otherwise strcmp() won't work
    no_dupes[pos] = '\0';
    return no_dupes;
}
