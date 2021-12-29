#include "nlab.h"

void ExtractToken(char string[BIGNUM][BIGNUM], Program *p, int pos);
void Reset(Program *p);

int main(void){
   Program* prog = ncalloc(1, sizeof(Program));

   /*** TEST PROG ***/
   char string_prog_valid[BIGNUM][BIGNUM] = {
      {"BEGIN { }"},
      {"BEGIN { SET $A := 0 ; }"},
   };
   char string_prog_invalid[BIGNUM][BIGNUM] = {
      {"{ SET $A := 0 ; }"}, //missing BEGIN
      {"BEGIN SET $A := 0 ; }"} //missing {
   };
   int i = 0;
   while(string_prog_valid[i][0] != 0){
      ExtractToken(string_prog_valid, prog, i); //store string to prog
      assert(Prog(prog));
      Reset(prog);
      i++;
   }
   i = 0; //reset
   while(string_prog_invalid[i][0] != 0){
      ExtractToken(string_prog_invalid, prog, i);
      assert(!Prog(prog));
      Reset(prog);
      i++;
   }

   // /*** TEST INSTRCLIST ***/
   // char string_instrclist_valid[BIGNUM][BIGNUM] = {
   //    {"}"},
   //    {"PRINT $A }"},
   //    {"ONES 5 6 $A }"},
   // };
   // char string_instrclist_invalid[BIGNUM][BIGNUM] = {
   //    {"{ SET $A := 0 ; }"}, //2: invalid, has {
   //    {"BEGIN SET $A := 0 ; }"} //3: invalid, has BEGIN
   // };
   // i = 0; //reset
   // while(string_instrclist_valid[i][0] != 0){
   //    ExtractToken(string_instrclist_valid, prog, i); //store string to prog
   //    // printf("prog->wds[0]: %s\n", prog->wds[0]);
   //    assert(InstrcList(prog));
   //    Reset(prog);
   //    i++;
   // }
   // i = 0; //reset
   // while(string_instrclist_invalid[i][0] != 0){
   //    ExtractToken(string_instrclist_invalid, prog, i);
   //    // printf("prog->wds[0]: %s\n", prog->wds[0]);
   //    assert(!InstrcList(prog));
   //    Reset(prog);
   //    i++;
   // }

   // /*** TEST INSTRC ***/
   // char string_instrc_valid[BIGNUM][BIGNUM] = {
   //    {"PRINT $A"},
   //    {"SET $Z := 1000 ;"},
   //    {"SET $X := $Y ;"},
   //    {"READ \"test.arr\" $A"},
   //    {"LOOP $I 999 {"},
   //    {"ONES 10 10 $A"},
   // };
   // char string_instrc_invalid[BIGNUM][BIGNUM] = {
   //    {"READ"},
   //    {"SET $X = $Y"}, // no :=
   //    {"LOOP $I 999"}, // missing {
   //    {"LOOP $I A12 {"}, // not int
   //    {"ONES"},
   //    {"ONES 1 $Z"},
   //    {"ONES $I"},
   // };
   // i = 0; //reset
   // while(string_instrc_valid[i][0] != 0){
   //    ExtractToken(string_instrc_valid, prog, i); //store string to prog
   //    // printf("prog->wds[0]: %s\n", prog->wds[0]);
   //    assert(Instrc(prog));
   //    Reset(prog);
   //    i++;
   // }
   // i = 0; //reset
   // while(string_instrc_invalid[i][0] != 0){
   //    ExtractToken(string_instrc_invalid, prog, i);
   //    // printf("prog->wds[0]: %s\n", prog->wds[0]);
   //    assert(!Instrc(prog));
   //    Reset(prog);
   //    i++;
   // }



   free(prog);

   return EXIT_SUCCESS;
}

void ExtractToken(char string[BIGNUM][BIGNUM], Program *p, int pos){
   char* token = strtok(string[pos], " "); //TODO: fix valgrind error
   int i = 0;
   while(token != NULL){
      strcpy(p->wds[i++], token);
      // printf("%s\n", token); //printing each token
      token = strtok(NULL, " ");
   }
   // free(token);
}

void Reset(Program *p){
   memset(p->wds, 0, sizeof(p->wds));
   p->cw = 0;
}
