#include<stdio.h>
#include<pthread.h>
void * threadfunc(void *pvoid)
{
	int id=(int)pvoid;
	printf("Child thread %d says:Hello Zhaoshuangquan\n",id);
	
	return NULL;
} 

int main()
{
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,&threadfunc,1);
	pthread_create(&tid2,NULL,&threadfunc,2);
	
	pthread_detach(tid1);
	pthread_join(tid2,NULL);
	printf("Main thread says:Hello\n");
	system("pause"); 
	return 0;
}
