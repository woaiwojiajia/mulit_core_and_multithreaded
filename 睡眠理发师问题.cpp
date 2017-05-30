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
	printf("\n������쿪ʼӪҵ����ӭ��λ�˿͹��٣���");
	SetEvent(hDoor);
	while(true)
	{
		if(WaitForSingleObject(hFull,1)==WAIT_TIMEOUT)
		{
			printf("\n��ʦ��û�й˿��ˣ�˯һ���......");
			//ResetEvent(hBaberReady);
			if(WaitForSingleObject(hFull,10000)==WAIT_TIMEOUT)
			{
				printf("\n��ʦ����ô��ʱ�䶼û�й˿ͣ������ˣ���");
				ResetEvent(hDoor);
				return 0;
			}
		}
		else
		{
			printf("\n��ʦ������һλ....");
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
		printf("\n�˿�%d�����껹û�п��Ű�����һ���......", id);
		if(WaitForSingleObject(hDoor,10000)==WAIT_TIMEOUT)
		{
			printf("\n�˿�%d������ô��ʱ�䶼�������ţ����������ɣ���", id);
			return 0;
		}
	}
	EnterCriticalSection(&cs);
	CCount++;
	if(CCount==1)
	{
		printf("\n�˿�%d ����ʦ�����ѣ���Ҫ��......", id);
		ReleaseSemaphore(hFull,1,NULL);
		LeaveCriticalSection(&cs);
	}
	else
	{
		if(WaitForSingleObject(hEmpty,1)==WAIT_TIMEOUT)
		{
			printf("\n�˿�%d ���ö��˰�������λ��û�У��´�������......",id);
			LeaveCriticalSection(&cs);
			return 0;
		}
		printf("\n�˿�%d ����ʦ��æ�����Һû���%d����λ����һ���......", id,CHAIR_NUM+2-CCount);
		in=(in+1)%CHAIR_NUM;
		LeaveCriticalSection(&cs);
		ReleaseSemaphore(hFull,1,NULL);
		WaitForSingleObject(hChairs[in],INFINITE);
		printf("\n�˿�%d �������ֵ����ˣ�", id);
	}
	printf("\n�˿�%d ����ʼ��......", id);
	Sleep(CUT_TIME);
	printf("\n�˿�%d�����������뿪����......", id);

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
