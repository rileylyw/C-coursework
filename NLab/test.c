#include "nlab.h"
#define NUM 100

void ExtractToken(char string[NUM][NUM], Program *p, int pos);
void Reset(Program *p);

int main(void){
   Program* prog = ncalloc(1, sizeof(Program));

   /*** TEST PROG ***/
   char string_prog_valid[NUM][NUM] = {
      {"BEGIN { }"},
      {"BEGIN { SET $A := 0 ; }"},
   };
   char string_prog_invalid[NUM][NUM] = {
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

   /*** TEST INSTRCLIST ***/
   char string_instrclist_valid[NUM][NUM] = {
      {"}"},
      {"PRINT $A }"},
      {"ONES 5 6 $A }"},
   };
   char string_instrclist_invalid[NUM][NUM] = {
      {"{ SET $A := 0 ; }"}, //2: invalid, has {
      {"BEGIN SET $A := 0 ; }"} //3: invalid, has BEGIN
   };
   i = 0; 
   while(string_instrclist_valid[i][0] != 0){
      ExtractToken(string_instrclist_valid, prog, i); //store string to prog
      assert(InstrcList(prog));
      Reset(prog);
      i++;
   }
   i = 0; 
   while(string_instrclist_invalid[i][0] != 0){
      ExtractToken(string_instrclist_invalid, prog, i);
      assert(!InstrcList(prog));
      Reset(prog);
      i++;
   }

   /*** TEST INSTRC ***/
   char string_instrc_valid[NUM][NUM] = {
      {"PRINT $A"},
      {"SET $Z := 1000 ;"},
      {"SET $X := $Y ;"},
      {"READ \"test.arr\" $A"},
      {"LOOP $I 999 {"},
      {"ONES 10 10 $A"},
   };
   char string_instrc_invalid[NUM][NUM] = {
      {"READ"},
      {"SET $X = $Y"}, // no :=
      {"LOOP $I 999"}, // missing {
      {"LOOP $I A12 {"}, // not int
      {"ONES"},
      {"ONES 1 $Z"},
      {"ONES $I"},
   };
   i = 0; 
   while(string_instrc_valid[i][0] != 0){
      ExtractToken(string_instrc_valid, prog, i); //store string to prog
      assert(Instrc(prog));
      Reset(prog);
      i++;
   }
   i = 0; 
   while(string_instrc_invalid[i][0] != 0){
      ExtractToken(string_instrc_invalid, prog, i);
      assert(!Instrc(prog));
      Reset(prog);
      i++;
   }

   /*** TEST PRINT ***/
   char string_print_valid[NUM][NUM] = {
      {"$A"},
      {"\"../..testing123!!!\""},
   };
   char string_print_invalid[NUM][NUM] = {
      {"$ZZ"},
      {"$0"},
      {"A"},
      {" "},
   };
   i = 0; 
   while(string_print_valid[i][0] != 0){
      ExtractToken(string_print_valid, prog, i); //store string to prog
      assert(Print(prog));
      Reset(prog);
      i++;
   }
   i = 0; 
   while(string_print_invalid[i][0] != 0){
      ExtractToken(string_print_invalid, prog, i);
      assert(!Print(prog));
      Reset(prog);
      i++;
   }

   /*** TEST VARNAME ***/
   char string_varname_valid[NUM][NUM] = {
      {"$A"},
   };
   char string_varname_invalid[NUM][NUM] = {
      {"$ZZ"},
      {"$0"},
      {"A"},
      {" "},
   };
   i = 0; 
   while(string_varname_valid[i][0] != 0){
      ExtractToken(string_varname_valid, prog, i); //store string to prog
      assert(Varname(prog));
      Reset(prog);
      i++;
   }
   i = 0; 
   while(string_varname_invalid[i][0] != 0){
      ExtractToken(string_varname_invalid, prog, i);
      assert(!Varname(prog));
      Reset(prog);
      i++;
   }

   /*** TEST STRING ***/
   char string_string_valid[NUM][NUM] = {
      {"\"TESING!!!\""},
      {"\".;./asd!@#\""},
      {"\"1\""},
   };
   char string_string_invalid[NUM][NUM] = {
      {"TESING"}, //no ""
      {" "},
      // {""}, //TODO: empty string
   };
   i = 0; 
   while(string_string_valid[i][0] != 0){
      ExtractToken(string_string_valid, prog, i); //store string to prog
      assert(String(prog));
      Reset(prog);
      i++;
   }
   i = 0; 
   while(string_string_invalid[i][0] != 0){
      ExtractToken(string_string_invalid, prog, i);
      assert(!String(prog));
      Reset(prog);
      i++;
   }

   /*** TEST SET ***/
   char string_set_valid[NUM][NUM] = {
      {"$A := $A 2 B-ADD ;"},
      {"$F := 1 ;"},
   };
   char string_set_invalid[NUM][NUM] = {
      {"$A := $A 2 B-ADD"}, //missing ;
      {"$A = $A 2 B-ADD"}, //missing :=
      {"$A $A 2 B-ADD"}, //missing :=
      {"$0 := $A 2 B-ADD"}, //not var for polishlist func
      {" "}, //no content
   };
   i = 0; 
   while(string_set_valid[i][0] != 0){
      ExtractToken(string_set_valid, prog, i); //store string to prog
      assert(Set(prog));
      Reset(prog);
      i++;
   }
   i = 0; 
   while(string_set_invalid[i][0] != 0){
      ExtractToken(string_set_invalid, prog, i);
      assert(!Set(prog));
      Reset(prog);
      i++;
   }

/*** TEST POLISHLIST ***/
   char string_polishlist_valid[NUM][NUM] = {
      {";"},
      {"1 ;"}, //can pushdown
      {"$A ;"}, //can pushdown
      {"U-NOT ;"}, //can unaryop
      {"B-ADD ;"}, //can binaryop
   };
   char string_polishlist_invalid[NUM][NUM] = {
      {"B-ADD"}, //missing ;
      {"A ;"}, //not var
      {"!!! ;"}, //not var nor int
      {" "}, //no content
   };
   i = 0; 
   while(string_polishlist_valid[i][0] != 0){
      ExtractToken(string_polishlist_valid, prog, i); //store string to prog
      // printf("prog->wds[0]: %s\n", prog->wds[0]);
      assert(PolishList(prog));
      Reset(prog);
      i++;
   }
   i = 0; 
   while(string_polishlist_invalid[i][0] != 0){
      ExtractToken(string_polishlist_invalid, prog, i);
      // printf("prog->wds[0]: %s\n", prog->wds[0]);
      assert(!PolishList(prog));
      Reset(prog);
      i++;
   }

/*** TEST POLISH ***/
   char string_polish_valid[NUM][NUM] = {
      {"1"}, //int
      {"$Z"}, //var
      {"U-NOT"}, //unaryop
      {"B-ADD"}, //binaryop
   };
   char string_polish_invalid[NUM][NUM] = {
      {"A"}, //not var
      {"!!!"}, //not var nor int
      {" "}, //????
   };
   i = 0; 
   while(string_polish_valid[i][0] != 0){
      ExtractToken(string_polish_valid, prog, i); //store string to prog
      // printf("prog->wds[0]: %s\n", prog->wds[0]);
      assert(Polish(prog));
      Reset(prog);
      i++;
   }
   i = 0; 
   while(string_polish_invalid[i][0] != 0){
      ExtractToken(string_polish_invalid, prog, i);
      // printf("prog->wds[0]: %s\n", prog->wds[0]);
      assert(!Polish(prog));
      Reset(prog);
      i++;
   }


   free(prog);

   return EXIT_SUCCESS;
}

void ExtractToken(char string[NUM][NUM], Program *p, int pos){
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
   memset(p->wds, 0, sizeof(p->wds[0][0])*MAXTOKENSIZE*MAXNUMTOKENS);
   // memset(p->wds, 0, sizeof(p->wds)*NUM);
   // memset(p->wds, 0, sizeof(p->wds));
   p->cw = 0;
}
