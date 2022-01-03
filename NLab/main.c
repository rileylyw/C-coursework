#include "nlab.h"

int main(int argc, char* argv[]){
   Program* prog = ncalloc(1, sizeof(Program));
   char file[BIGNUM];
   verbose(file, argc, argv);
   readFile(file, prog);

   Prog(prog);
   
   #ifdef INTERP
   // printf("Parsed OK\n");
   for(int i=0; i<CHARACTER; i++){
      if(prog->variable[i] != NULL){
         n2dfree(prog->variable[i]->num, 1); //TODO: matrix
         free(prog->variable[i]);
         printf("%s", "INTERP OK");
      }
   }
   #endif

   free(prog);
   return 0;
}
