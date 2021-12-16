#include "nlab.h"

int main(void){
   Program* prog = ncalloc(1, sizeof(Program));
   int i=0;
   while(scanf("%s", prog->wds[i++])==1 && i<MAXNUMTOKENS);
   assert(i<MAXNUMTOKENS);
   Prog(prog);
   printf("Parsed OK\n");
   free(prog);
   return 0;
}

void Prog(Program *p){
   if(!strsame(p->wds[p->cw], "BEGIN")){
      ERROR("No BEGIN statement ?");
   }
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   Instrclist(p);
}

void Instrclist(Program *p){
   if(strsame(p->wds[p->cw], "}")){
      return;
   }
   Instrc(p);
   p->cw = p->cw + 1;
   Instrclist(p);
}

// void Code(Program *p)
// {
//    if(strsame(p->wds[p->cw], "END")){
//       return;
//    }
//    Statement(p);
//    p->cw = p->cw + 1;
//    Code(p);
// }

// void Statement(Program *p)
// {
//    if(strsame(p->wds[p->cw], "ONE")){
//       return;
//    }
//    if(strsame(p->wds[p->cw], "NOUGHT")){
//       return;
//    }
//    ERROR("Expecting a ONE or NOUGHT ?");
// }
