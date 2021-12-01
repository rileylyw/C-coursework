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
   assert(x<=cw->sz&&x>=0&&y<=cw->sz&&y>=0);
   while(y<cw->sz&&!(cw->arr[x][y]=='*'||cw->arr[x][y]=='X')){
      matrix[x][y]+=1;
      y++;
   }
}

void markDown(const crossword* cw,int **matrix,int x,int y){
   while(x<cw->sz&&!(cw->arr[x][y]=='*'||cw->arr[x][y]=='X')){
      matrix[x][y]+=2;
      x++;
   }
}

int append(char* string,int length,int startCount){
   int newLength=length;
   char ASCII[5];
   string[newLength++]='-';
   string[newLength]='\0';
   int i=0;
   while(startCount!=0){
      int temp=startCount%10;
      ASCII[i++]=(char)(temp+'0');
      startCount=startCount/10;
   }
   for(int j=i-1;j>=0;j--){
      string[newLength++]=ASCII[j]; //add ASCII to downNumber th in down[]
   }
   string[newLength]='\0';
   return newLength;
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
   int startCount=0,acrossNumber=1,downNumber=1;

   for(int i=0;i<c->sz;i++){
      for(int j=0;j<c->sz;j++){
         if(c->arr[i][j]==' '||c->arr[i][j]=='.'){
            startCount++;
            if(matrix[i][j]==0){
               if(i<c->sz-1&&(c->arr[i+1][j]==' '||c->arr[i+1][j]=='.')){ //if next row is blank
                  markDown(c,matrix,i,j); //+2 to matrix if box is not blacked
                  downNumber=append(down,downNumber,startCount); //add count for across
               }
               if(j<c->sz-1&&(c->arr[i][j+1]==' '||c->arr[i][j+1]=='.')){ //if next col is blank
                  markAcross(c,matrix,i,j); //+1 to matrix if box is not blacked
                  acrossNumber=append(across,acrossNumber,startCount);//add count for across
               }
            }
            else if(matrix[i][j]==1&&i<c->sz-1&&(c->arr[i+1][j]==' '||c->arr[i+1][j]=='.')){ //if hv across already and bottom 2 blank
               markDown(c,matrix,i,j);
               downNumber=append(down,downNumber,startCount); //add count for down
            }
            else if(matrix[i][j]==2&&j<c->sz-1&&(c->arr[i][j+1]==' '||c->arr[i][j+1]=='.')){ //if hv down already and right 2 blank
               markAcross(c,matrix,i,j);
               acrossNumber=append(across,acrossNumber,startCount); //add count for down
            }
            else{
               startCount--;
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

   int** matrix3=newMatrix(50000);
   assert(matrix3!=NULL);
   freeMartix(matrix3,50000);
   assert(matrix3==NULL);
   assert(sizeof(matrix3)==sizeof(int**));

   //test string functions
   char s11[100]="A-1-4",s12[100]="D-1-2-3",s13[100];
   assert(append(s11,5,5)==7);
   assert(strcmp("A-1-4-5",s11)==0);
   merge(s11,s12,s13);
   assert(strcmp("A-1-4-5|D-1-2-3",s13)==0);

   // char s1[100]="A-1-4-5",s2[100]="D-1-2-3",s3[100];
   // assert(append(s1,5,5)==7);
   // assert(strcmp("A-1-4-5",s1)==0);
   // merge(s1,s2,s3);
   // assert(strcmp("A-1-4-5|D-1-2-3",s13)==0);

   char s21[100]="A",s22[100]="D",s23[100];
   assert(append(s21,1,0)==2);
   assert(strcmp("A-",s21)==0);
   merge(s21,s22,s23);
   assert(strcmp("A-|D",s23)==0);

   // char s31[100]="A-1-4-",s32[100]="D-1-4",s33[100];
   // assert(append(s31,15,15)==17);
   // printf("%s", s31);
   // assert(strcmp("A-",s31)==0);
   // merge(s31,s32,s33);
   // assert(strcmp("A-|D",s33)==0);

/*
   // test markDown markAcross
   char str1[BIGSTR];
   crossword* c1;
   assert(str2crossword(5, "....X.XX.X.X......X.XX...", &c1));
   getcluestring(&c1, str1);
   int** matrix3=newMatrix(5);
   assert(matrix3!=NULL);
   markDown(c1,matrix3,0,3);
   assert(matrix3[0][3]==2);
   freeMartix(matrix3,5);
*/
}
