#include <stdio.h>
#include <pthread.h>
void* print()
{
	printf("haha\n");
}
int main()
{
	pthread_t p;
	pthread_create(&p, NULL, print, NULL);
	sleep(2);
	return 0;
}
