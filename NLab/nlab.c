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
   #ifdef INTERP
   p->stack = stack_init();
   #endif
   if(!strsame(p->wds[p->cw], "BEGIN")){
      ERROR("No BEGIN statement ?");
   }
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   p->cw = p->cw + 1;
   InstrcList(p);
   #ifdef INTERP
   // printf("%d\n", p->pos);
   stack_free(p->stack);
   #endif
   return true;
}

bool InstrcList(Program *p){
   if(strsame(p->wds[p->cw], "}")){
      // var tempvar1;
      // stack_pop(p->stack, &tempvar1);

      // p->variable[p->looppos].num[0][0] += 1;
      // printf("size %d\n", p->stack->a[1].loopstart);
      // printf("here %d\n", p->loopstart);
      // printf("here %d\n", p->variable[p->looppos].num[0][0]);
      // printf("p->wds[p->cw] %s\n", p->wds[p->cw]);
      // if(p->variable[p->looppos].num[0][0]<=p->maxloop){
      
      // if(p->variable[p->looppos].loopcount<=p->variable[p->looppos].maxloop){
      //    p->cw = p->variable[p->looppos].loopstart; //TODO
      //    // printf("here %d\n", p->variable[p->looppos].loopcount);
      //    p->variable[p->looppos].loopcount++;
      //    printf("here %d\n", p->looppos);
      //    // printf("here %d\n", p->variable[p->looppos].loopcount);
      //    // stack_push(p->stack, &tempvar1);
      //    // n2dfree(tempvar1.num, tempvar1.height);
      //    InstrcList(p);
      //    // printf("p->wds[p->cw] %s\n", p->wds[p->cw]);
      // }

      return true;
   }
   if(!Instrc(p)){
      return false;
      // ERROR("Instuclist error");
   }
   p->cw = p->cw + 1;
   // printf("p->wds[p->cw] %s\n", p->wds[p->cw]);
   InstrcList(p);
   return true;
}

bool Instrc(Program *p){
   if(strsame(p->wds[p->cw], "PRINT")){
      p->cw = p->cw + 1;
      if(Print(p)){
         return true;
      };
      ERROR("PRINT error");
   }
   if(strsame(p->wds[p->cw], "SET")){
      p->cw = p->cw + 1;
      p->pos = (int) p->wds[p->cw][1] - 'A'; //SET $A
      p->workingpos = (int) p->wds[p->cw+2][1] - 'A';
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
   // ERROR("Wrong instruction");
   return false;
}

bool Print(Program *p){
   if(Varname(p)){
      #ifdef INTERP
      int pos = (int)(p->wds[p->cw][1]) - 'A'; //Print $C
      if(p->variable[pos].num == NULL){
         ERROR("Empty variable");
      }
      for(int j=0; j<p->variable[pos].height; j++){ //row
         for(int i=0; i<p->variable[pos].width; i++){ //col
            if(p->variable[pos].height > 1){
               printf("%d ", p->variable[pos].num[j][i]);
            }
            else{
               printf("%d\n", p->variable[pos].num[j][i]);
            }
         }
         if(p->variable[pos].height > 1){
            printf("\n");
         }
      }
      #endif

      return true;
   }
   else if(String(p)){
      #ifdef INTERP
      int len = strlen(p->wds[p->cw]);
      for(int i=0; i<len; i++){
         if(p->wds[p->cw][i] != '"'){
            printf("%c", p->wds[p->cw][i]);
         }
      }
      printf("\n");
      #endif
      return true;
   }
   ERROR("Print error");
}

bool Varname(Program *p){
   p->workingpos = (int) p->wds[p->cw][1] - 'A';
   int len = strlen(p->wds[p->cw]);
   if(len != 2){
      return false;
      // ERROR("Wrong varname2");
   }

   char* var = (char*)ncalloc(len+1, sizeof(char));
   strcpy(var, p->wds[p->cw]);

   if(!(var[0] == '$' && var[1] >= 'A' && var[1] <= 'Z')){
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
   char* string2 = (char*)ncalloc(len-2+1, sizeof(char));
   strcpy(string, p->wds[p->cw]);
   strncpy(string2, string+1, len-2);
   if(strsame(p->wds[p->cw-1], "READ")){
      FILE* fp = fopen(string2, "r");
      if(fp==NULL){
         fprintf(stderr, "Cannot open file");
      }
      fscanf(fp, "%d", &p->variable[p->pos].height);
      fscanf(fp, "%d", &p->variable[p->pos].width);
      p->variable[p->pos].num = (int**)n2dcalloc(p->variable[p->pos].height, p->variable[p->pos].width, sizeof(int));
      for(int j=0; j<p->variable[p->pos].height; j++){ //row
         for(int i=0; i<p->variable[p->pos].width; i++){ //col
            fscanf(fp, "%d", &p->variable[p->pos].num[j][i]);
         }
      }
      fclose(fp);
   }
   int count = 0;
   for(int i=0; i<len; i++){
      if(string[i] == '"'){
         count++;
      }
   }
   if(count != 2){
   // if(!((string[0] == '"') && (string[strlen(p->wds[p->cw])-1] == '"'))){
      free(string2);
      free(string);
      return false;
      // ERROR("string error");
   }
   else{
      free(string2);
      free(string);
   }
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
   if(Varname(p)){
      if(strsame(p->wds[p->cw+1], ";")){
         printf("pos %d\n", p->workingpos);
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
   }
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
      return true;
   }
   if(!Polish(p)){
      // printf("%s\n", "TEST");
      ERROR("Polish error");
   }
   p->cw = p->cw + 1;
   PolishList(p);
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
      if(strsame(p->wds[p->cw+1], "U-NOT") || strsame(p->wds[p->cw+1], "U-EIGHTCOUNT")){
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      // printf("ap->stack->size %d\n", p->stack->size);
         // p->stack->size = 0; //TBC
         printf("here %d\n", p->variable[1].num[3][3]);
         printf("pos %d\n", p->pos);
         // printf("here %d\n", p->variable[3].num[3][3]);
         printf("TEST");
         // for(int i=0; i<p->stack->capacity; i++){
         //    n2dfree(p->stack->a[i].num, p->stack->a[i].height);
         // }
         // p->stack->size = 0;
      }
      return true;
   }
   else if(Integer(p)){
      #ifdef INTERP
      p->cw = p->cw - 2;
      if(strsame(p->wds[p->cw+1], ":=")){
         if(Varname(p)){
         // if(p->wds[p->cw-1][0]=='$' && p->wds[p->cw-1][1]<'A' && p->wds[p->cw-][1]>'Z'){ //check if set one value or other op
            p->cw = p->cw + 2;
            // int pos = (int) p->wds[p->cw-2][1] - 'A'; //respective var ascii
            p->pos = (int) p->wds[p->cw-2][1] - 'A'; //respective var ascii
            int value = atoi(p->wds[p->cw]);
            AllocSpace(p, 1, 1, p->pos);
            AssignValues(p, p->pos, value);
            if(strsame(p->wds[p->cw+1], "U-NOT") || strsame(p->wds[p->cw+1], "U-EIGHTCOUNT")){
               // printf("%s\n", p->wds[p->cw+1]); //TODO: tbc
               // printf("num %d\n", p->variable[p->pos].num[0][0]);
               stack_push(p->stack, &p->variable[p->pos]);
            }
         }
      }
      else{
         // printf("p->pos %d\n", p->pos);
         p->cw = p->cw + 2 + 1; // ... B-ADD 2 *B-TIMES*
         return false;
      }
      #endif
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
      #ifdef INTERP
      var tempvar1;
      stack_pop(p->stack, &tempvar1);
      // stack_pop(p->stack, &tempvar2);
      for(int j=0; j<tempvar1.height; j++){ //row
         for(int i=0; i<tempvar1.width; i++){ //col
            tempvar1.num[j][i] =  ~tempvar1.num[j][i];
         }
      }
      stack_push(p->stack, &tempvar1);
      StackToVar_working(p);
      n2dfree(tempvar1.num, tempvar1.height);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "U-EIGHTCOUNT")){
      #ifdef INTERP
      printf("ap->stack->size %d\n", p->stack->size); //todo
      var tempvar1;
      stack_pop(p->stack, &tempvar1);
      var tempvar_bound;
      tempvar_bound.height = tempvar1.height + 2;
      tempvar_bound.width = tempvar1.width + 2;
      tempvar_bound.num = (int**)n2dcalloc(tempvar_bound.height, tempvar_bound.width, sizeof(int));
      for(int j=0; j<tempvar_bound.height; j++){ //row
         for(int i=0; i<tempvar_bound.width; i++){ //col
            if(j==0 || i==0 || j==tempvar_bound.height-1 || i==tempvar_bound.width -1){
               tempvar_bound.num[j][i] = 0;
            }
         }
      }
      for(int j=1; j<tempvar_bound.height-1; j++){
         for(int i=1; i<tempvar_bound.width-1; i++){
            tempvar_bound.num[j][i] = tempvar1.num[j-1][i-1];
         }
      }
      for(int j=1; j<tempvar_bound.height-1; j++){ //row
         for(int i=1; i<tempvar_bound.width-1; i++){ //col
            int count = 0;
            for(int y=-1; y<=1; y++){
               for(int x=-1; x<=1; x++){
                  if(tempvar_bound.num[j+y][i+x] > 0){
                     count++;
                  }
               }
            }
            tempvar1.num[j-1][i-1] = count;
         }
      }
      stack_push(p->stack, &tempvar1);
      p->cw = p->cw - 1;
      if(Integer(p)){
         p->cw = p->cw + 1;
         p->workingpos = p->pos;
      }
      else{
         p->cw = p->cw + 1;
         StackToVar_working(p);
      }
      n2dfree(tempvar_bound.num, tempvar_bound.height);
      n2dfree(tempvar1.num, tempvar1.height);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
         // printf("TEST");
         // for(int i=0; i<p->stack->capacity; i++){
         //    n2dfree(p->stack->a[i].num, p->stack->a[i].height);
         // }
         p->stack->size = 0;
      }
      #endif
      return true;
   }
   return false;
   // ERROR("UnaryOp error");
}

bool BinaryOp(Program *p){
   if(strsame(p->wds[p->cw], "B-AND")){
      #ifdef INTERP
      printf("p->stack->size %d\n", p->stack->size);
      p->cw = p->cw - 2;
      if(!Push(p)){
         p->cw = p->cw - 1;
         // printf("here %s\n", p->wds[p->cw]);
         if(Varname(p)){
            stack_push(p->stack, &p->variable[p->workingpos]);
            p->cw = p->cw + 1;
         }
         else if(Integer(p)){
            int value = atoi(p->wds[p->cw]);
            // printf("%d", value);
            var temp = MakeIntMatrix(value);
            stack_push(p->stack, &temp);
            n2dfree(temp.num, temp.height);
            p->cw = p->cw + 1;
         }
         else{
            p->cw = p->cw + 1;
         }
      }
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = p->cw - 1;
      if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] &= tempvar1.num[0][0];
            }
         }
         p->cw = p->cw + 1;
      }
      else if (Varname(p)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] &= tempvar1.num[j][i];
            }
         }
         p->cw = p->cw + 1;
      }
      stack_push(p->stack, &tempvar2);
      FreeNum(tempvar1, tempvar2);
      if(strsame(p->wds[p->cw+1], ";")){
         // printf("here %d\n", p->pos);
         // printf("%d\n", p->variable[p->pos].height);
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-OR")){
      #ifdef INTERP
      // printf("p->stack->size %d\n", p->stack->size);
      p->cw = p->cw - 2;
      if(!Push(p)){
         p->cw = p->cw - 1;
         // printf("here %s\n", p->wds[p->cw]);
         if(Varname(p)){
            stack_push(p->stack, &p->variable[p->workingpos]);
            p->cw = p->cw + 1;
         }
         else if(Integer(p)){
            int value = atoi(p->wds[p->cw]);
            // printf("%d", value);
            var temp = MakeIntMatrix(value);
            stack_push(p->stack, &temp);
            n2dfree(temp.num, temp.height);
            p->cw = p->cw + 1;
         }
         else{
            p->cw = p->cw + 1;
         }
      }
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = p->cw - 1;
      if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
         // printf("working pos %d\n", p->workingpos);
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] |= tempvar1.num[0][0];
            }
         }
         p->cw = p->cw + 1;
      }
      else if (Varname(p)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] |= tempvar1.num[j][i];
            }
         }
         p->cw = p->cw + 1;
      }
      stack_push(p->stack, &tempvar2);
      FreeNum(tempvar1, tempvar2);
      if(strsame(p->wds[p->cw+1], ";")){
         // printf("pos %d\n", p->pos);
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-GREATER")){
      #ifdef INTERP
      p->cw = p->cw - 2;
      if(!Push(p)){
         p->cw = p->cw - 1;
         // printf("here %s\n", p->wds[p->cw]);
         if(Varname(p)){
            stack_push(p->stack, &p->variable[p->workingpos]);
            p->cw = p->cw + 1;
         }
         else if(Integer(p)){
            int value = atoi(p->wds[p->cw]);
            // printf("%d", value);
            var temp = MakeIntMatrix(value);
            stack_push(p->stack, &temp);
            n2dfree(temp.num, temp.height);
            p->cw = p->cw + 1;
         }
         else{
            p->cw = p->cw + 1;
         }
      }
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = p->cw - 1;
      if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] = tempvar2.num[j][i] > tempvar1.num[0][0];
            }
         }
         p->cw = p->cw + 1;
      }
      else if (Varname(p)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] = tempvar2.num[j][i] > tempvar1.num[j][i];
            }
         }
         p->cw = p->cw + 1;
      }
      stack_push(p->stack, &tempvar2);
      FreeNum(tempvar1, tempvar2);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-LESS")){
      #ifdef INTERP
      p->cw = p->cw - 2;
      if(!Push(p)){
         p->cw = p->cw - 1;
         // printf("here %s\n", p->wds[p->cw]);
         if(Varname(p)){
            stack_push(p->stack, &p->variable[p->workingpos]);
            p->cw = p->cw + 1;
         }
         else if(Integer(p)){
            int value = atoi(p->wds[p->cw]);
            // printf("%d", value);
            var temp = MakeIntMatrix(value);
            stack_push(p->stack, &temp);
            n2dfree(temp.num, temp.height);
            p->cw = p->cw + 1;
         }
         else{
            p->cw = p->cw + 1;
         }
      }
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = p->cw - 1;
      if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] = tempvar2.num[j][i] < tempvar1.num[0][0];
            }
         }
         p->cw = p->cw + 1;
      }
      else if (Varname(p)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] = tempvar2.num[j][i] < tempvar1.num[j][i];
            }
         }
         p->cw = p->cw + 1;
      }
      stack_push(p->stack, &tempvar2);
      FreeNum(tempvar1, tempvar2);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-ADD")){
      #ifdef INTERP
      p->cw = p->cw - 2;
      if(!Push(p)){
         p->cw = p->cw - 1;
         printf("here %s\n", p->wds[p->cw]);
         if(Varname(p)){
            stack_push(p->stack, &p->variable[p->workingpos]);
            p->cw = p->cw + 1;
         }
         else if(Integer(p)){
            int value = atoi(p->wds[p->cw]);
            // printf("%d", value);
            var temp = MakeIntMatrix(value);
            stack_push(p->stack, &temp);
            n2dfree(temp.num, temp.height);
            p->cw = p->cw + 1;
         }
         else{
            p->cw = p->cw + 1;
         }
      }
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = p->cw - 1;
      if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] += tempvar1.num[0][0];
            }
         }
         p->cw = p->cw + 1;
      }
      else if (Varname(p)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] += tempvar1.num[j][i];
            }
         }
         p->cw = p->cw + 1;
      }
      stack_push(p->stack, &tempvar2);
      FreeNum(tempvar1, tempvar2);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-TIMES")){
      #ifdef INTERP
      p->cw = p->cw - 2;
      if(!Push(p)){
         p->cw = p->cw - 1;
         // printf("here %s\n", p->wds[p->cw]);
         if(Varname(p)){
            stack_push(p->stack, &p->variable[p->workingpos]);
            p->cw = p->cw + 1;
         }
         else if(Integer(p)){
            int value = atoi(p->wds[p->cw]);
            // printf("%d", value);
            var temp = MakeIntMatrix(value);
            stack_push(p->stack, &temp);
            n2dfree(temp.num, temp.height);
            p->cw = p->cw + 1;
         }
         else{
            p->cw = p->cw + 1;
         }
      }
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = p->cw - 1;
      if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] *= tempvar1.num[0][0];
            }
         }
         p->cw = p->cw + 1;
      }
      else if (Varname(p)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] *= tempvar1.num[j][i];
            }
         }
         p->cw = p->cw + 1;
      }
      stack_push(p->stack, &tempvar2);
      FreeNum(tempvar1, tempvar2);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-EQUALS")){
      #ifdef INTERP
      p->cw = p->cw - 2;
      if(!Push(p)){
         p->cw = p->cw - 1;
         // printf("here %s\n", p->wds[p->cw]);
         if(Varname(p)){
            stack_push(p->stack, &p->variable[p->workingpos]);
            p->cw = p->cw + 1;
         }
         else if(Integer(p)){
            int value = atoi(p->wds[p->cw]);
            // printf("%d", value);
            var temp = MakeIntMatrix(value);
            stack_push(p->stack, &temp);
            n2dfree(temp.num, temp.height);
            p->cw = p->cw + 1;
         }
         else{
            p->cw = p->cw + 1;
         }
      }
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = p->cw - 1;
      if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] = tempvar2.num[j][i] == tempvar1.num[0][0];
            }
         }
         p->cw = p->cw + 1;
      }
      else if (Varname(p)){
         for(int j=0; j<tempvar2.height; j++){ //row
            for(int i=0; i<tempvar2.width; i++){ //col
               tempvar2.num[j][i] = tempvar2.num[j][i] == tempvar1.num[j][i];
            }
         }
         p->cw = p->cw + 1;
      }
      stack_push(p->stack, &tempvar2);
      FreeNum(tempvar1, tempvar2);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
         printf("TEST");
         // // for(int i=0; i<p->stack->capacity; i++){
         // //    n2dfree(p->stack->a[i].num, p->stack->a[i].height);
         // // }
         // p->stack->size = 0;
      }
      #endif
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
      p->pos = (int) p->wds[p->cw+2][1] - 'A'; //READ "lglider.arr" $A
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
      // printf("%d\n", (int)p->wds[p->cw][0] - '0'); //turn s to d
      return true;
   }
   ERROR("Row error");
}

bool Col(Program *p){
   if(Integer(p)){
      #ifdef INTERP
      int pos = (int)(p->wds[p->cw+1][1]) - 'A'; //respective var ascii
      int h = atoi(p->wds[p->cw-1]);
      int w = atoi(p->wds[p->cw]);
      // int h = (int)(p->wds[p->cw-1][0] - '0');
      // int w = (int)(p->wds[p->cw][0] - '0');
      AllocSpace(p, h, w, pos);
      AssignValues(p, pos, 1);
      #endif
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
   // p->looppos = (int) p->wds[p->cw-1][1] - 'A';
   // p->variable[p->looppos].maxloop = atoi(p->wds[p->cw]); //10
   // p->variable[p->looppos] = MakeIntMatrix(0); //TODO
   // printf("num %d\n", p->variable[p->pos].num[0][0]);

   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   if(strsame(p->wds[p->cw], "{")){
      int ptr = 1;
      p->looppos = (int) p->wds[p->cw-2][1] - 'A';
      // int x = atoi(p->wds[p->cw-1]);
      // p->variable[p->looppos] = MakeIntMatrix(1); //init $I=1
      // p->variable[p->looppos].loopstart = p->cw + 1; //PRINT
      p->variable[p->looppos].loopcount = 1;
      // p->variable[p->looppos].maxloop = atoi(p->wds[p->cw-1]); //10
      int temp = p->cw + 1;
      int maxloop = atoi(p->wds[p->cw-1]);
      // while(ptr <= x){
      while(ptr <= maxloop){
      // while(ptr <= p->variable[p->looppos].maxloop){
         // printf("p->wds[p->cw] %s\n", p->wds[temp]);
         p->cw = temp;
         // p->cw = p->variable[p->looppos].loopstart;
         if(InstrcList(p)){
            ptr++;
         }
         // printf("POS %d\n",p->looppos);
         // printf("maxxloop %d\n", p->variable[p->looppos].maxloop);
         // ptr++;
      }
   }
   p->cw = p->cw + 1;
   // p->variable[p->looppos].num[0][0] += 1; //TODO
   InstrcList(p);
   return true;
}

#ifdef INTERP
void AssignValues(Program *p, int pos, int value){ //stack push
   // p->variable[pos].height = h;
   // p->variable[pos].width = w;
   // p->variable[pos].num = (int**)n2dcalloc(h, w, sizeof(int));
   for(int j=0; j<p->variable[pos].height; j++){ //row
      for(int i=0; i<p->variable[pos].width; i++){ //col
         // printf("before: %d\n", p->variable[pos].num[j][i]);
         p->variable[pos].num[j][i] += value;
         // printf("word: %d\n", p->variable[pos].num[j][i]);
      }
   }
}

void AllocSpace(Program *p, int h, int w, int pos){
   // p->variable[pos] = (var*) ncalloc(1, sizeof(var));
   p->variable[pos].height = h;
   p->variable[pos].width = w;
   p->variable[pos].num = (int**)n2dcalloc(h, w, sizeof(int));
}

stack* stack_init(void)
{
   stack *s = (stack*) ncalloc(1, sizeof(stack));
   /* Some implementations would allow you to pass
      a hint about the initial size of the stack */
   s->a = (var*) ncalloc(FIXEDSIZE, sizeof(var));
   s->size = 0;
   s->capacity= FIXEDSIZE;
   return s;
}

void stack_push(stack* s, var* d)
{
   if(s==NULL){
      return;
   }
   if(s->size >= s->capacity){
      s->a = (var*) nremalloc(s->a,
            sizeof(var)*s->capacity*SCALEFACTOR);
      s->capacity = s->capacity*SCALEFACTOR;
   }
   if(s->a[s->size].height == 0){
      s->a[s->size].num = (int**)n2dcalloc(d->height, d->width, sizeof(int));
   }
   s->a[s->size].height = d->height;
   s->a[s->size].width = d->width;
   // s->a[s->size].loopstart = d->loopstart; //TBC
   // s->a[s->size].loopcount = d->loopcount;
   // s->a[s->size].maxloop = d->maxloop;
   for(int j=0; j<d->height; j++){ //row
      for(int i=0; i<d->width; i++){ //col
         s->a[s->size].num[j][i] = d->num[j][i];
         // printf("word: %d\n", s->a[s->size].num[j][i]); 
      }
   }
   s->size = s->size + 1;
}

bool stack_pop(stack* s, var* temp)
{
   if((s == NULL) || (s->size < 1)){
      return false;
   }
   s->size = s->size - 1;
   temp->height = s->a[s->size].height;
   temp->width = s->a[s->size].width;
   // temp->loopstart = s->a[s->size].loopstart;
   // temp->loopcount = s->a[s->size].loopcount;
   // temp->maxloop = s->a[s->size].maxloop;

   
   temp->num = (int**)n2dcalloc(temp->height, temp->width, sizeof(int));
   for(int j=0; j<temp->height; j++){ //row
      for(int i=0; i<temp->width; i++){ //col
         temp->num[j][i] = s->a[s->size].num[j][i];
         // printf("word: %d\n", temp->num[j][i]);
      }
   }
   return true;
}


bool stack_free(stack* s)
{
   if(s==NULL){
      return true;
   }
   for(int i=0; i<s->capacity; i++){
      n2dfree(s->a[i].num, s->a[i].height);
   }
   free(s->a);
   free(s);
   return true;
}

var MakeIntMatrix(int num){
   var temp;
   temp.num = (int**)n2dcalloc(1, 1, sizeof(int));
   temp.height = 1;
   temp.width = 1;
   temp.num[0][0] = num;
   return temp;
}

void FreeNum(var temp1, var temp2){
   n2dfree(temp1.num, temp1.height);
   n2dfree(temp2.num, temp2.height);
}

void StackToVar(Program *p){
   if(p->variable[p->pos].num==NULL){
   // if(p->variable[p->pos].height==0){
   // if(p->workingpos != p->pos && p->workingpos >= 0){
      p->variable[p->pos].num = (int**)n2dcalloc(p->stack->a[p->stack->size-1].height, p->stack->a[p->stack->size-1].width, sizeof(int));
   }
   p->variable[p->pos].height = p->stack->a[p->stack->size-1].height;
   p->variable[p->pos].width = p->stack->a[p->stack->size-1].width;
   for(int j=0; j<p->stack->a[p->stack->size-1].height; j++){ //row
      for(int i=0; i<p->stack->a[p->stack->size-1].width; i++){ //col
         p->variable[p->pos].num[j][i] = p->stack->a[p->stack->size-1].num[j][i];
      }
   }
}

void StackToVar_working(Program *p){
   for(int j=0; j<p->variable[p->workingpos].height; j++){ //row
      for(int i=0; i<p->variable[p->workingpos].width; i++){ //col
         p->variable[p->workingpos].num[j][i] = p->stack->a[p->stack->size-1].num[j][i];
      }
   }
}

bool Push(Program *p){
   // p->cw = p->cw - 2;
   if(Varname(p)){
      p->cw = p->cw + 1;
      if(Varname(p)){
         p->workingpos = (int) p->wds[p->cw-1][1] - 'A';
         stack_push(p->stack, &p->variable[p->workingpos]);
         p->workingpos = (int) p->wds[p->cw][1] - 'A';
         stack_push(p->stack, &p->variable[p->workingpos]);
         p->cw = p->cw + 1;
         return true;
      }
      else if(Integer(p)){
         p->workingpos = (int) p->wds[p->cw-1][1] - 'A'; //TBC
         stack_push(p->stack, &p->variable[p->workingpos]);
         int value = atoi(p->wds[p->cw]);
         var temp = MakeIntMatrix(value);
         stack_push(p->stack, &temp);
         n2dfree(temp.num, temp.height);
         p->cw = p->cw + 1;
         return true;
      }
      return false;
   }
   else if(Integer(p)){
      int value = atoi(p->wds[p->cw]);
      var temp = MakeIntMatrix(value);
      stack_push(p->stack, &temp);
      n2dfree(temp.num, temp.height);
      p->cw = p->cw + 1;
      if(Integer(p)){
         int value1 = atoi(p->wds[p->cw]);
         var temp1 = MakeIntMatrix(value1);
         stack_push(p->stack, &temp1);
         n2dfree(temp.num, temp.height);
         p->cw = p->cw + 1;
         return true;
      }
      else if(Varname(p)){
         stack_push(p->stack, &p->variable[p->workingpos]);
         p->cw = p->cw + 1;
         return true;
      }
      return false;
   }
   else{
      p->cw = p->cw + 2;
      return false;
   }
   return false;
}

bool stack_clear(stack* s) //TODO: clear stack after ";", }} stop, nested loop
{
   // if((s == NULL) || (s->size < 1)){
   //    return false;
   // }
   // s->size = 0;
   // s->a[s->size].height = 0;
   // s->a[s->size].width = 0;
   // // temp->num = (int**)n2dcalloc(temp->height, temp->width, sizeof(int));
   // for(int j=0; j<temp->height; j++){ //row
   //    for(int i=0; i<temp->width; i++){ //col
   //       temp->num[j][i] = s->a[s->size].num[j][i];
   //       // printf("word: %d\n", temp->num[j][i]);
   //    }
   // }
   // return true;
}

#endif
