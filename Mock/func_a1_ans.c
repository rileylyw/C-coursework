#include "common.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Easy to write solution
/*
bool struses(const char* s1, const char* s2){
	if(strlen(s1)== 0){
		return true;
	}
	int l1 = strlen(s1);
	int l2 = strlen(s2);
	bool matches;
	for(int i=0; i< l1; i++){
		matches = false;
		for(int j=0; j<l2; j++){
			if(s1[i] == s2[j]){
				matches = true;
			}
		}
		if(!matches){
			return false;
		}
	}
	return true;
}
*/
// Neill's efficient solution

bool struses(const char* s1, const char* s2)
{
	bool letters[26] = {false};

	int l = strlen(s2);
	for(int i=0; i<l; i++){
		letters[tolower(s2[i]) - 'a'] = true;
	}
	l = strlen(s1);
	char current_letter;
	for(int i=0; i<l; i++){
		current_letter = tolower(s1[i]);
		if(letters[current_letter - 'a'] == false){
			return false;
		}
	}
	return true;
}

