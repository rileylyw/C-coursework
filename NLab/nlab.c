#include "nlab.h"

void readFile(char file[], Program* p){
   FILE* fp=fopen(file, "r");
   if(fp==NULL){
      fprintf(stderr, "Cannot open file");
      // ERROR("Cannot open file");
   }
   char buffer[BIGNUM];
   int i=0;
   while(fscanf(fp, "%s", buffer) != EOF){
      strcpy(p->wds[i++], buffer);
   }
   fclose(fp);
}

bool verbose(char file[BIGNUM], int argc, char* argv[]){
   if(argc==2){
      strcpy(file, argv[1]);
   }
   else if(argc==1){
      fprintf(stderr, "No file");
      // ERROR("No file");
   }
   return true;
}

bool Prog(Program *p){
   if(!strsame(p->wds[p->cw], "BEGIN")){
      ERROR("No BEGIN statement ?");
   }
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   p->cw = p->cw + 1;
   Instrclist(p);
   return true;
}

bool Instrclist(Program *p){
   if(!strsame(p->wds[p->cw], "}")){
      ERROR("Missing }");
   }
   Instrc(p);
   p->cw = p->cw + 1;
   Instrclist(p);
   return true;
}

bool Instrc(Program *p){
   if(strsame(p->wds[p->cw], "PRINT")){
      return true;
   }
   if(strsame(p->wds[p->cw], "SET")){
      return true;
   }
   if(strsame(p->wds[p->cw], "CREATE")){
      return true;
   }
   if(strsame(p->wds[p->cw], "LOOP")){
      return true;
   }
   return false;
}

bool Set(Program *p){
   // p->wds[p->cw]
   // if(!strsame(p->wds[p->cw], "SET")){
   //    ERROR("Missing SET function");
   // }
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], ":=")){
      ERROR("Incorrect operator");
   }
   p->cw = p->cw + 1;
   Instrclist(p);
   return true;
}

bool Varname(Program *p){

}