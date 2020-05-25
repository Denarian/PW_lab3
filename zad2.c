#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

void * watek(void * arg);

int globalna = 0;

int main()
{
pthread_t tid[10];
int wyn;
for( int i = 0; i < 10; i++)
{
	wyn = pthread_create(&tid[i], NULL, watek, i);
	//pthread_join(tid[i],NULL);
}
for( int i = 0; i < 10; i++)
{
	pthread_join(tid[i],NULL);
}

printf("Globalna: %d\n", globalna);

return 0;
}

void * watek(void * arg)
{
	printf("%d - %li\n", arg,  pthread_self());
	globalna++;
	return NULL ;
}