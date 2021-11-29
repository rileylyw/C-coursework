// coll_functions referenced from Niell's notes on collections

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DIM 6
#define FIXEDSIZE 2000000
#define NUM 10

typedef struct rollerboard{
   char board[MAX_DIM][MAX_DIM];
   int parentIndex;
} rollerboard;

typedef struct listOfBoard{
   rollerboard listOfBoards[FIXEDSIZE];
   int size;
} listOfBoard;

void test(void);
void* ncalloc(int n, size_t size);
listOfBoard* coll_init(void);
void on_error(const char* s);
int coll_size(listOfBoard* c);
// Reason for passing the dimensions to functions directly:
// no need to call the big list of struct in the loops
// might run faster
void readFile(char file[], int *ySize, int *xSize, char board[MAX_DIM][MAX_DIM]);
void moveRight(char newBoard[MAX_DIM][MAX_DIM], int ySize, int xSize, int row_num);
void moveLeft(char newBoard[MAX_DIM][MAX_DIM], int ySize, int xSize, int row_num);
void moveUp(char newBoard[MAX_DIM][MAX_DIM], int ySize, int xSize, int col_num);
void moveDown(char newBoard[MAX_DIM][MAX_DIM], int ySize, int xSize, int col_num);
void printBoard(int ySize,int xSize, char board[MAX_DIM][MAX_DIM]);
void initBoard(int ySize, int xSize, char parentBoard[MAX_DIM][MAX_DIM], char newBoard[MAX_DIM][MAX_DIM]);
bool isSolution(int ySize, int xSize, char board[MAX_DIM][MAX_DIM]);
void coll_add(listOfBoard* c, rollerboard newBoard);
bool coll_isin(listOfBoard* c, int ySize, int xSize, char newBoard[MAX_DIM][MAX_DIM]);
void printSolution(listOfBoard* c, int ySize, int xSize, int parentIdx, int argc);
void initFirst(int ySize, int xSize, listOfBoard* c, char init[MAX_DIM][MAX_DIM]);
void search(listOfBoard* c, int ySize, int xSize, int argc);
bool checkOnesInBoard(int ySize, int xSize, char board[MAX_DIM][MAX_DIM]);
void checkValidBoard(int ySize, int xSize, char board[MAX_DIM][MAX_DIM]);
void verbose(char file[NUM], int argc, char* argv[]);

int main(int argc, char* argv[]){
   test();
   int ySize, xSize;
   listOfBoard* listOfBoards=coll_init();
   char file[100];
   verbose(file, argc, argv);
   char initialBoard[MAX_DIM][MAX_DIM];
   readFile(file, &ySize, &xSize, initialBoard);
   checkValidBoard(ySize, xSize, initialBoard);
   initFirst(ySize, xSize, listOfBoards, initialBoard);
   search(listOfBoards, ySize, xSize, argc);
   free(listOfBoards);
   return 0;
}

void verbose(char file[NUM], int argc, char* argv[]){
   if(argc==3){
      strcpy(file, argv[2]);
      if(strcmp(argv[1], "-v")!=0){
         fprintf(stderr, "Invalid Inputs\n");
         exit(EXIT_FAILURE);
      }
   }
   else if(argc==2){
      strcpy(file, argv[1]);
   }
}

void checkValidBoard(int ySize, int xSize, char board[MAX_DIM][MAX_DIM]){
   if(isSolution(ySize, xSize, board)){
      fprintf(stderr, "No Solution?\n");
      exit(EXIT_FAILURE);
   }
   if(!checkOnesInBoard(ySize, xSize, board)){
      fprintf(stderr, "Invalid board, can't be solved\n");
      exit(EXIT_FAILURE); 
   }
}

void readFile(char file[], int *ySize, int *xSize, char board[MAX_DIM][MAX_DIM]){
   FILE* fp=fopen(file, "r");
   if(fp==NULL){
      fprintf(stderr, "Cannot open file %s\n", file);
      exit(EXIT_FAILURE);
   }
   if(fscanf(fp, "%d%d\n", ySize, xSize)<0){
      fprintf(stderr, "Rows and Cols dimensions incorrect %s\n", file);
      exit(EXIT_FAILURE);
   }
   fclose(fp);
   FILE* fp1=fopen(file, "r");
   signed char buffer;
   signed char bufferArr[MAX_DIM*MAX_DIM];

   int count=0;
   while((buffer=fgetc(fp1))!=EOF){
      if((buffer=='0')||(buffer=='1')){
         bufferArr[count]=buffer;
         count++;
      }
   }
   if(count!=(*ySize)*(*xSize)){
      fprintf(stderr, "Rollerboard contains invalid elements\n");
      exit(EXIT_FAILURE);
   }
   fclose(fp1);
   for(int j=0; j<*ySize; j++){
      for(int i=0; i<*xSize; i++){
         board[j][i]=bufferArr[j* (*xSize)+i];
      }
   }
}

bool checkOnesInBoard(int ySize, int xSize, char board[MAX_DIM][MAX_DIM]){
   int totalCount=0;

   for(int j=0; j<ySize; j++){
      for(int i=0; i<xSize; i++){
         if(board[j][i]=='1'){
            totalCount++;
         }
      }
   }
   if(totalCount==xSize){
      return true;
   }
   return false;
}

void moveRight(char newBoard[MAX_DIM][MAX_DIM], int ySize, int xSize, int row_num){
   char temp[NUM];
   temp[0]=newBoard[row_num][xSize-1];
   for(int j=0; j<ySize; j++){
      if(row_num==j){
         for(int i=0; i<xSize; i++){
            temp[i+1]=newBoard[j][i];
         }
      }
   }
   for(int j=0; j<ySize; j++){
      if(row_num==j){
         for(int i=0; i<xSize; i++){
            newBoard[j][i]=temp[i];
         }
      }
   }  
}

void moveLeft(char newBoard[MAX_DIM][MAX_DIM], int ySize, int xSize, int row_num){
   char temp[NUM];
   char temp2[NUM];
   for(int j=0; j<ySize; j++){
      for(int i=0; i<xSize; i++){
         if(j==row_num){
            temp[i]=newBoard[j][i];
         }
      }
   }
   for(int j=0; j<xSize; j++){
         temp2[j]=temp[(j+1)%xSize];
   }
   for(int j=0; j<ySize; j++){
      if(row_num==j){
         for(int i=0; i<xSize; i++){
            newBoard[j][i]=temp2[i];
         }
      }
   }
} 

void moveDown(char newBoard[MAX_DIM][MAX_DIM], int ySize, int xSize, int col_num){
   char temp[NUM];
   temp[0]=newBoard[ySize-1][col_num];
   for(int j=0; j<ySize; j++){
      for(int i=0; i<xSize; i++){
         if(i==col_num){
            temp[j+1]=newBoard[j][i];
         }
      }
   }
   for(int j=0; j<ySize; j++){
      for(int i=0; i<xSize; i++){
         if(col_num==i){
            newBoard[j][i]=temp[j];
         }
      }
   }
}

void moveUp(char newBoard[MAX_DIM][MAX_DIM], int ySize, int xSize, int col_num){
   char temp[NUM];
   char temp2[NUM];
   for(int j=0; j<ySize; j++){
      for(int i=0; i<xSize; i++){
         if(i==col_num){
            temp[j]=newBoard[j][i];
         }
      }
   }
   for(int i=0; i<ySize; i++){
      temp2[i]=temp[(i+1)%ySize];
   }
   for(int j=0; j<ySize; j++){
      for(int i=0; i<xSize; i++){
         if(col_num==i){
            newBoard[j][i]=temp2[j];
         }
      }
   }
}

void* ncalloc(int n, size_t size){
   void* v = calloc(n, size);
   if(v==NULL){
      on_error("Cannot calloc() space");
   }
   return v;
}

listOfBoard* coll_init(void){
   listOfBoard* c = (listOfBoard*) ncalloc(1, sizeof(listOfBoard));
   c->size = 0;
   return c;
}

int coll_size(listOfBoard* c){
   if(c==NULL){
      return 0;
   }
   return c->size;
}

bool coll_isin(listOfBoard* c, int ySize, int xSize, char newBoard[MAX_DIM][MAX_DIM]){
   for(int x=0; x<=c->size; x++){
      int count=0;
      for(int j=0; j<ySize; j++){
         for(int i=0; i<xSize; i++){
            if(c->listOfBoards[x].board[j][i]==newBoard[j][i]){
               count++;
            }
         }
      }
      if(count==ySize*xSize){
         return true;
      }
   }
   return false;
}

void printBoard(int ySize, int xSize, char board[MAX_DIM][MAX_DIM]){
   for(int j=0; j<ySize; j++){
      for(int i=0; i<xSize; i++){
         printf("%c", board[j][i]);
      }
      printf("\n");
   }
   printf("\n");
}

void initBoard(int ySize, int xSize, char parentBoard[MAX_DIM][MAX_DIM], char newBoard[MAX_DIM][MAX_DIM]){
   for(int j=0; j<ySize; j++){
      for(int i=0; i<xSize; i++){
         newBoard[j][i]=parentBoard[j][i];
      }
   }
}

bool isSolution(int ySize, int xSize, char board[MAX_DIM][MAX_DIM]){
   int count=0;
   for(int i=0; i<xSize; i++){
      if(board[0][i]=='1'){
         count++;
      }
   }
   if(count==xSize && checkOnesInBoard(ySize, xSize, board)){
      return true;
   }
   return false;
}

void coll_add(listOfBoard* c, rollerboard newBoard){
   if(c){
      if(c->size >= FIXEDSIZE){
         on_error("Collection overflow");
      }
      c->listOfBoards[coll_size(c)+1]=newBoard;
      c->size=c->size+1;
   }
}

void on_error(const char* s){
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}

void printSolution(listOfBoard* c, int ySize, int xSize, int parentIdx, int argc){
   for(int i=0; i<=coll_size(c); i++){ //loop through list of boards
      if(isSolution(ySize, xSize, c->listOfBoards[i].board)){
         int parentArray[FIXEDSIZE]={0};
         int count=0; //no. of steps-1

         while(parentIdx!=-1){
            parentArray[count]=parentIdx;
            count++;
            parentIdx=c->listOfBoards[parentIdx].parentIndex;
         }
         if(argc==3){
            for(int i=count-1; i>-1; i--){
               printf("%d: \n", count-1-i);
               printBoard(ySize, xSize, c->listOfBoards[parentArray[i]].board);
            }
            printf("%d: \n", count);
            printBoard(ySize, xSize, c->listOfBoards[i].board);
         }
         else if(argc==2){
            printf("%d moves\n", count);
         }
      }
   }
}

void initFirst(int ySize, int xSize, listOfBoard* c, char init[MAX_DIM][MAX_DIM]){
   for(int j=0; j<ySize; j++){
      for(int i=0; i<xSize; i++){
         c->listOfBoards[0].board[j][i]=init[j][i];
      }
   }
   c->listOfBoards[0].parentIndex = -1;
}

void search(listOfBoard* c, int ySize, int xSize, int argc){
   int parentIdx=0;
   bool solved=false; //not solved is false
   for(int a=0; a<=coll_size(c) && !solved; a++){ //loop through list of boards
      parentIdx=a;
      for(int x=0; x<xSize && !solved; x++){ //loop through each col (can go up or down)
         rollerboard newBoard;
         initBoard(ySize, xSize, c->listOfBoards[parentIdx].board, newBoard.board);
         newBoard.parentIndex=parentIdx;
         moveUp(newBoard.board, ySize, xSize, x);
         if(!coll_isin(c, ySize, xSize, newBoard.board)){
            coll_add(c, newBoard);
         }
         solved=isSolution(ySize, xSize, c->listOfBoards[coll_size(c)].board);
         if(!solved){
            rollerboard newBoard2;
            initBoard(ySize, xSize, c->listOfBoards[parentIdx].board, newBoard2.board);
            newBoard2.parentIndex=parentIdx;
            moveDown(newBoard2.board, ySize, xSize, x);
            if(!coll_isin(c, ySize, xSize, newBoard2.board)){
               coll_add(c, newBoard2);
            }
            solved=isSolution(ySize, xSize, c->listOfBoards[coll_size(c)].board);
         }
      }

      for(int y=0; y<ySize && !solved; y++){ //loop through each row (can go left or right)
         rollerboard newBoard;
         initBoard(ySize, xSize, c->listOfBoards[parentIdx].board, newBoard.board);
         newBoard.parentIndex=parentIdx;
         moveRight(newBoard.board, ySize, xSize, y);
         if(!coll_isin(c, ySize, xSize, newBoard.board)){
            coll_add(c, newBoard);
         }
         solved=isSolution(ySize, xSize, c->listOfBoards[coll_size(c)].board);
         if(!solved){
            rollerboard newBoard2;
            initBoard(ySize, xSize, c->listOfBoards[parentIdx].board, newBoard2.board);
            newBoard2.parentIndex=parentIdx;
            moveLeft(newBoard2.board, ySize, xSize, y);
            if(!coll_isin(c, ySize, xSize, newBoard2.board)){
               coll_add(c, newBoard2);
            }
            solved=isSolution(ySize, xSize, c->listOfBoards[coll_size(c)].board);
         }
      }
      printSolution(c, ySize, xSize, parentIdx, argc);
   }
}


void test(void){
   char TestBoard[MAX_DIM][MAX_DIM] = {
      {'1','0','0'},
      {'0','1','0'},
      {'0','0','1'}
	};
   char testBoard[MAX_DIM][MAX_DIM];
   initBoard(3, 3, TestBoard, testBoard);
   assert(!isSolution(3, 3, testBoard));
   assert(checkOnesInBoard(3, 3, testBoard));
   checkValidBoard(3, 3, testBoard);

   /* Movement functions tests*/
   moveRight(testBoard, 3, 3, 0);
   assert(testBoard[0][0]=='0');
   assert(testBoard[0][1]=='1');
   assert(testBoard[0][2]=='0');
   assert(!(testBoard[0][0]=='1'));
   assert(!(testBoard[0][1]=='0'));
   assert(!(testBoard[0][2]=='1'));
   moveLeft(testBoard, 3, 3, 0); //back to original
   assert(testBoard[0][0]=='1');
   assert(testBoard[0][1]=='0');
   assert(testBoard[0][2]=='0');
   assert(!(testBoard[0][0]=='0'));
   assert(!(testBoard[0][1]=='1'));
   assert(!(testBoard[0][2]=='1'));
   moveUp(testBoard, 3, 3, 1);
   assert(testBoard[0][1] == '1');
   assert(testBoard[1][1] == '0');
   assert(testBoard[2][1] == '0');
   assert(!(testBoard[0][1] == '0'));
   assert(!(testBoard[1][1] == '1'));
   assert(!(testBoard[2][1] == '1'));
   moveDown(testBoard, 3, 3, 1); //back to original
   assert(testBoard[0][1] == '0');
   assert(testBoard[1][1] == '1');
   assert(testBoard[2][1] == '0');
   assert(!(testBoard[0][1] == '1'));
   assert(!(testBoard[1][1] == '0'));
   assert(!(testBoard[2][1] == '1'));
   assert(!(testBoard[2][1] == 'a'));
   assert(!(testBoard[2][1] == '!'));
   /* move ones to top */
   moveUp(testBoard, 3, 3, 1);
   moveDown(testBoard, 3, 3, 2);
   assert(isSolution(3, 3, testBoard));

   char TestBoard2[MAX_DIM][MAX_DIM] = {
      {'0','0','0','1','0'},
      {'0','1','0','1','0'},
      {'0','0','0','1','0'},
      {'0','0','0','0','0'},
      {'0','0','1','0','0'},
	};
   char testBoard2[MAX_DIM][MAX_DIM];
   initBoard(5, 5, TestBoard2, testBoard2);
   assert(!isSolution(5, 5, testBoard2));
   assert(checkOnesInBoard(5, 5, testBoard2));
   checkValidBoard(5, 5, testBoard2);

   moveRight(testBoard2, 5, 5, 1);
   assert(testBoard2[1][0]=='0');
   assert(testBoard2[1][1]=='0');
   assert(testBoard2[1][2]=='1');
   assert(testBoard2[1][3]=='0');
   assert(testBoard2[1][4]=='1');
   moveLeft(testBoard2, 5, 5, 1); //back to original
   assert(testBoard2[1][0]=='0');
   assert(testBoard2[1][1]=='1');
   assert(testBoard2[1][2]=='0');
   assert(testBoard2[1][3]=='1');
   assert(testBoard2[1][4]=='0');
   moveUp(testBoard2, 5, 5, 3); 
   assert(testBoard2[0][3] == '1');
   assert(testBoard2[1][3] == '1');
   assert(testBoard2[2][3] == '0');
   assert(testBoard2[3][3] == '0');
   assert(testBoard2[4][3] == '1');
   assert(!(testBoard2[0][3] == '0'));
   assert(!(testBoard2[1][3] == '0'));
   assert(!(testBoard2[2][3] == '1'));
   assert(!(testBoard2[3][3] == '-'));
   assert(!(testBoard2[4][3] == '['));
   moveDown(testBoard2, 5, 5, 3); //back to original
   assert(testBoard2[0][3] == '1');
   assert(testBoard2[1][3] == '1');
   assert(testBoard2[2][3] == '1');
   assert(testBoard2[3][3] == '0');
   assert(testBoard2[4][3] == '0');
   assert(!(testBoard2[0][3] == '0'));
   assert(!(testBoard2[1][3] == 1));
   assert(!(testBoard2[2][3] == '0'));
   assert(!(testBoard2[3][3] == '1'));
   assert(!(testBoard2[4][3] == '1'));
   /* move ones to top */
   moveUp(testBoard2, 5, 5, 1);
   moveDown(testBoard2, 5, 5, 2);
   moveRight(testBoard2, 5, 5, 1);
   moveRight(testBoard2, 5, 5, 2);
   moveUp(testBoard2, 5, 5, 4);
   moveRight(testBoard2, 5, 5, 1);
   moveUp(testBoard2, 5, 5, 0);
   assert(isSolution(5, 5, testBoard2));
}
