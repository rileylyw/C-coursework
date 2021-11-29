/*
   input a : A square 2D Boolean array (maximum size 15x15)
   input n : How much of the 15x15 array is actually being
             used, e.g. 2x2, 3x3, etc.
   output : Returns true if all elements horizontally adjacent to
            each other (left & right) are different AND if
            elements vertically adjacent (up & down) are different
*/

#include "common.h"

#define T true
#define F false

bool adj(const bool a[15][15], int n);

int main(void)
{

   /* Valid : 0 1
              1 0 */
   const bool e[15][15] = {{F,T}, {T,F}};
   // bool x=adj(e,2);
   assert(adj(e,2)==true);
   
   /* Valid : 0 1 0
              1 0 1
              0 1 0 */
   const bool a[15][15] = {{F,T,F}, {T,F,T}, {F,T,F}};
   assert(adj(a,3)==true);

   /* Valid : 0 1 0 1 0
              1 0 1 0 1
              0 1 0 1 0
              1 0 1 0 1
              0 1 0 1 0 */
   const bool c[15][15] = { {F,T,F,T,F}, {T,F,T,F,T}, {F,T,F,T,F}, {T,F,T,F,T}, {F,T,F,T,F}};
   assert(adj(c,5)==true);

   /* Invalid : (adjacent cells vertically are the same)
                0 1 0
                0 1 0
                0 1 0 */
   const bool b[15][15] = {{F,T,F}, {F,T,F}, {F,T,F}};
   assert(adj(b,3)==false);
   /* Invalid : (Bottom right cell should be 0)
              0 1 0 1 0
              1 0 1 0 1
              0 1 0 1 0
              1 0 1 0 1
              0 1 0 1 1 */
   const bool d[15][15] = { {F,T,F,T,F}, {T,F,T,F,T}, {F,T,F,T,F}, {T,F,T,F,T}, {F,T,F,T,T}};
   assert(adj(d,5)==false);

   return 0;
}
