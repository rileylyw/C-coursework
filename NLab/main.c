#include "nlab.h"

int main(int argc, char* argv[]){
   Program* prog = ncalloc(1, sizeof(Program));
   char file[BIGNUM];
   verbose(file, argc, argv);
   readFile(file, prog);

   Prog(prog);
   
   #ifdef INTERP
   printf("Parsed OK\n");
   #endif
   free(prog);
   return 0;
}
