#include <stdio.h>
#include <windows.h>
#include <iostream>
const int numThreads = 2;
using namespace std;
int main() 
{	
	
	int length=50;
	
	cout<<"\n龟兔赛跑：乌龟每秒钟跑1米，跑10米休息休息3秒；"
	     <<"兔子每秒跑3米，跑15米休息20秒"
	      <<"\n比赛长度50米";

	printf("\n裁判：比赛开始！！！");
	
    HANDLE hThread[2];
	hThread[0]=CreateEvent(NULL,TRUE,FALSE,(LPCWSTR)"GuiWin");
	hThread[1]=CreateEvent(NULL,TRUE,FALSE,(LPCWSTR)"TuWin");
	
	DWORD r=WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
	if(r==0)
		printf("\n\t\t裁判：恭喜乌龟赢得了比赛！！！\n");
	else
		printf("\n\t\t裁判：恭喜兔子赢得了比赛！！！\n");
	
     
     CloseHandle(hThread[0]);
     CloseHandle(hThread[1]);

	system("pause");
	return 0; 
}

