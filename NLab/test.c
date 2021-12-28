#include "nlab.h"

void ExtractToken(char string[BIGNUM][BIGNUM], Program *p, int pos);
void Reset(Program *p);

int main(void){
   Program* prog = ncalloc(1, sizeof(Program));

   /* TEST PROG */
   char string_prog[BIGNUM][BIGNUM] = {
      {"BEGIN { }"}, //0: valid
      {"BEGIN { SET $A := 0 ; }"}, //1: valid
      {"{ SET $A := 0 ; }"}, //2: invalid, missing BEGIN
      {"BEGIN SET $A := 0 ; }"} //3: invalid, missing {
   };

   ExtractToken(string_prog, prog, 0); //store string to prog
   assert(Prog(prog));
   Reset(prog);

   ExtractToken(string_prog, prog, 1);
   assert(Prog(prog));
   Reset(prog);

   ExtractToken(string_prog, prog, 2);
   assert(!Prog(prog));
   Reset(prog);

   ExtractToken(string_prog, prog, 3);
   assert(!Prog(prog));
   Reset(prog);

   /* TEST INSTRCLIST */
   char string_instrclist[BIGNUM][BIGNUM] = {
      {"}"}, //0: valid
      {"PRINT $A }"}, //1: valid
      {"{ SET $A := 0 ; }"}, //2: invalid, has {
      {"BEGIN SET $A := 0 ; }"} //3: invalid, has BEGIN
   };
   ExtractToken(string_instrclist, prog, 0);
   assert(InstrcList(prog));
   Reset(prog);

   ExtractToken(string_instrclist, prog, 1);
   assert(InstrcList(prog));
   Reset(prog);

   ExtractToken(string_instrclist, prog, 2);
   assert(!InstrcList(prog));
   Reset(prog);

   ExtractToken(string_instrclist, prog, 3);
   assert(!InstrcList(prog));
   Reset(prog);
   // printf("test prog->wds[0]: %s\n", prog->wds[0]);

   /* TEST INSTRC */
   char string_instrc[BIGNUM][BIGNUM] = {
      {"PRINT $A"}, //0: valid
      {"SET $Z := 1000 ;"}, //1: valid
      {"SET $X := $Y ;"}, //2: valid
      {"READ \"test.arr\" $A"}, //3: valid
      {"READ"}, //4: invalid
      {"SET $X = $Y"}, //5: invalid, no :=
      {"LOOP $I 999 {"}, //6: valid
      {"LOOP $I 999"}, //7: invalid, missing {
      {"LOOP $I A12 {"}, //8: invalid, not int
      {"ONES 10 10 $A"}, //9: valid
      {"ONES"}, //10: invalid
      {"ONES 1 $Z"}, //11: invalid
      {"ONES $I"}, //12: invalid
   };
   ExtractToken(string_instrc, prog, 0);
   assert(Instrc(prog));
   Reset(prog);
   // printf("prog->cw: %d\n", prog->cw);
   // printf("prog->wds[0]: %s\n", prog->wds[0]);

   ExtractToken(string_instrc, prog, 1);
   assert(Instrc(prog));
   Reset(prog); 

   ExtractToken(string_instrc, prog, 2);
   assert(Instrc(prog));
   Reset(prog);

   ExtractToken(string_instrc, prog, 3);
   assert(Instrc(prog));
   Reset(prog);

   ExtractToken(string_instrc, prog, 4);
   assert(!Instrc(prog));
   Reset(prog);
   
   ExtractToken(string_instrc, prog, 5);
   assert(!Instrc(prog));
   Reset(prog);

   ExtractToken(string_instrc, prog, 6);
   assert(Instrc(prog));
   Reset(prog);

   ExtractToken(string_instrc, prog, 7);
   assert(!Instrc(prog));
   Reset(prog);

   ExtractToken(string_instrc, prog, 8);
   assert(!Instrc(prog));
   Reset(prog);

   ExtractToken(string_instrc, prog, 9);
   assert(Instrc(prog));
   Reset(prog);

   ExtractToken(string_instrc, prog, 10);
   assert(!Instrc(prog));
   Reset(prog);

   ExtractToken(string_instrc, prog, 11);
   assert(!Instrc(prog));
   Reset(prog);

   ExtractToken(string_instrc, prog, 12);
   assert(!Instrc(prog));
   Reset(prog);

   free(prog);

   return EXIT_SUCCESS;
}

void ExtractToken(char string[BIGNUM][BIGNUM], Program *p, int pos){
   char* token = strtok(string[pos], " ");
   int i = 0;
   while(token != NULL){
      strcpy(p->wds[i++], token);
      // printf("%s\n", token); //printing each token
      token = strtok(NULL, " ");
   }
}

void Reset(Program *p){
   memset(p->wds, 0, sizeof(p->wds));
   p->cw = 0;
}
