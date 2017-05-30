#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define CHAIR_NUM 5
#define ARRIVE_TIME 5000
#define CUT_TIME 5000
int num_empty_chair=CHAIR_NUM;
HANDLE hFull=CreateSemaphore(NULL, 0,CHAIR_NUM,NULL);
HANDLE hEmpty=CreateSemaphore(NULL, CHAIR_NUM,CHAIR_NUM,NULL);
HANDLE hDoor= CreateEvent(NULL, TRUE,FALSE, NULL);
//HANDLE hBaberReady= CreateEvent(NULL, FALSE,TRUE, NULL);
HANDLE hCutOver= CreateEvent(NULL, FALSE,FALSE, NULL);
int in=-1,out=0;
int CCount=0;
HANDLE hChairs[CHAIR_NUM];

CRITICAL_SECTION cs;

DWORD WINAPI Barber(LPVOID p)
{
	printf("\n本店今天开始营业，欢迎各位顾客光临！！");
	SetEvent(hDoor);
	while(true)
	{
		if(WaitForSingleObject(hFull,1)==WAIT_TIMEOUT)
		{
			printf("\n理发师：没有顾客了，睡一会吧......");
			//ResetEvent(hBaberReady);
			if(WaitForSingleObject(hFull,10000)==WAIT_TIMEOUT)
			{
				printf("\n理发师：这么长时间都没有顾客，关门了！！");
				ResetEvent(hDoor);
				return 0;
			}
		}
		else
		{
			printf("\n理发师：请下一位....");
			SetEvent(hChairs[out]);
		//SetEvent(hBaberReady);
			out=(out+1)%CHAIR_NUM;
			ReleaseSemaphore(hEmpty,1,NULL);
		}
		WaitForSingleObject(hCutOver,INFINITE);
	}
}
DWORD WINAPI Custom(LPVOID p)
{
	int id=(int)p;
	if(WaitForSingleObject(hDoor,1)==WAIT_TIMEOUT)
	{
		printf("\n顾客%d：理发店还没有开门啊，等一会吧......", id);
		if(WaitForSingleObject(hDoor,10000)==WAIT_TIMEOUT)
		{
			printf("\n顾客%d：：这么长时间都还不开门，明天再来吧！！", id);
			return 0;
		}
	}
	EnterCriticalSection(&cs);
	CCount++;
	if(CCount==1)
	{
		printf("\n顾客%d ：理发师快醒醒，我要理发......", id);
		ReleaseSemaphore(hFull,1,NULL);
		LeaveCriticalSection(&cs);
	}
	else
	{
		if(WaitForSingleObject(hEmpty,1)==WAIT_TIMEOUT)
		{
			printf("\n顾客%d ：好多人啊，连座位都没有，下次再来吧......",id);
			LeaveCriticalSection(&cs);
			return 0;
		}
		printf("\n顾客%d ：理发师很忙啊，幸好还有%d个座位，等一会吧......", id,CHAIR_NUM+2-CCount);
		in=(in+1)%CHAIR_NUM;
		LeaveCriticalSection(&cs);
		ReleaseSemaphore(hFull,1,NULL);
		WaitForSingleObject(hChairs[in],INFINITE);
		printf("\n顾客%d ：终于轮到我了！", id);
	}
	printf("\n顾客%d ：开始理发......", id);
	Sleep(CUT_TIME);
	printf("\n顾客%d：理发结束，离开理发店......", id);

	EnterCriticalSection(&cs);
	CCount--;
	LeaveCriticalSection(&cs);

	SetEvent(hCutOver);
}
void main()
{
	InitializeCriticalSection(&cs);
	 for (int i = 0; i < CHAIR_NUM; i++)
             hChairs[i] =CreateEvent(NULL,FALSE,FALSE, NULL);
	 HANDLE t[11];
	 t[0]=CreateThread(NULL, 0, Barber,NULL, 0, NULL );
	 Sleep(2000);
	 for(int i=1;i<=10;i++)
	 {
		 t[i]=CreateThread(NULL, 0, Custom,(LPVOID)i,0, NULL );
		 Sleep(2000);
	 }
	 WaitForMultipleObjects(11, t, TRUE, INFINITE);

	DeleteCriticalSection(&cs);

}
