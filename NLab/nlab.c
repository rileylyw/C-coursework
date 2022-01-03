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
      // fprintf(stderr, "No file");
      ERROR("No file");
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
   if(!Instrc(p)){
      ERROR("Instuclist error");
   }
   p->cw = p->cw + 1;
   InstrcList(p);
   return true;
}

bool Instrc(Program *p){
   if(strsame(p->wds[p->cw], "PRINT")){
      // printf("Instrc %s\n", p->wds[p->cw]);
      p->cw = p->cw + 1;
      if(Print(p)){
         return true;
      };
      ERROR("PRINT error");
   }
   if(strsame(p->wds[p->cw], "SET")){
      p->cw = p->cw + 1;
      if(Set(p)){
         return true;
      }
      ERROR("SET error");
   }
   if(strsame(p->wds[p->cw], "ONES") || strsame(p->wds[p->cw], "READ")){
      if(Create(p)){
         return true;
      }
      ERROR("CREATE error");
   }
   if(strsame(p->wds[p->cw], "LOOP")){
      p->cw = p->cw + 1;
      if(Loop(p)){
         return true;
      };
      ERROR("LOOP error");
   }
   ERROR("Wrong instruction");
}

bool Print(Program *p){
   // p->cw = p->cw + 1;
   if(Varname(p)){
      return true;
   }
   else if(String(p)){
      // printf("String %s\n", p->wds[p->cw]);
      return true;
   }
   ERROR("Print error");
}

bool Varname(Program *p){
   int len = strlen(p->wds[p->cw]);
   if(len != 2){
      return false;
      // ERROR("Wrong varname2");
   }

   char* var = (char*)ncalloc(len+1, sizeof(char));
   strcpy(var, p->wds[p->cw]);

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
   if(len == 0 || strsame(p->wds[p->cw], " ")){
      ERROR("No string or empty");
   }
   char* string = (char*)ncalloc(len+1, sizeof(char));
   strcpy(string, p->wds[p->cw]);
   // printf("string: %s\n", string);
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
   // p->cw = p->cw + 1; //$I
   // printf("p->wds[p->cw] %s\n", p->wds[p->cw]);
   if(!Varname(p)){
      // printf("set: %s\n", p->wds[p->cw]);
      ERROR("Wrong varname");
   }
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], ":=")){
      ERROR("Incorrect operator");
   }
   p->cw = p->cw + 1;
   if(PolishList(p)){
      return true;
   }
   return false;
   // if(!PolishList(p)){
   //    printf("%s\n", "Polishlist true");
   //    ERROR("PolishList error");
   // }
   // printf("TESTING4 %s\n", p->wds[p->cw]);
   // return true;
}

bool PolishList(Program *p){
   if(p->wds[p->cw] && !p->wds[p->cw][0]){
      ERROR("NULL");
   }
   if(strsame(p->wds[p->cw], ";")){
      // printf("TESTING3 %s\n", p->wds[p->cw]);
      return true;
   }
   if(!Polish(p)){
      // printf("%s\n", "TEST");
      ERROR("Polish error");
   }
   p->cw = p->cw + 1;
   // printf("TESTING %d\n", p->wds[p->cw][0]);
   // if(p->wds[p->cw] && !p->wds[p->cw][0]){
   //    printf("TESTING2 %d\n", p->wds[p->cw][0]);
   //    ERROR("NULL");
   // }
   PolishList(p);
   // printf("%s\n", "TEST");
   if(!(strsame(p->wds[p->cw], ";"))){
      ERROR("Missing ;");
   }
   return true;
}

bool Polish(Program *p){
   if(PushDown(p)){
      // printf("PushDown %s\n", p->wds[p->cw]);
      return true;
   }
   else if(UnaryOp(p)){
      // printf("UnaryOp %s\n", p->wds[p->cw]);
      return true;
   }
   else if(BinaryOp(p)){
      // printf("BinaryOp %s\n", p->wds[p->cw]);
      return true;
   }
   ERROR("Polish error");
}

bool PushDown(Program *p){
   if(Varname(p)){
      // printf("varname %s\n", p->wds[p->cw]);
      return true;
   }
   else if(Integer(p)){
      #ifdef INTERP
      // printf("int: %s\n", p->wds[p->cw]);
      
      int pos = (int)(p->wds[p->cw-2][1]) - 'A'; //respective var
      p->variable[pos] = (var*) ncalloc(1, sizeof(var));
      p->variable[pos]->num = (int**)n2dcalloc(1, 1, sizeof(int*));
      p->variable[pos]->height = 1;
      p->variable[pos]->width = 1;
      for(int j=0; j<p->variable[pos]->height; j++){ //row
         for(int i=0; i<p->variable[pos]->width; i++){ //col
            p->variable[pos]->num[j][i] = p->wds[p->cw][0] - '0';
            printf("word: %d\n", p->variable[pos]->num[j][i]);
         }
      }
      #endif

      // printf("int: %d\n", p->variable[0]->height);
      // n2dfree(p->variable[pos]->num, 1);
      // free(p->variable[pos]);
      return true;
   }
   return false;
}

bool Integer(Program *p){
   if(p->wds[p->cw][0] == 0){
      return false;
   }
   char var[BIGNUM];
   strcpy(var, p->wds[p->cw]); //e.g. 522
   // printf("vartest %s\n", var);
   // printf("vartest len %lu\n", strlen(var));
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
   // return false;
   ERROR("BinaryOp error");
}

bool Create(Program *p){
   if(strsame(p->wds[p->cw], "ONES")){
      p->cw = p->cw + 1;
      // printf("Row %s\n", p->wds[p->cw]);
      if(!Row(p)){
         ERROR("Row error");
      }
      p->cw = p->cw + 1;
      // printf("Col %s\n", p->wds[p->cw]);
      if(!Col(p)){
         ERROR("Col error");
      }
      p->cw = p->cw + 1;
      if(!Varname(p)){
         ERROR("Varname error");
      }
      return true;
   }
   else if(strsame(p->wds[p->cw], "READ")){
      p->cw = p->cw + 1;
      if(!FileName(p)){
         ERROR("FileName error");
      }
      p->cw = p->cw + 1;
      if(!Varname(p)){
         ERROR("Varname error");
      }
      return true;
   }
   ERROR("Create error");
}

bool Row(Program *p){
   if(Integer(p)){
      return true;
   }
   ERROR("Row error");
}

bool Col(Program *p){
   if(Integer(p)){
      return true;
   }
   ERROR("Col error");
}

bool FileName(Program *p){
   if(String(p)){
      return true;
   }
   ERROR("FileName error");
}

bool Loop(Program *p){
   // p->cw = p->cw + 1;
   if(!Varname(p)){
      ERROR("Varname error");
   }
   p->cw = p->cw + 1;
   if(!Integer(p)){
      ERROR("Integer error");
   }
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   p->cw = p->cw + 1;
   InstrcList(p);
   return true;
}
