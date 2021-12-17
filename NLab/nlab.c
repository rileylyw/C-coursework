#include "nlab.h"

int main(int argc, char* argv[]){
   Program* prog = ncalloc(1, sizeof(Program));
   char file[BIGNUM];
   verbose(file, argc, argv);
   readFile(file, prog);

   Prog(prog);
   printf("Parsed OK\n");
   free(prog);
   return 0;
}

void readFile(char file[], Program* p){
   FILE* fp=fopen(file, "r");
   if(fp==NULL){
      ERROR("Cannot open file");
   }
   char buffer[BIGNUM];
   int i=0;
   while(fscanf(fp, "%s", buffer) != EOF){
      strcpy(p->wds[i++], buffer);
   }
   fclose(fp);
}

void verbose(char file[BIGNUM], int argc, char* argv[]){
   if(argc==2){
      strcpy(file, argv[1]);
   }
   else if(argc==1){
      ERROR("No file");
   }
}

void Prog(Program *p){
   if(!strsame(p->wds[p->cw], "BEGIN")){
      ERROR("No BEGIN statement ?");
   }
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   // Instrclist(p);
}

// void Instrclist(Program *p){
//    if(strsame(p->wds[p->cw], "}")){
//       return;
//    }
//    // Instrc(p);
//    p->cw = p->cw + 1;
//    Instrclist(p);
// }

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
