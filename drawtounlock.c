#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define BIGSTR 1000
#define GRID 3

int* patterns(char screen[GRID][GRID], char string[], int x, int y, int* count);
bool withinGrid(int a);
bool notOccupied(char n, char occupied[]);
void makeScreen(char screen[GRID][GRID]);
void test(void);

int main(void){
   char string[BIGSTR];
   char screen[GRID][GRID];
   makeScreen(screen);
   int count=0;
   strcpy(string, &screen[1][0]); //'4'
   int* patternsCount=patterns(screen, string, 1, 0, &count);
   printf("Number of patterns: %d\n", *patternsCount);
   return 0;
}

void makeScreen(char screen[GRID][GRID]){
   char num[GRID*GRID];
   for(int x=0; x<GRID*GRID; x++){
      num[x]=(char)((x+1)+'0');
   }
   for(int i=0; i<GRID; i++){
      for(int j=0; j<GRID; j++){
         screen[i][j]=num[i*GRID+j];
      }
   }
}

int* patterns(char screen[GRID][GRID], char string[], int x, int y, int* count){
   // int shift[3] = {0, -1, 1};
   // for(int i=0; i<3; i++){
   //    for(int j=0; j<3; j++){
   for(int i=-1; i<=1; i++){
      for(int j=-1; j<=1; j++){
         // if((screen[x][y])=='7'){
            if(i==0 || j==0){
               if(withinGrid(x+i)&&withinGrid(y+j)){
                  if(notOccupied(screen[x+i][y+j], string)){
                     // printf("%d, %d, %c\n", x+i, y+j, screen[x+i][y+j]);
                     strncat(string, &screen[x+i][y+j], 1);
                     // printf("%s\n", string);
                     patterns(screen, string, x+i, y+j, count);
                     string[strlen(string)-1]='\0'; //backtrack
                  }
               }
            }
         // }
      }
   }
   // if(strlen(string)==GRID*GRID && string[0]=='5')/{
   if(strlen(string)==GRID*GRID){
      printf("%s\n", string);
      // printf("count %d\n", *count);
      (*count)++;
      // printf("count %d\n", *count);
   }
   return count;
}

bool withinGrid(int a){
   if(a>=0 && a<GRID){
      return true;
   }
   return false;
}

bool notOccupied(char n, char occupied[]){
   int i=0;
   while(occupied[i]!='\0'){
      if(occupied[i]==n){
         return false;
      }
      i++;
   }
   return true;
}

void test(void){
   assert(withinGrid(0));
   assert(!withinGrid(-1));
   assert(!withinGrid(4)); //depends on GRID
}