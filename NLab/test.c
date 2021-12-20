#include "nlab.h"

int main(int argc, char* argv[]){
   Program* prog = ncalloc(1, sizeof(Program));
   // char file[BIGNUM];
   // verbose(file, argc, argv);
   // readFile(file, prog);
   strcpy(prog->wds[0], "BEGIN");
   strcpy(prog->wds[1], "{");
   strcpy(prog->wds[2], "{");

   assert(Prog(prog));
   // printf("b %s\n", prog->wds[1]);
   // printf("%s\n", prog->wds[prog->cw]);
   assert(!Instrclist(prog));

   free(prog);

   return EXIT_SUCCESS;
}
