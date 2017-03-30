#include<stdio.h>
#include<Windows.h>
#include<iostream>
using namespace std;

DWORD WINAPI Tortoise(LPVOID arg);
DWORD WINAPI Rabit(LPVOID arg);
HANDLE hThread[2];

DWORD WINAPI Tortoise(LPVOID arg){
	int speed=1;
	int length=*(int*)arg;
	int run=0;//乌龟已经跑的长度
	
	while(true)
	{
		if(WaitForSingleObject(hThread[1],1)==0)
		{
			printf("\n乌龟：兔子你跑得太快了，我输了!!!\n");
			break ;
		}
        run=run+speed;//跑
		if(run>=length)//判断是否到达终点
		{
			
			printf("\n乌龟:已经到达终点！！");
			
			break;
		}
		printf("\n乌龟:已经跑了%d米啦！！",run);
		
		Sleep(1000);
	}
	return 0;
}

DWORD WINAPI Rabit(LPVOID arg){
	int speed=3;
	int length=*(int*)arg;
	int run=0;//兔子已经跑的长度
	while(true)
	{
		if(WaitForSingleObject(hThread[0],1)==0)
		{
			printf("\n\t\t\t兔子：乌龟你跑得太快了，我输了!!!\n");
			break ;
		}
        run=run+speed;//跑
		if(run>=length)//判断是否到达终点
		{	
			printf("\n\t\t\t兔子:已经到达终点！！");
			break;
		}
		printf("\n\t\t\t兔子:已经跑了%d米啦！！",run);
		if(run%10==0) //兔子每跑10米休息3秒
		{
			printf("\n\t\t\t兔子:好累啊，休息一会.......");
			Sleep(15000);
		}
		else
			Sleep(1000);
	}
	return 0;
}



int main()
{
	int length;
	cout<<"\n龟兔赛跑：乌龟每秒跑1米，不休息";
	cout<<"\n兔子每秒跑3米，跑9米休息180秒";
	cout<<"\n请输入赛道长度（单位：米）";
	cin>>length;
	printf("\n裁判：比赛开始！！！\n");

	hThread[0]=CreateThread(NULL,0,Tortoise,(LPVOID)&length,0,NULL);
	hThread[1]=CreateThread(NULL,0,Rabit,(LPVOID)&length,0,NULL);

	DWORD r=WaitForMultipleObjects(2,hThread,FALSE,INFINITE);
	if(r==0)
	{
		printf("\n裁判：乌龟胜利\n");		
	}
	else
	{
		printf("\n裁判：兔子胜利\n");
	}
	WaitForMultipleObjects(2,hThread,TRUE,INFINITE);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	
	system("pause");

}
