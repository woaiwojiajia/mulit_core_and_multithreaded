#include<pthread.h>
#include<stdio.h>
#include<time.h>
#define PASSENGERCOUNT 20

int totalStation=5;
int CurrentStationIndex=-1;

int door=0;//0��ʾ����״̬��1��ʾ����״̬ 
pthread_mutex_t mutexdoor=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conddoor=PTHREAD_COND_INITIALIZER;

int run=0;//0��ʾͣ��״̬��1��ʾ��ʻ״̬
pthread_mutex_t mutexrun=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condrun=PTHREAD_COND_INITIALIZER;

//int station=0;
pthread_mutex_t mutexstation=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conddstation=PTHREAD_COND_INITIALIZER;

void *driver(void *pvoid)
{
	pthread_mutex_lock(&mutexrun);
	printf("˾�������ѷ������뿪��վ\n");
	run=1;
	pthread_mutex_unlock(&mutexrun);
	
	while(1)
	{
		sleep(1);
		
		pthread_mutex_lock(&mutexstation);
		CurrentStationIndex++;
		printf("˾���������ѵ����%dվ���³��ĳ˿���Ӻ����³�\n",CurrentStationIndex);
		pthread_mutex_unlock(&mutexstation);
		
		pthread_mutex_lock(&mutexrun);
		run=0;
		pthread_cond_signal(&condrun);
		pthread_mutex_unlock(&mutexrun);
		
		
	}

	return NULL;
} 

void *conductor(void *pvoid)
{
	
	while(1)
	{
		pthread_mutex_lock(&mutexrun);
		
		if(run==0)
		{
			door=1;
			printf("\t��ƱԱ���򿪳���\n");
			pthread_cond_signal(&conddoor); 
			
		}
	}

	return NULL;
}


void *passenger(void *pvoid)
{
	int sleeptime;
	sleeptime=rand()%10;
	int i=(int )pvoid;
	int takeon,takeoff;
	takeoff=0;
	sleep(sleeptime);
	printf("\t\t\t\t\t�˿�%d: ���Ѿ������%d��վ�����ڵȳ�\n",i,takeon) ;
	while(1)
	{
		
	}
    return NULL;
}

int main()
{
	
	int i;
	
	pthread_t pdriver;
	pthread_t pconductor;
	pthread_t ppassenger[PASSENGERCOUNT];
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	pthread_create(&pdriver,&attr,driver,NULL);
	pthread_create(&pconductor,&attr,conductor,NULL);
	for(i=1;i<=PASSENGERCOUNT;i++)
	{

    	pthread_create(&ppassenger[i],&attr,passenger,(void *)i); 
    	sleep(5); 
	}
	
	pthread_join(pdriver,NULL);
	pthread_join(pconductor,NULL);

	for(i=0;i<PASSENGERCOUNT;i++)
	{
		pthread_join(ppassenger[i],NULL);
	} 

	pthread_attr_destroy(&attr);
	
	pthread_exit(NULL);
	
	return 0;
}
