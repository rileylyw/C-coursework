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
   p->cw = p->cw + 1;
   InstrcList(p);
   return true;
}

bool Instrc(Program *p){
   if(strsame(p->wds[p->cw], "PRINT")){
      // printf("Instrc %s\n", p->wds[p->cw]);
      Print(p);
      return true;
   }
   if(strsame(p->wds[p->cw], "SET")){
      Set(p);
      return true;
   }
   if(strsame(p->wds[p->cw], "ONES") || strsame(p->wds[p->cw], "READ")){
      Create(p);
      return true;
   }
   if(strsame(p->wds[p->cw], "LOOP")){
      Loop(p);
      return true;
   }
   ERROR("Wrong instruction");
}

bool Print(Program *p){
   p->cw = p->cw + 1;
   if(Varname(p)){
      return true;
   }
   else if(String(p)){
      printf("String %s\n", p->wds[p->cw]);
      return true;
   }
   ERROR("Print error");
}

bool Varname(Program *p){
   int len = strlen(p->wds[p->cw]);
   char* var = (char*)ncalloc(len+1, sizeof(char));
   strcpy(var, p->wds[p->cw]);  //TODO: if $ABC
   printf("varname: %s\n", var);
   if(!(var[0] == '$') || var[1]<'A' || var[1]>'Z'){
      free(var);
      return false;
      // ERROR("Wrong variable definition");
   }
   // printf("var: %c\n", var[0]);
   free(var);
   return true;
}

bool String(Program *p){
   int len = strlen(p->wds[p->cw]);
   char* string = (char*)ncalloc(len+1, sizeof(char));
   strcpy(string, p->wds[p->cw]);
   printf("string: %s\n", string);
   // printf("string[0]: %c\n", string[0]);
   // printf("string[last]: %c\n", string[strlen(p->wds[p->cw])-1]);
   int count = 0;
   for(int i=0; i<len; i++){
      if(string[i] == '"'){
         count++;
      }
   }
   if(count != 2){
   // if(!((string[0] == '"') && (string[strlen(p->wds[p->cw])-1] == '"'))){
      free(string);
      return false;
      // ERROR("string error");
   }
   free(string);
   return true;
}

bool Set(Program *p){
   p->cw = p->cw + 1; //$I
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
   if(strsame(p->wds[p->cw], ";")){ //5
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
      printf("PushDown %s\n", p->wds[p->cw]);
      return true;
   }
   else if(UnaryOp(p)){
      printf("UnaryOp %s\n", p->wds[p->cw]);
      return true;
   }
   else if(BinaryOp(p)){
      printf("BinaryOp %s\n", p->wds[p->cw]);
      return true;
   }
   ERROR("Polish error");
}

bool PushDown(Program *p){
   if(Varname(p)){
      printf("varname %s\n", p->wds[p->cw]);
      return true;
   }
   else if(Integer(p)){
      printf("int %s\n", p->wds[p->cw]);
      return true;
   }
   return false;
}

bool Integer(Program *p){
   char var[BIGNUM];
   strcpy(var, p->wds[p->cw]); //e.g. 522
   for(unsigned int i=0; i<strlen(var); i++){
      if(!(var[i]<='9' && var[i]>='0')){
         // printf("var %s\n", var);
         return false;
         // ERROR("Not an integer / non-negative integer");
      }
   }
   return true;
}

bool UnaryOp(Program *p){
   if(strsame(p->wds[p->cw], "U-NOT")){
      return true;
   }
   else if(strsame(p->wds[p->cw], "U-EIGHTCOUNT")){
      return true;
   }
   return false;
   // ERROR("UnaryOp error");
}

bool BinaryOp(Program *p){
   if(strsame(p->wds[p->cw], "B-AND")){
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-OR")){
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-GREATER")){
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-LESS")){
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-ADD")){
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-TIMES")){
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-EQUALS")){
      return true;
   }
   return false;
   // ERROR("BinaryOp error");
}

bool Create(Program *p){
   if(strsame(p->wds[p->cw], "ONES")){
      p->cw = p->cw + 1;
      printf("Rows %s\n", p->wds[p->cw]);
      Rows(p);
      p->cw = p->cw + 1;
      printf("Cols %s\n", p->wds[p->cw]);
      Cols(p);
      p->cw = p->cw + 1;
      Varname(p);
      return true;
   }
   else{
      p->cw = p->cw + 1;
      FileName(p);
      p->cw = p->cw + 1;
      Varname(p);
      return true;
   }
   ERROR("Create error");
}

bool Rows(Program *p){
   if(Integer(p)){
      return true;
   }
   ERROR("Rows error");
}

bool Cols(Program *p){
   if(Integer(p)){
      return true;
   }
   ERROR("Cols error");
}

bool FileName(Program *p){
   if(String(p)){
      return true;
   }
   ERROR("FileName error");
}

bool Loop(Program *p){
   p->cw = p->cw + 1;
   Varname(p);
   p->cw = p->cw + 1;
   Integer(p);
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   p->cw = p->cw + 1;
   InstrcList(p);
   return true;
}
