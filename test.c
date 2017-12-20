#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int n = 10;

   int *a;
   a = (int *)malloc(n * sizeof(int));
   int i;
   for (i=0; i<n; i++) {
   	a[i] = i;
   }

   /*
   int *b;
   b = (int *)malloc(n * sizeof(int));
   for (i=0; i<n; i++) {
   	b[i] = i*2;
   }
   */

   a = (int *)malloc(n * sizeof(int));
   for (i=0; i<n; i++) {
   	a[i] = i*2;
   }

   for (i=0; i<n; i++) {
   	printf("a[%d] = %d\n", i, a[i]);
   }
   /*
   for (i=0; i<n; i++) {
   	printf("b[%d] = %d\n", i, b[i]);
   }
   */

   return 0;
}