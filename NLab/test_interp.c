#include "nlab.h"

void test(void);

int main(void){
   // Program* prog = ncalloc(1, sizeof(Program));
   // char file[BIGNUM];
   // verbose(file, argc, argv);
   // readFile("set.nlb", prog);

   // Prog(prog);
   test();
//    #ifdef INTERP
//    for(int i=0; i<CHARACTER; i++){
//       if(prog->variable[i].num != NULL){
//          n2dfree(prog->variable[i].num, prog->variable[i].height); //TODO: matrix free
//       }
//    }
//    #endif

//    free(prog);
}




void test(void){
   Program* prog = ncalloc(1, sizeof(Program));
   readFile("set.nlb", prog);
   assert(strsame(Prog(prog),"1"));
   #ifdef INTERP
   for(int i=0; i<CHARACTER; i++){
      if(prog->variable[i].num != NULL){
         n2dfree(prog->variable[i].num, prog->variable[i].height); //TODO: matrix free
      }
   }
   #endif

   free(prog);
}
