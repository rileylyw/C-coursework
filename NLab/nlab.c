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
   ERROR("Wrong instruction");
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
         // printf("%s\n", p->wds[p->cw+1]); //TODO: tbc
         // printf("here %d\n", p->workingpos);
         // printf("here %d\n", p->variable[p->workingpos].height);
         // printf("here %d\n", p->variable[p->workingpos].num[0][0]);
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      // printf("varname %s\n", p->wds[p->cw]);
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
            printf("%d\n", p->variable[p->pos].height);
            if(strsame(p->wds[p->cw+1], "U-NOT") || strsame(p->wds[p->cw+1], "U-EIGHTCOUNT")){
               printf("%s\n", p->wds[p->cw+1]); //TODO: tbc
               // printf("%d\n", p->variable[p->pos].height);
               stack_push(p->stack, &p->variable[p->pos]);
            }
         }
      }
      else{
         printf("p->pos %d\n", p->pos);
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
      var tempvar1;
      stack_pop(p->stack, &tempvar1);
      //TODO***: set boundary
      for(int j=0; j<tempvar1.height; j++){ //row
         for(int i=0; i<tempvar1.width; i++){ //col
            int count = 0;
            for(int y=-1; y<=1; y++){
                  for(int x=-1; x<=1; x++){
                     if(tempvar1.num[y][x] > 0){
                        count++;
                     }
                  }
               }
            }
            tempvar1.num[j][i] = count;
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
   return false;
   // ERROR("UnaryOp error");
}

bool BinaryOp(Program *p){
   if(strsame(p->wds[p->cw], "B-AND")){
      #ifdef INTERP
      if(p->stack->size == 0){
         p->workingpos = (int) p->wds[p->cw-2][1] - 'A'; //respective var ascii
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      int value = atoi(p->wds[p->cw-1]);
      var temp = MakeIntMatrix(value);
      stack_push(p->stack, &temp);
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar2.num[j][i] &= tempvar1.num[0][0];
         }
      }
      stack_push(p->stack, &tempvar2);
      StackToVar_working(p);
      FreeNum(tempvar1, tempvar2, temp);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-OR")){
      #ifdef INTERP
      if(p->stack->size == 0){
         p->workingpos = (int) p->wds[p->cw-2][1] - 'A'; //respective var ascii
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      int value = atoi(p->wds[p->cw-1]);
      var temp = MakeIntMatrix(value);
      stack_push(p->stack, &temp);
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar2.num[j][i] |= tempvar1.num[0][0];
         }
      }
      stack_push(p->stack, &tempvar2);
      StackToVar_working(p);
      FreeNum(tempvar1, tempvar2, temp);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-GREATER")){
      #ifdef INTERP
      if(p->stack->size == 0){
         p->workingpos = (int) p->wds[p->cw-2][1] - 'A'; //respective var ascii
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      int value = atoi(p->wds[p->cw-1]);
      var temp = MakeIntMatrix(value);
      stack_push(p->stack, &temp);
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar2.num[j][i] = tempvar2.num[j][i] > tempvar1.num[0][0];
         }
      }
      stack_push(p->stack, &tempvar2);
      StackToVar_working(p);
      FreeNum(tempvar1, tempvar2, temp);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-LESS")){
      #ifdef INTERP
      if(p->stack->size == 0){
         p->workingpos = (int) p->wds[p->cw-2][1] - 'A'; //respective var ascii
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      int value = atoi(p->wds[p->cw-1]);
      var temp = MakeIntMatrix(value);
      stack_push(p->stack, &temp);
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar2.num[j][i] = tempvar2.num[j][i] < tempvar1.num[0][0];
         }
      }
      stack_push(p->stack, &tempvar2);
      StackToVar_working(p);
      FreeNum(tempvar1, tempvar2, temp);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-ADD")){
      #ifdef INTERP
      if(p->stack->size == 0){
         p->workingpos = (int) p->wds[p->cw-2][1] - 'A'; //respective var ascii
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      printf("p->pos %d\n", p->pos);
      int value = atoi(p->wds[p->cw-1]);
      var temp = MakeIntMatrix(value);
      stack_push(p->stack, &temp);
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar2.num[j][i] += tempvar1.num[0][0];
         }
      }
      stack_push(p->stack, &tempvar2);
      StackToVar_working(p);
      FreeNum(tempvar1, tempvar2, temp);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-TIMES")){
      #ifdef INTERP
      if(p->stack->size == 0){
         p->workingpos = (int) p->wds[p->cw-2][1] - 'A'; //respective var ascii
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      int value = atoi(p->wds[p->cw-1]);
      // stack_push(p->stack, &p->variable[pos]);
      var temp = MakeIntMatrix(value);
      stack_push(p->stack, &temp);
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar2.num[j][i] *= tempvar1.num[0][0];
         }
      }
      stack_push(p->stack, &tempvar2);
      StackToVar_working(p);
      FreeNum(tempvar1, tempvar2, temp);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
      }
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-EQUALS")){
      #ifdef INTERP
      if(p->stack->size == 0){
         p->workingpos = (int) p->wds[p->cw-2][1] - 'A'; //respective var ascii
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      int value = atoi(p->wds[p->cw-1]);
      var temp = MakeIntMatrix(value);
      stack_push(p->stack, &temp);
      var tempvar1, tempvar2;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar2.num[j][i] = tempvar2.num[j][i] == tempvar1.num[0][0];
         }
      }
      stack_push(p->stack, &tempvar2);
      StackToVar_working(p);
      FreeNum(tempvar1, tempvar2, temp);
      if(strsame(p->wds[p->cw+1], ";")){
         StackToVar(p);
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
      if(!Varname(p)){ //TODO: check varname valid first b4 allocating space?
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
   p->cw = p->cw + 1;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   p->cw = p->cw + 1;
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
      s->a[s->size].num = (int**)n2dcalloc(d->height, d->width, sizeof(int)); //TODO FREE
   }
   s->a[s->size].height = d->height;
   s->a[s->size].width = d->width;
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
   for(int i=0; i<s->capacity; i++){  //TODO: change back to s->size
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

void FreeNum(var temp1, var temp2, var temp3){
   n2dfree(temp1.num, temp1.height);
   n2dfree(temp2.num, temp2.height);
   n2dfree(temp3.num, temp3.height);
}

void StackToVar(Program *p){
   if(p->workingpos != p->pos){
      p->variable[p->pos].num = (int**)n2dcalloc(p->stack->a[0].height, p->stack->a[0].width, sizeof(int)); //TODO FREE
   }
   p->variable[p->pos].height = p->stack->a[0].height;
   p->variable[p->pos].width = p->stack->a[0].width;
   for(int j=0; j<p->stack->a[0].height; j++){ //row
      for(int i=0; i<p->stack->a[0].width; i++){ //col
         p->variable[p->pos].num[j][i] = p->stack->a[0].num[j][i];
      }
   }
}

void StackToVar_working(Program *p){
   for(int j=0; j<p->variable[p->workingpos].height; j++){ //row
      for(int i=0; i<p->variable[p->workingpos].width; i++){ //col
         p->variable[p->workingpos].num[j][i] = p->stack->a[0].num[j][i];
      }
   }
}

#endif
