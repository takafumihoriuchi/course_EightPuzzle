#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void)
{
	srand(time(NULL));
	int i;
	for (i=0; i<100; i++) {
		int random_number = rand()%10;
		printf("random_number = %d\n", random_number);
	}
	return 0;
}
