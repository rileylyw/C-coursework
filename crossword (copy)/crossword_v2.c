#include "crossword.h"

// Might be useful to be able to print them
// to hep with debugging
void print_crossword(const crossword* c)
{
   for(int y=0; y<c->sz; y++){
      for(int x=0; x<c->sz; x++){
         printf("%c", c->arr[y][x]);
      }
      printf("\n");
   }
}

int** newMatrix(int size){
   int** matrix=(int**)malloc(sizeof(int*)*size); //2d array ptr
   for(int i=0;i<size;i++){
      matrix[i]=(int*)calloc(size,sizeof(int));
   }
   return matrix;
}

void freeMartix(int** matrix,int size){
   for(int i=0;i<size;i++){
      free(matrix[i]);
   }
   free(matrix);
}

void markAcross(const crossword* cw,int **matrix,int x,int y){
   int i=y;
   while(i<cw->sz){
      if(cw->arr[x][i]=='*'||cw->arr[x][i]=='X'){
         break;
      }
      matrix[x][i]+=1;
      i++;
   }
}

void markDown(const crossword* cw,int **matrix,int x,int y){
   int i=x;
   while(i<cw->sz){
      if(cw->arr[i][y]=='*'||cw->arr[i][y]=='X'){
         break;
      }
      matrix[i][y]+=2;
      i++;
   }
}

int append(char* string,int index,int number){
   int newIndex=index;
   char buffer[10];
   string[newIndex++]='-';
   string[newIndex]='\0';
   int i=0;
   while(number!=0){
      int temp=number%10;
      buffer[i++]=(char)(temp+'0');
      number=number/10;
   }
   for(int j=i-1;j>=0;j--){
      string[newIndex++]=buffer[j];
   }
   string[newIndex]='\0';
   return newIndex;
}

void merge(char* s1,char* s2,char* string){
   int i=0;
   while(s1[i]!='\0'){
      string[i]=s1[i];
      i++;
   }
   string[i++]='|';
   int j=0;
   while(s2[j]!='\0'){
      string[i+j]=s2[j];
      j++;
   }
   string[i+j]='\0';
}

void print(int **matrix,int size){
   for(int y=0; y<size; y++){
      for(int x=0; x<size; x++){
         printf("%d ", matrix[y][x]);
      }
      printf("\n");
   }
}

bool str2crossword(int sz, char* ip, crossword* cw)
{
   if(ip==NULL||cw==NULL){
      return false;
   }
   if((int)strlen(ip)!=sz*sz||sz<0){
      return false;
   }
   for(int i=0;i<sz;i++){
      for(int j=0;j<sz;j++){
         cw->arr[i][j]=ip[i*sz+j];
      }
   }
   cw->sz=sz;
   return true;
}

int getchecked(crossword c)
{
   int** matrix=newMatrix(c.sz);
   for(int i=0;i<c.sz;i++){
      for(int j=0;j<c.sz;j++){
         if(c.arr[i][j]==' '||c.arr[i][j]=='.'){
            if(matrix[i][j]==0){
               if(i<c.sz-1&&(c.arr[i+1][j]==' '||c.arr[i+1][j]=='.')){
                  markDown(&c,matrix,i,j);
               }
               if(j<c.sz-1&&(c.arr[i][j+1]==' '||c.arr[i][j+1]=='.')){
                  markAcross(&c,matrix,i,j);
               }
            }
            else if(matrix[i][j]==1&&i<c.sz-1&&(c.arr[i+1][j]==' '||c.arr[i+1][j]=='.')){
               markDown(&c,matrix,i,j);
            }
            else if(matrix[i][j]==2&&j<c.sz-1&&(c.arr[i][j+1]==' '||c.arr[i][j+1]=='.')){
               markAcross(&c,matrix,i,j);
            }
         }
      }
   }
   int count=0,block=0;
   for(int i=0;i<c.sz;i++){
      for(int j=0;j<c.sz;j++){
         count+=(matrix[i][j]==3?1:0);
         block+=(matrix[i][j]==0?1:0);
      }
   }
   freeMartix(matrix,c.sz);
   return (int)((double)count*100/(c.sz*c.sz-block)+0.5);
}

void getcluestring(const crossword* c, char* ans)
{
   int** matrix=newMatrix(c->sz);
   char across[BIGSTR],down[BIGSTR];
   across[0]='A';across[1]='\0';
   down[0]='D';down[1]='\0';
   int markNumber=0,acrossNumber=1,downNumber=1;
   for(int i=0;i<c->sz;i++){ //row
      for(int j=0;j<c->sz;j++){ //col
         if(c->arr[i][j]==' '||c->arr[i][j]=='.'){
            markNumber++;
            if(matrix[i][j]==0){
               if(i<c->sz-1&&(c->arr[i+1][j]==' '||c->arr[i+1][j]=='.')){ //check blank down
                  markDown(c,matrix,i,j);
                  downNumber=append(down,downNumber,markNumber);
               }
               if(j<c->sz-1&&(c->arr[i][j+1]==' '||c->arr[i][j+1]=='.')){
                  markAcross(c,matrix,i,j);
                  acrossNumber=append(across,acrossNumber,markNumber);
               }
            }
            else if(matrix[i][j]==1&&i<c->sz-1&&(c->arr[i+1][j]==' '||c->arr[i+1][j]=='.')){
               markDown(c,matrix,i,j);
               downNumber=append(down,downNumber,markNumber);
            }
            else if(matrix[i][j]==2&&j<c->sz-1&&(c->arr[i][j+1]==' '||c->arr[i][j+1]=='.')){
               markAcross(c,matrix,i,j);
               acrossNumber=append(across,acrossNumber,markNumber);
            }
            else{
               markNumber--;
            }
         }
      }
   }
   merge(across,down,ans);
   freeMartix(matrix,c->sz);
}

void test(void)
{
}
