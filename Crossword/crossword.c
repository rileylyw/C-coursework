#include "crossword.h"

int** newMatrix(int size);
void freeMartix(int** matrix, int size);
void markAcross(const crossword* cw, int **matrix, int y, int x);
void markDown(const crossword* cw, int **matrix, int y, int x);
int append(char* string, int length, int startCount);
void merge(char* s1, char* s2, char* string);

// Might be useful to be able to print them
// to hep with debugging
void print_crossword(const crossword* c){
   for(int y=0; y<c->sz; y++){
      for(int x=0; x<c->sz; x++){
         printf("%c", c->arr[y][x]);
      }
      printf("\n");
   }
}

bool str2crossword(int sz, char* ip, crossword* cw){
   if(ip==NULL || cw==NULL){
      return false;
   }
   if((int)strlen(ip)!=sz*sz || sz<0){
      return false;
   }
   for(int j=0; j<sz; j++){
      for(int i=0; i<sz; i++){
         cw->arr[j][i]=ip[j*sz+i];
      }
   }
   cw->sz=sz;
   return true;
}

int getchecked(crossword c){
   int** matrix=newMatrix(c.sz);
   for(int j=0; j<c.sz; j++){
      for(int i=0; i<c.sz; i++){
         if(c.arr[j][i]==' ' || c.arr[j][i]=='.'){
            if(matrix[j][i]==0){
               if(j<c.sz-1 && (c.arr[j+1][i]==' ' || c.arr[j+1][i]=='.')){ //if next row blank
                  markDown(&c, matrix, j, i); //+2 to matrix
               }
               if(i<c.sz-1 && (c.arr[j][i+1]==' ' || c.arr[j][i+1]=='.')){ //if next col blank
                  markAcross(&c, matrix, j, i); //+1 to matrix
               }
            }
            else if(matrix[j][i]==1 && j<c.sz-1 && (c.arr[j+1][i]==' ' || c.arr[j+1][i]=='.')){ //if already has col and next row blank
               markDown(&c, matrix, j, i);
            }
            else if(matrix[j][i]==2 && i<c.sz-1 && (c.arr[j][i+1]==' ' || c.arr[j][i+1]=='.')){ //if already has row and next col blank
               markAcross(&c, matrix, j, i);
            }
         }
      }
   }
   int sharedCount=0, full=0;
   for(int j=0; j<c.sz; j++){
      for(int i=0; i<c.sz; i++){
         sharedCount+=(matrix[j][i]==3?1:0); //shared squares will be 3
         full+=(matrix[j][i]==0?1:0); //full squares will be 0
      }
   }
   freeMartix(matrix, c.sz);
   return (int)((double)sharedCount*100/(c.sz*c.sz-full)+0.5);
}

void getcluestring(const crossword* c, char* ans){
   int** matrix=newMatrix(c->sz);
   char across[BIGSTR]={'A'}, down[BIGSTR]={'D'};
   int markNumber=0, acrossNumber=1, downNumber=1;
   for(int j=0; j<c->sz; j++){
      for(int i=0; i<c->sz; i++){
         if(c->arr[j][i]==' ' || c->arr[j][i]=='.'){
            markNumber++;
            if(matrix[j][i]==0){
               if(j<c->sz-1 && (c->arr[j+1][i]==' ' || c->arr[j+1][i]=='.')){ //if next row blank
                  markDown(c, matrix, j, i); //+2 to matrix
                  downNumber=append(down, downNumber, markNumber);
               }
               if(i<c->sz-1 && (c->arr[j][i+1]==' ' || c->arr[j][i+1]=='.')){ //if next col blank
                  markAcross(c, matrix, j, i); //+1 to matrix
                  acrossNumber=append(across, acrossNumber, markNumber);
               }
            }
            else if(matrix[j][i]==1 && j<c->sz-1 && (c->arr[j+1][i]==' ' || c->arr[j+1][i]=='.')){ //if already has col and next row blank
               markDown(c, matrix, j, i);
               downNumber=append(down, downNumber, markNumber);
            }
            else if(matrix[j][i]==2 && i<c->sz-1 && (c->arr[j][i+1]==' ' || c->arr[j][i+1]=='.')){ //if already has row and next col blank
               markAcross(c, matrix, j, i);
               acrossNumber=append(across, acrossNumber, markNumber);
            }
            else{
               markNumber--;
            }
         }
      }
   }
   merge(across, down, ans);
   freeMartix(matrix, c->sz);
}

// for storing numbers
// if across: +1, if down: +2
// therefore, shared sqaures will be 3
int** newMatrix(int size){
   int** matrix=(int**)malloc(sizeof(int*)*size);
   for(int i=0; i<size; i++){
      matrix[i]=(int*)calloc(size,sizeof(int)); //sqaures initialized to 0
   }
   return matrix;
}

void freeMartix(int** matrix, int size){
   for(int i=0; i<size; i++){
      free(matrix[i]);
   }
   free(matrix);
}

// +1 to matrix square if it's not full
void markAcross(const crossword* cw, int **matrix, int y, int x){
   assert(y<=cw->sz && y>=0 && x<=cw->sz && x>=0); //avoid possible segfault
   while(x<cw->sz && !(cw->arr[y][x]=='*' || cw->arr[y][x]=='X')){
      matrix[y][x]+=1;
      x++;
   }
}

// +2 to matrix square if it's not full
void markDown(const crossword* cw, int **matrix, int y, int x){
   assert(y<=cw->sz && y>=0 && x<=cw->sz && x>=0);
   while(y<cw->sz && !(cw->arr[y][x]=='*' || cw->arr[y][x]=='X')){
      matrix[y][x]+=2;
      y++;
   }
}

// append start count to across and down arrays respectively
// return length of strings
int append(char* string, int length, int startCount){
   int newLength=length;
   char ASCII[10];
   string[newLength++]='-';
   string[newLength]='\0';
   int i=0;
   while(startCount!=0){ //turn int to string
      int temp=startCount%10;
      ASCII[i++]=(char)(temp+'0');
      startCount=startCount/10;
   }
   for(int j=i-1; j>=0; j--){
      string[newLength++]=ASCII[j];
   }
   string[newLength]='\0';
   return newLength;
}

// merge across and down arrays
void merge(char* s1, char* s2, char* string){
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

void test(void)
{

   // test matrix malloc and free
   int** matrix=newMatrix(5);
   assert(matrix!=NULL);
   freeMartix(matrix, 5);
   assert(sizeof(matrix)==sizeof(int**));

   int** matrix2=newMatrix(50000);
   assert(matrix2!=NULL);
   freeMartix(matrix2, 50000);
   assert(sizeof(matrix2)==sizeof(int**));

   // test string functions
   char s11[GRID]="A-1-4", s12[GRID]="D-1-2-3", s13[GRID];
   assert(append(s11, 5, 5)==7); //append '5' to s11, original length of s11 is 5
   assert(strcmp("A-1-4-5", s11)==0);
   assert(strcmp("A-!@#-5sdfasd", s11)!=0);
   merge(s11, s12, s13);
   assert(strcmp("A-1-4-5|D-1-2-3", s13)==0);

   char s21[GRID]="A-1-4", s22[GRID]="D-1-4-5-6-9", s23[GRID];
   assert(append(s22, 11, 10)==14); //append '10' to s22
   assert(append(s22, 14, 15)==17);
   assert(append(s22, 17, 1000)==22);
   assert(append(s22, 22, 1000000)==30);
   assert(strcmp("D-1-4-5-6-9-10-15-1000-1000000", s22)==0);
   assert(strcmp("D-1-4-5-6-9-10-15-1000-1000001", s22)!=0);
   merge(s21, s22, s23);
   assert(strcmp("A-1-4|D-1-4-5-6-9-10-15-1000-1000000", s23)==0);

   char s31[GRID]="A-1-4-5-7", s32[GRID]="D-1", s33[GRID];
   assert(append(s31, 9, 8)==11); //append '8' to s31
   assert(append(s31, 11, 9)==13);
   assert(append(s31, 13, 10)==16);
   assert(append(s32, 3, 2)==5);
   assert(append(s32, 5, 3)==7);
   assert(append(s32, 7, 6)==9);
   assert(strcmp("A-1-4-5-7-8-9-10", s31)==0);
   assert(strcmp("D-1-2-3-6", s32)==0);
   merge(s31, s32, s33);
   assert(strcmp("A-1-4-5-7-8-9-10|D-1-2-3-6", s33)==0);

   char s41[GRID]="A", s42[GRID]="D", s43[GRID];
   assert(append(s41, 1, 100)==5); //append '100' to s41
   assert(append(s41, 5, 1000)==10);
   assert(append(s41, 10, 10000)==16);
   assert(append(s42, 1, 9999)==6);
   assert(append(s42, 6, 88888123)==15);
   assert(append(s42, 15, 234102213)==25);
   assert(strcmp("A-100-1000-10000", s41)==0);
   assert(strcmp("D-9999-88888123-234102213", s42)==0);
   merge(s41, s42, s43);
   assert(strcmp("A-100-1000-10000|D-9999-88888123-234102213", s43)==0);

   // test mark functions
   crossword c1;
   int** matrix3=newMatrix(5);
   str2crossword(5, "....X.XX.X.X......X.XX...", &c1);
   markDown(&c1, matrix3, 0, 0);
   assert(matrix3[0][0]==2);
   assert(matrix3[1][0]!=1);
   assert(matrix3[3][0]==2);
   assert(matrix3[4][0]==0);
   markAcross(&c1, matrix3, 0, 0);
   assert(matrix3[0][0]==3);
   assert(matrix3[0][1]==1);
   assert(matrix3[0][3]==1);
   assert(matrix3[0][3]!=3);
   markDown(&c1, matrix3, 0, 3);
   assert(matrix3[0][3]==3);
   assert(matrix3[1][3]==2);
   assert(matrix3[2][3]==2);
   assert(matrix3[3][3]==0); //full
   markAcross(&c1, matrix3, 2, 2);
   assert(matrix3[2][1]==0);
   assert(matrix3[2][2]==1);
   assert(matrix3[2][3]==3);
   assert(matrix3[2][4]==1);
   markDown(&c1, matrix3, 2, 2);
   assert(matrix3[2][2]==3);
   assert(matrix3[3][2]==2);
   assert(matrix3[4][2]==2);
   markDown(&c1, matrix3, 1, 1); //full
   assert(matrix3[1][1]==0);
   assert(matrix3[2][1]==0);
   assert(matrix3[3][1]==0);
   markDown(&c1, matrix3, 1, 4); //full
   assert(matrix3[0][4]==0);
   assert(matrix3[1][4]==0);
   assert(matrix3[2][4]==1); //already marked +1 before
   assert(matrix3[3][4]==0);
   freeMartix(matrix3, 5);

   crossword c2;
   int** matrix4=newMatrix(8);
   str2crossword(8, ".....X.XX.X.X..........XX.X.X......X.X.XX..........X.X.XX.X.....", &c2);
   markDown(&c2, matrix4, 0, 1);
   assert(matrix4[0][1]==2);
   assert(matrix4[1][1]==2);
   assert(matrix4[7][1]==2);
   markAcross(&c2, matrix4, 0, 1);
   assert(matrix4[0][0]==0);
   assert(matrix4[0][1]==3);
   assert(matrix4[0][3]==1);
   assert(matrix4[0][5]==0); //full
   assert(matrix4[0][6]==0);
   markDown(&c2, matrix4, 4, 4);
   assert(matrix4[3][4]==0); //full
   assert(matrix4[4][4]==2);
   assert(matrix4[7][4]==2);
   markAcross(&c2, matrix4, 5, 1);
   assert(matrix4[5][1]==3);
   assert(matrix4[5][2]==1);
   assert(matrix4[5][4]==3);
   freeMartix(matrix4, 8);

   // test other corner cases
   crossword c3;
   assert(!str2crossword(10000, "....X.XX.X.X......X.XX...", &c3));
   assert(!str2crossword('a', "*   *               *   *", &c3));
   assert(!str2crossword(12, "ABCDABCDABCDABCD", &c3));
   assert(!str2crossword(-5.0, "*   *               *   *", &c3));
}
