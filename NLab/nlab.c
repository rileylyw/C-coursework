#include "nlab.h"

void readFile(char file[], Program* p){
   FILE* fp=fopen(file, "r");
   if(fp==NULL){
      fprintf(stderr, "Cannot open file");
   }
   char buffer[BIGNUM];
   int j=0;
   while(fscanf(fp, "%s", buffer) != EOF){
      strcpy(p->wds[j++], buffer);
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
   if(strsame(p->wds[p->cw], "}")){
      return true;
   }
   Instrc(p);
   // printf("string: %s\n", p->wds[p->cw]); //TODO
   // printf("%s\n", Instrc(p)?"TRUE":"FALSE");
   p->cw = p->cw + 1;
   // printf("string: %s\n", p->wds[p->cw]); //TODO
   Instrclist(p);
   return true;
}

bool Instrc(Program *p){
   if(strsame(p->wds[p->cw], "PRINT")){
      return true;
   }
   if(strsame(p->wds[p->cw], "SET")){
      Set(p);
      return true;
   }
   if(strsame(p->wds[p->cw], "CREATE")){
      return true;
   }
   if(strsame(p->wds[p->cw], "LOOP")){
      return true;
   }
   ERROR("Wrong instruction");
}

bool Set(Program *p){
   p->cw = p->cw + 1;
   // printf("p->wds[p->cw] %s\n", p->wds[p->cw]);
   char var[2];
   strcpy(var, p->wds[p->cw]); //e.g. $A
   if(!strcmp(var[0], "$")==0 || var[1]<'A' || var[1]>'Z'){ //TODO
      ERROR("Wrong variable definition");
   }
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], ":=")){
      ERROR("Incorrect operator");
   }
   p->cw = p->cw + 1;
   Instrclist(p);
   return true;
}

// bool Varname(Program *p){

// }
