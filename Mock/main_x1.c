/*
   Tests whether string s1 is the same as string s2,
   but ignores the case of letters, and all punctuation
   (all non-letters, such as spaces, commas, ! etc.).
*/

#include "common.h"

bool strsimilar(const char* s1, const char* s2);

int main(void)
{
   // bool a=strsimilar("W-O-R-D.", "word?");

   assert(strsimilar("abc", "abc"));
   assert(strsimilar("abc", "AbC"));
   assert(strsimilar("abc!", "AbC"));
   assert(strsimilar("abc!", "!AbC"));
   assert(strsimilar("W-O-R-D.", "word?"));
   assert(strsimilar("!@##!", ""));
   assert(strsimilar("It's OK!", "itsok"));

   assert(!strsimilar("It OK!", "itsok"));
   assert(!strsimilar("aabc", "abc"));

   return 0;
}
