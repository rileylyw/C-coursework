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
   InstrcList(p);
   return true;
}

bool InstrcList(Program *p){
   if(strsame(p->wds[p->cw], "}")){
      return true;
   }
   Instrc(p);
   // printf("string: %s\n", p->wds[p->cw]); //TODO
   // printf("%s\n", Instrc(p)?"TRUE":"FALSE");
   p->cw = p->cw + 1;
   // printf("string: %s\n", p->wds[p->cw]); //TODO
   InstrcList(p);
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


bool Varname(Program *p){
   char var[2];
   strcpy(var, p->wds[p->cw]); //e.g. $A
   // printf("var: %c\n", var[0]);
   // printf("var: %s\n", var);
   if(!(var[0] == '$') || var[1]<'A' || var[1]>'Z'){
      ERROR("Wrong variable definition");
   }
   // printf("var: %c\n", var[0]);
   return true;
}

bool Set(Program *p){
   p->cw = p->cw + 1;
   // printf("p->wds[p->cw] %s\n", p->wds[p->cw]);
   if(!Varname(p)){
      ERROR("Wrong varname");
   }
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], ":=")){
      ERROR("Incorrect operator");
   }
   p->cw = p->cw + 1;
   PolishList(p);
   return true;
}

bool PolishList(Program *p){
   // printf("begin %s\n", p->wds[p->cw]);
   if(strsame(p->wds[p->cw], ";")){
      // printf("; %s\n", p->wds[p->cw]);
      return true;
   }
   if(!Polish(p)){
      ERROR("Polish error");
   }
   p->cw = p->cw + 1;
   PolishList(p);
   return false;
}

bool Polish(Program *p){
   if(PushDown(p)){
      return true;
   }
   else if(UnaryOp(p)){
      return true;
   }
   else if(BinaryOp(p)){
      return true;
   }
   ERROR("Polish error");
}

bool PushDown(Program *p){
   return true;
}

bool UnaryOp(Program *p){
   return true;
}

bool BinaryOp(Program *p){
   return true;
}
