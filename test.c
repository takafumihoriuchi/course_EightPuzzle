#include <stdio.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char **argv)
{
	clock_t c_start, c_end;
	time_t t_start, t_end;
	
	t_start = time(NULL);
	
	while(time(NULL) - t_start == 0);
	
	c_start = clock();
	t_start = time(NULL);
	
	while(time(NULL) - t_start < 2);
	
	c_end = clock();
	t_end = time(NULL);
	
	printf("%f\n", (double)(c_end - c_start) / CLOCKS_PER_SEC);
	printf("%ld\n", t_end - t_start);
	
	c_start = clock();
	t_start = time(NULL);
	
	sleep(2);
	
	c_end = clock();
	t_end = time(NULL);
	
	printf("%f\n", (double)(c_end - c_start) / CLOCKS_PER_SEC);
	printf("%ld\n", t_end - t_start);
	
	return 0;
}

/*
#include <stdio.h>
#include <time.h>

int main(void)
{
	clock_t start, end;
	double cpu_time_used;

	start = clock();

	int i, cnt=0;
	for (i=0; i<999999; i++) cnt++;
	for (i=0; i<999999; i++) cnt--;
	for (i=0; i<999999; i++) cnt++;
	//for (i=0; i<999999; i++) cnt--;

	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("cnt           = %d\n", cnt);
	printf("cpu_time_used = %f\n", cpu_time_used);

	return 0;
}
*/