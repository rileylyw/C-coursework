#include "nlab.h"

int main(int argc, char* argv[]){
   Program* prog = ncalloc(1, sizeof(Program));
   char file[BIGNUM];
   verbose(file, argc, argv);
   readFile(file, prog);

   Prog(prog);
   
   #ifdef INTERP
   for(int i=0; i<CHARACTER; i++){
      if(prog->variable[i].num != NULL){
         n2dfree(prog->variable[i].num, prog->variable[i].height); //TODO: matrix free
         // free(prog->variable[i]);
         // printf("INTERP OK\n");
      }
   }
   #endif

   free(prog);
   return 0;
}
