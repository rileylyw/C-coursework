#include "nlab.h"

int main(void){
   Program* prog = ncalloc(1, sizeof(Program));

   char file[BIGNUM];
   // verbose(file, argc, argv);
   char wds[MAXNUMTOKENS][MAXTOKENSIZE];
   readFile(file, prog);

   int i=0;
   while(scanf("%s", prog->wds[i++])==1 && i<MAXNUMTOKENS);
   assert(i<MAXNUMTOKENS);
   Prog(prog);
   printf("Parsed OK\n");
   free(prog);
   return 0;
}

void readFile(char file[], Program* p){
   FILE* fp=fopen(file, "r");
   if(fp==NULL){
      fprintf(stderr, "Cannot open file %s\n", file);
      exit(EXIT_FAILURE);
   }
   char buffer[BIGNUM];
   int i=0;
   while(fscanf(fp, "%s", buffer) != EOF){
      strcpy(p->wds[i++], buffer);
   }
   fclose(fp);
}

// void verbose(char file[BIGNUM], int argc, char* argv[]){
//    if(argc==3){
//       strcpy(file, argv[2]);
//       if(strcmp(argv[1], "-v")!=0){
//          fprintf(stderr, "Invalid Inputs\n");
//          exit(EXIT_FAILURE);
//       }
//    }
//    else if(argc==2){
//       strcpy(file, argv[1]);
//    }
// }

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
