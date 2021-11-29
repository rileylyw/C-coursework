#include "crossword.h" //add back Werror

// Might be useful to be able to print them
// to hep with debugging
void print_crossword(const crossword *c)
{
   for (int y = 0; y < c->sz; y++)
   {
      for (int x = 0; x < c->sz; x++)
      {
         printf("%c", c->arr[y][x]);
      }
      printf("\n");
   }
}

bool str2crossword(int sz, char *ip, crossword *cw)
{ //build crossword board
   if (ip == NULL || //if not 'x.* ' -->for loop function
       strcmp(ip, "") == 0 ||
       cw == NULL ||
       sz <= 0 || sz > 99)
   { //float?
      return false;
   }
   if (((int)strlen(ip) % sz) != 0 || (int)strlen(ip) == sz || 
         (int)strlen(ip)/sz != sz)
   {
      return false;
   }
   return true;
}

/*

int getchecked(crossword c)
{ //get percentage of intersections
}
*/


void getcluestring(const crossword* c, char* ans)
{//populate the crosswords
   for (int i=0; i<BIGSTR; i++){
      if(str[i-2]=="X" || str[i-2]=="*"){
         str[i-1]=
      }
   }

   // c->arr[(c->sz)][(c->sz)];
   // printf("%s\n", c->arr);

   // char str[BIGSTR] = "....X.XX.X.X......X.XX...";
   // c->arr = str;

   // c->arr[(strlen(str)/c->sz)][(strlen(str)/c->sz)]

   // c->arr[0][0]={'A', 'D'};
   
   // for(int i=0; i<GRID; i++){
   //    for(int j=0; j<GRID; j++){
   //       printf("i %d\n", c->arr[i]);
   //       printf("j %d\n", c->arr[j]);
   //    }
   // }
}

void test(void)
{
   crossword a;
   assert(!str2crossword(5, ".....", &a));
   assert(!str2crossword(5, "..........", &a));//10
   assert(str2crossword(5, ".........................", &a));//25
   assert(!str2crossword(999, ".....", &a));
   //assert(!str2crossword(7, "asdflkjh", &a));
   //assert(!str2crossword(3, "asdflkjasdf", &a)); //9
   printf("test done\n");
}
