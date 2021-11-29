#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

int main(void)
{
   char maze[100][100];
   int count = 0;
   FILE* fp=fopen("/home/parallels/Desktop/C-coursework/maze/maze.txt", "r");
   while (fgets(maze[count], 10, fp))
   { //fgetc
      // printf("%s", maze[count]);
      count++;
   }
   for (int i = 0; i < count; i++)
   {
      printf("%s", maze[i]);
   }
   fclose(fp);
}