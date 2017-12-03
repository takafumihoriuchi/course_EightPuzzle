#include <stdio.h>

void print_array(int array[][3])
{
   int i, j;
   for (i=0; i<3; i++) {
      for (j=0; j<3; j++) {
         printf(" %d", array[i][j]);
      }
      putchar('\n');
   }
}

int main(void)
{
   int array[4][3][3] = {
      {{0,1,2},{3,4,5},{6,7,8}},
      {{0,2,4},{6,8,1},{3,5,7}},
      {{1,3,5},{7,2,4},{6,8,0}},
      {{0,8,7},{6,5,4},{3,2,1}}
   };
   print_array(array[2]);
   return 0;
}