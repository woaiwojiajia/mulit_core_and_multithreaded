#include <stdio.h>
#include <windows.h>
#include <iostream>
const int numThreads = 2;
using namespace std;

int main()//(int argc, char *args[]) 
{  
	int speed=1; //乌龟速度
	int length=50;//=atoi(args[1]);
	//cout<<args[1];
	int run=0;//乌龟已经跑的长度
	int f;
	HANDLE GuiEvent=OpenEvent(EVENT_ALL_ACCESS,FALSE,(LPCWSTR)"GuiWin");
	HANDLE TuEvent=OpenEvent(EVENT_ALL_ACCESS,FALSE,(LPCWSTR)"TuWin");
	DWORD r;
	while(true)
	{
		r=WaitForSingleObject(TuEvent,1);
		if(r==0)
		{
			printf("\n乌龟：兔子你跑得太快了，我输了!!!\n");
			break ;
		}
        run=run+speed;//跑
		if(run>=length)//判断是否到达终点
		{
			
			printf("\n乌龟:已经到达终点！！");
			SetEvent(GuiEvent);
			break;
		}
		printf("\n乌龟:已经跑了%d米啦！！",run);
		if(run%10==0) //乌龟每跑10米休息3秒
		{
			printf("\n乌龟:好累啊，休息一会.......");
			Sleep(3000);
		}
		else
			Sleep(1000);
	}
	system("pause");
}
