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
   int** matrix=(int**)malloc(sizeof(int*)*size);
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
   while(i<cw->sz&&!(cw->arr[x][i]=='*'||cw->arr[x][i]=='X')){
      matrix[x][i]+=1;
      i++;
   }
}

void markDown(const crossword* cw,int **matrix,int x,int y){
   int i=x;
   while(i<cw->sz&&!(cw->arr[i][y]=='*'||cw->arr[i][y]=='X')){
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
         cw->arr[i][j]=ip[i*sz+j]; //main crossword
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

   for(int i=0;i<c->sz;i++){
      for(int j=0;j<c->sz;j++){
         if(c->arr[i][j]==' '||c->arr[i][j]=='.'){
            markNumber++; //count blank
            if(matrix[i][j]==0){
               if(i<c->sz-1&&(c->arr[i+1][j]==' '||c->arr[i+1][j]=='.')){ //if next row is blank
                  markDown(c,matrix,i,j); //+2 to matrix if box is not blacked
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
   //test matrix malloc and free
   int** matrix1=newMatrix(5);
   assert(matrix1!=NULL);
   freeMartix(matrix1,5);
   assert(sizeof(matrix1)==sizeof(int**));

   int** matrix2=newMatrix(50000);
   assert(matrix2!=NULL);
   freeMartix(matrix2,50000);
   assert(sizeof(matrix2)==sizeof(int**));


/*
   int size=5000;
   int** matrix3=newMatrix(size);
   assert(matrix3!=NULL);
   // freeMartix(matrix3,5000);
   assert(sizeof(matrix3)==(sizeof(int**)*size));
*/

   //test string functions
   char s1[100]="A-1-4",s2[100]="D-1-2-3",s3[100];
   assert(append(s1,5,5)==7);
   assert(strcmp("A-1-4-5",s1)==0);
   merge(s1,s2,s3);
   assert(strcmp("A-1-4-5|D-1-2-3",s3)==0);
}
