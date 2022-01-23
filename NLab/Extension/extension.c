#include "extension.h"

void readFile(char file[], Program* p){
   FILE* fp=fopen(file, "r");
   if(fp==NULL){
      fprintf(stderr, "Cannot open file");
   }
   int i = 0;
   char buffer[MAXNUMTOKENS];
   char comment[MAXNUMTOKENS];
   char wds[MAXNUMTOKENS];
   while(fscanf(fp, "%[^\n]\n", buffer) != EOF){
      if(buffer[0]=='#'){
         strcpy(comment, buffer);
      }
      else{
         strcpy(wds, buffer);
         char *pch = strtok(wds, " ");
         while(pch != NULL){
            strcpy(p->wds[i++], pch);
            pch = strtok(NULL, " ");
         }
      }
   }
   fclose(fp);
}

bool verbose(char file[BIGNUM], int argc, char* argv[]){
   if(argc==2){
      strcpy(file, argv[1]);
   }
   else if(argc==1){
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
   p->cw = _1wordforward;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   p->cw = _1wordforward;
   InstrcList(p);
   #ifdef INTERP
   stack_free(p->stack);
   #endif
   return true;
}

bool InstrcList(Program *p){
   if(strsame(p->wds[p->cw], "}")){
      return true;
   }
   if(!Instrc(p)){
      return false;
   }
   p->cw = _1wordforward;
   InstrcList(p);
   return true;
}

bool Instrc(Program *p){
   if(strsame(p->wds[p->cw], "PRINT")){
      p->cw = _1wordforward;
      if(Print(p)){
         return true;
      };
      ERROR("PRINT error");
   }
   if(strsame(p->wds[p->cw], "SET")){
      p->cw = _1wordforward;
      #ifdef INTERP
      p->pos = CW;
      p->workingpos = CW_plus2;
      #endif
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
      p->cw = _1wordforward;
      if(Loop(p)){
         return true;
      };
      ERROR("LOOP error");
   }
   return false;
}

bool Print(Program *p){
   if(Varname(p)){
      #ifdef INTERP
      int pos = CW;
      if(p->variable[pos].num == NULL){
         ERROR("Empty variable");
      }
      PrintVar(p, pos);
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
   #ifdef INTERP
   p->workingpos = CW;
   #endif
   int len = strlen(p->wds[p->cw]);
   if(len != 2){
      return false;
   }
   char* var = (char*)ncalloc(len+1, sizeof(char));
   strcpy(var, p->wds[p->cw]);
   if(!(var[0] == '$' && var[1] >= 'A' && var[1] <= 'Z')){
      free(var);
      return false;
   }
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
   #ifdef INTERP
   char* string2 = (char*)ncalloc(len-2+1, sizeof(char));
   strncpy(string2, string+1, len-2);
   if(strsame(p->wds[_1wordback], "READ")){
      ReadArray(string2, p);
   }
   #endif
   int count = 0;
   for(int i=0; i<len; i++){
      if(string[i] == '"'){
         count++;
      }
   }
   if(count != 2){
   #ifdef INTERP
      free(string2);
   #endif
      free(string);
      return false;
   }
   else{
   #ifdef INTERP
      free(string2);
   #endif
      free(string);
   }
   return true;
}

bool Set(Program *p){
   if(!Varname(p)){
      ERROR("Wrong varname");
   }
   p->cw = _1wordforward;
   if(!strsame(p->wds[p->cw], ":=")){
      ERROR("Incorrect operator");
   }
   p->cw = _1wordforward;
   if(PolishList(p)){
      return true;
   }
   return false;
}

bool PolishList(Program *p){
   if(p->wds[p->cw] && !p->wds[p->cw][0]){
      ERROR("NULL");
   }
   if(strsame(p->wds[p->cw], ";")){
      return true;
   }
   if(!Polish(p)){
      ERROR("Polish error");
   }
   p->cw = _1wordforward;
   PolishList(p);
   if(!(strsame(p->wds[p->cw], ";"))){
      ERROR("Missing ;");
   }
   return true;
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
   ERROR("Missing ;");
}

bool PushDown(Program *p){
   if(Varname(p)){
      #ifdef INTERP
      if(strsame(p->wds[_1wordforward], "U-NOT") || strsame(p->wds[_1wordforward], "U-EIGHTCOUNT")){
         stack_push(p->stack, &p->variable[p->workingpos]);
      }
      if(strsame(p->wds[_1wordforward], ";")){
         stack_push(p->stack, &p->variable[p->workingpos]);
         StackToVar(p);
         p->stack->size = 0;
      }
      #endif
      return true;
   }
   else if(Integer(p)){
      #ifdef INTERP
      p->cw = _2wordsback;
      if(strsame(p->wds[_1wordforward], ":=")){
         if(Varname(p)){
            p->cw = _2wordsforward;
            #ifdef INTERP
            p->pos = CW_minus2;
            int value = atoi(p->wds[p->cw]);
            AllocSpace(p, 1, 1, p->pos);
            AssignValues(p, p->pos, value);
            if(strsame(p->wds[_1wordforward], "U-NOT") || strsame(p->wds[_1wordforward], "U-EIGHTCOUNT")){
               stack_push(p->stack, &p->variable[p->pos]);
            #endif
            }
         }
      }
      else{
         p->cw = _2wordsforward + 1;
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
   strcpy(var, p->wds[p->cw]); 
   for(unsigned int i=0; i<strlen(var); i++){
      if(!(var[i]<='9' && var[i]>='0')){
         return false;
      }
   }
   return true;
}

bool UnaryOp(Program *p){
   if(strsame(p->wds[p->cw], "U-NOT")){
      #ifdef INTERP
      U_NOT(p);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "U-EIGHTCOUNT")){
      #ifdef INTERP
      U_EIGHTCOUNT(p);
      #endif
      return true;
   }
   return false;
}

bool BinaryOp(Program *p){
   if(strsame(p->wds[p->cw], "B-AND")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_AND(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-MINUS")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_MINUS(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-OR")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_OR(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-GREATER")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_GREATER(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-LESS")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_LESS(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-ADD")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_ADD(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-TIMES")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_TIMES(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-DIVIDE-RU")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_DIVIDE_ROUNDUP(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-DIVIDE-RD")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_DIVIDE_ROUNDDOWN(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   else if(strsame(p->wds[p->cw], "B-EQUALS")){
      #ifdef INTERP
      PushPrevItem(p);
      var tempvar1, tempvar2, tempvar3;
      stack_pop(p->stack, &tempvar1);
      stack_pop(p->stack, &tempvar2);
      p->cw = _1wordback;
      tempvar3 = TempVarForResult(tempvar2);
      B_EQUALS(p, tempvar1, tempvar2, tempvar3);
      PushResult(p, tempvar1, tempvar2, tempvar3);
      #endif
      return true;
   }
   return false;
}

bool Create(Program *p){
   if(strsame(p->wds[p->cw], "ONES")){
      p->cw = _1wordforward;
      if(!Row(p)){
         ERROR("Row error");
      }
      p->cw = _1wordforward;
      if(!Col(p)){
         ERROR("Col error");
      }
      p->cw = _1wordforward;
      if(!Varname(p)){
         ERROR("Varname error");
      }
      return true;
   }
   else if(strsame(p->wds[p->cw], "READ")){
      #ifdef INTERP
      p->pos = CW_plus2; 
      #endif
      p->cw = _1wordforward;
      if(!FileName(p)){
         ERROR("FileName error");
      }
      p->cw = _1wordforward;
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
      #ifdef INTERP
      int pos = CW_plus1; //respective var ascii
      int h = atoi(p->wds[_1wordback]);
      int w = atoi(p->wds[p->cw]);
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
   if(!Varname(p)){
      ERROR("Varname error");
   }
   p->cw = _1wordforward;
   if(!Integer(p)){
      ERROR("Integer error");
   }
   p->cw = _1wordforward;
   if(!strsame(p->wds[p->cw], "{")){
      ERROR("Missing {");
   }
   #ifdef INTERP
   if(strsame(p->wds[p->cw], "{")){
      int ptr = 1; 
      int looppos = CW_minus2;
      if(p->variable[looppos].num==NULL){
         p->variable[looppos].num = (int**)n2dcalloc(1, 1, sizeof(int));
      }
      p->variable[looppos].height = 1;
      p->variable[looppos].width = 1;
      p->variable[looppos].num[0][0] = 1;
      int loopstart = _1wordforward; //go back to 
      int maxloop = atoi(p->wds[_1wordback]);
      while(ptr <= maxloop){
         p->cw = loopstart;
         if(InstrcList(p)){
            ptr = p->variable[looppos].num[0][0];
            ptr++;
            p->variable[looppos].num[0][0] = ptr;
         }
      }
   }
   #endif
   p->cw = _1wordforward;
   InstrcList(p);
   return true;
}

#ifdef INTERP
void AssignValues(Program *p, int pos, int value){
   for(int j=0; j<p->variable[pos].height; j++){ //row
      for(int i=0; i<p->variable[pos].width; i++){ //col
         p->variable[pos].num[j][i] += value;
      }
   }
}

void AllocSpace(Program *p, int h, int w, int pos){
   p->variable[pos].height = h;
   p->variable[pos].width = w;
   p->variable[pos].num = (int**)n2dcalloc(h, w, sizeof(int));
}

stack* stack_init(void)
{
   stack *s = (stack*) ncalloc(1, sizeof(stack));
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
   if(s->a[s->size].num==NULL){
      s->a[s->size].num = (int**)n2dcalloc(d->height, d->width, sizeof(int));
   }
   else if(s->a[s->size].height!=d->height || s->a[s->size].width!=d->width){
      n2dfree(s->a[s->size].num, s->a[s->size].height);
      s->a[s->size].num = (int**)n2dcalloc(d->height, d->width, sizeof(int));
   }
   s->a[s->size].height = d->height;
   s->a[s->size].width = d->width;
   for(int j=0; j<d->height; j++){ //row
      for(int i=0; i<d->width; i++){ //col
         s->a[s->size].num[j][i] = d->num[j][i];
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
      }
   }
   return true;
}


bool stack_free(stack* s){
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

void FreeNum(var temp1, var temp2, var temp3){
   n2dfree(temp1.num, temp1.height);
   n2dfree(temp2.num, temp2.height);
   n2dfree(temp3.num, temp3.height);
}

void StackToVar(Program *p){
   if(p->variable[p->pos].num==NULL){
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

bool Pushable(Program *p){
   if(Varname(p)){
      p->cw = _1wordforward;
      if(Pushable_VarFirst(p)){
         return true;
      }
      else{
         return false;
      }
   }
   else if(Integer(p)){
      if(Pushable_IntFirst(p)){
         return true;
      }
      else{
         return false;
      }
   }
   else{
      p->cw = _2wordsforward;
      return false;
   }
   return false;
}

bool Pushable_VarFirst(Program *p){
   if(Varname(p)){
      p->workingpos = CW_minus1;
      stack_push(p->stack, &p->variable[p->workingpos]);
      p->workingpos = CW;
      stack_push(p->stack, &p->variable[p->workingpos]);
      p->cw = _1wordforward;
      return true;
   }
   else if(Integer(p)){
      p->workingpos = CW_minus1;
      stack_push(p->stack, &p->variable[p->workingpos]);
      int value = atoi(p->wds[p->cw]);
      var temp = MakeIntMatrix(value);
      stack_push(p->stack, &temp);
      n2dfree(temp.num, temp.height);
      p->cw = _1wordforward;
      return true;
   }
   return false;
}

bool Pushable_IntFirst(Program *p){
   int value = atoi(p->wds[p->cw]);
   var temp = MakeIntMatrix(value);
   stack_push(p->stack, &temp);
   n2dfree(temp.num, temp.height);
   p->cw = _1wordforward;
   if(Integer(p)){
      int value1 = atoi(p->wds[p->cw]);
      var temp1 = MakeIntMatrix(value1);
      stack_push(p->stack, &temp1);
      n2dfree(temp.num, temp.height);
      p->cw = _1wordforward;
      return true;
   }
   else if(Varname(p)){
      stack_push(p->stack, &p->variable[p->workingpos]);
      p->cw = _1wordforward;
      return true;
   }
   return false;
}

void PrintVar(Program *p, int pos){
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
}

void ReadArray(char string2[], Program *p){
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

var TempVarWithBounds(var tempvar1){
   var tempvar_bound = PadZeros(tempvar1);
   CountTrueValues(tempvar_bound, tempvar1);
   return tempvar_bound;
}

var PadZeros(var tempvar1){
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
   return tempvar_bound;
}

void CountTrueValues(var tempvar_bound, var tempvar1){
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
                  if(!(y==0 && x==0)){ //count itself out
                     count++;
                  }
               }
            }
         }
         tempvar1.num[j-1][i-1] = count;
      }
   }
}

void PushPrevItem(Program *p){
   p->cw = _2wordsback;
   if(!Pushable(p)){
      p->cw = _1wordback;
      if(Varname(p)){
         stack_push(p->stack, &p->variable[p->workingpos]);
         p->cw = _1wordforward;
      }
      else if(Integer(p)){
         int value = atoi(p->wds[p->cw]);
         var temp = MakeIntMatrix(value);
         stack_push(p->stack, &temp);
         n2dfree(temp.num, temp.height);
         p->cw = _1wordforward;
      }
      else{
         p->cw = _1wordforward;
      }
   }
}

var TempVarForResult(var tempvar2){
   var tempvar3;
   tempvar3.height = tempvar2.height;
   tempvar3.width = tempvar2.width;
   tempvar3.num = (int**)n2dcalloc(tempvar3.height, tempvar3.width, sizeof(int));
   return tempvar3;
}

void PushResult(Program *p, var tempvar1, var tempvar2, var tempvar3){
   stack_push(p->stack, &tempvar3);
   FreeNum(tempvar1, tempvar2, tempvar3);
   if(strsame(p->wds[_1wordforward], ";")){
      StackToVar(p);
      p->stack->size = 0; //reset
   }
}

void U_NOT(Program *p){
   var tempvar1;
   stack_pop(p->stack, &tempvar1);
   for(int j=0; j<tempvar1.height; j++){ //row
      for(int i=0; i<tempvar1.width; i++){ //col
         tempvar1.num[j][i] =  ~tempvar1.num[j][i];
      }
   }
   stack_push(p->stack, &tempvar1);
   n2dfree(tempvar1.num, tempvar1.height);
   if(strsame(p->wds[_1wordforward], ";")){
      StackToVar(p);
      p->stack->size = 0;
   }
}

void U_EIGHTCOUNT(Program *p){
   var tempvar1;
   stack_pop(p->stack, &tempvar1);
   var tempvar_bound = TempVarWithBounds(tempvar1);
   stack_push(p->stack, &tempvar1);
   p->cw = _1wordback;
   if(Integer(p)){
      p->cw = _1wordforward;
      p->workingpos = p->pos;
   }
   else{
      p->cw = _1wordforward;
   }
   n2dfree(tempvar_bound.num, tempvar_bound.height);
   n2dfree(tempvar1.num, tempvar1.height);
   if(strsame(p->wds[_1wordforward], ";")){
      StackToVar(p);
      p->stack->size = 0;
   }
}

void B_AND(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] & tempvar1.num[0][0];
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] & tempvar1.num[j][i];
         }
      }
      p->cw = _1wordforward;
   }
}

void B_OR(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] | tempvar1.num[0][0];
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] | tempvar1.num[j][i];
         }
      }
      p->cw = _1wordforward;
   }
}

void B_GREATER(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] > tempvar1.num[0][0];
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] > tempvar1.num[j][i];
         }
      }
      p->cw = _1wordforward;
   }
}

void B_LESS(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] < tempvar1.num[0][0];
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] < tempvar1.num[j][i];
         }
      }
      p->cw = _1wordforward;
   }
}

void B_ADD(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] + tempvar1.num[0][0];
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] + tempvar1.num[j][i];
         }
      }
      p->cw = _1wordforward;
   }
}

void B_MINUS(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] - tempvar1.num[0][0];
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] - tempvar1.num[j][i];
         }
      }
      p->cw = _1wordforward;
   }
}

void B_TIMES(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] * tempvar1.num[0][0];
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] * tempvar1.num[j][i];
         }
      }
      p->cw = _1wordforward;
   }
}

void B_DIVIDE_ROUNDUP(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = ceil((double)tempvar2.num[j][i] / tempvar1.num[0][0]);
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = ceil((double)tempvar2.num[j][i] / tempvar1.num[j][i]);
         }
      }
      p->cw = _1wordforward;
   }
}

void B_DIVIDE_ROUNDDOWN(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = floor((double)tempvar2.num[j][i] / tempvar1.num[0][0]);
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = floor((double)tempvar2.num[j][i] / tempvar1.num[j][i]);
         }
      }
      p->cw = _1wordforward;
   }
}

void B_EQUALS(Program *p, var tempvar1, var tempvar2, var tempvar3){
   if(Integer(p) || (Varname(p) && p->variable[p->workingpos].height==1 && p->variable[p->workingpos].width==1)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] == tempvar1.num[0][0];
         }
      }
      p->cw = _1wordforward;
   }
   else if (Varname(p)){
      for(int j=0; j<tempvar2.height; j++){ //row
         for(int i=0; i<tempvar2.width; i++){ //col
            tempvar3.num[j][i] = tempvar2.num[j][i] == tempvar1.num[j][i];
         }
      }
      p->cw = _1wordforward;
   }
}

#endif

