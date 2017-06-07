#define  PASSENGERCOUNT 20  
class CPassengerBusQuestion  
{  
public:  
    CPassengerBusQuestion(void);  
    virtual ~CPassengerBusQuestion(void);  
  
private:  
    CCriticalSection m_cs;  
public:  
  
    CWinThread *m_pDriverThread;  
    CWinThread *m_pConductorThread;  
    CWinThread *m_pPassengerThreadArr[PASSENGERCOUNT];  
    HANDLE m_eventBusStop;  
    HANDLE m_eventConductorClosedoor;  
    HANDLE m_eventConductorOpenDoor;  
      
    int m_nTotalStation;  
    int m_nCurStationIndex;  
public:  
    void Init();  
    static UINT Driver_Proc(LPVOID lpParam);  
    static UINT Conductor_Proc(LPVOID lpParam);  
    static UINT Passenger_Proc(LPVOID lpParam);  
  
private:  
    void OutputLog(char *format,...);  
  
};  

 

[cpp] view plain copy print?
static char StationName[7][20] =  
{  
    "��վ1",  
    "��վ2",  
    "��վ3",  
    "��վ4",  
    "��վ5",  
    "��վ6",  
};  
  
typedef struct tagThreadParam  
{  
    CPassengerBusQuestion *pBusQuestion;  
    int index;// Passenger NO.  
    int nGetOnStation;//The tation NO Passenger gets on.  
    BOOL bLastPerson;//if Passenger is the Last Person   
}ThreadParam;  
  
void CPassengerBusQuestion::Init()  
{  
    m_nTotalStation = 4;  
    m_nCurStationIndex = -1;  
    m_pDriverThread = NULL;  
    m_pConductorThread = NULL;  
    m_eventConductorOpenDoor = NULL;  
    m_eventConductorClosedoor = NULL;  
    m_eventBusStop = NULL;  
  
  
    m_eventBusStop = ::CreateEvent(NULL,FALSE,FALSE,NULL);  
    m_eventConductorClosedoor = ::CreateEvent(NULL,FALSE,FALSE,NULL);  
    m_eventConductorOpenDoor  = ::CreateEvent(NULL,TRUE,FALSE,NULL);  
  
    m_pDriverThread = AfxBeginThread(CPassengerBusQuestion::Driver_Proc,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);  
    m_pConductorThread = AfxBeginThread(CPassengerBusQuestion::Conductor_Proc,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);  
    for(int i =0;i< PASSENGERCOUNT;i++)  
    {  
        ThreadParam* pParam =new ThreadParam;  
        pParam->pBusQuestion = this;  
        pParam->index = i;  
        pParam->nGetOnStation = i%5;  
  
        m_pPassengerThreadArr[i] = AfxBeginThread(CPassengerBusQuestion::Passenger_Proc,pParam,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);  
    }  
  
    m_pDriverThread->ResumeThread();  
    m_pConductorThread->ResumeThread();  
  
    for(int i =0;i<PASSENGERCOUNT;i++)  
    {  
        m_pPassengerThreadArr[i]->ResumeThread();  
    }  
}



//˾���߳�  
UINT CPassengerBusQuestion::Driver_Proc(LPVOID lpParam)  
{  
    bool bExitThread=false;  
    CPassengerBusQuestion* pBusQuestion =(CPassengerBusQuestion *)lpParam;  
  
    while(!bExitThread)  
    {  
        pBusQuestion->OutputLog("˾����������ʼ��ʻ....\n");  
        ::Sleep(5000);  
        pBusQuestion->OutputLog("˾������վ\n");  
        pBusQuestion->m_nCurStationIndex++;  
  
        SetEvent(pBusQuestion->m_eventBusStop);  
        ::WaitForSingleObject(pBusQuestion->m_eventConductorClosedoor,INFINITE);  
        if( pBusQuestion->m_nCurStationIndex == pBusQuestion->m_nTotalStation)  
        {  
            bExitThread = TRUE;  
        }  
  
    }  
    return 0;  
}  
  
//��ƱԱ�߳�  
UINT CPassengerBusQuestion::Conductor_Proc(LPVOID lpParam)  
{  
    bool bExitThread = false;  
    CPassengerBusQuestion* pBusQuestion =(CPassengerBusQuestion *)lpParam;  
  
    while(!bExitThread)  
    {  
        ::WaitForSingleObject(pBusQuestion->m_eventBusStop,INFINITE);//�ȴ�˾��ͣ��  
        pBusQuestion->OutputLog("��ƱԱ���򿪳���\n");  
        pBusQuestion->OutputLog("��ƱԱ��%s����\n", StationName[pBusQuestion->m_currentStationIndex]);  
        SetEvent(pBusQuestion->m_eventConductorOpenDoor);  
        ::Sleep(3000);  //�ȴ��˿����³���    
        ::ResetEvent(pBusQuestion->m_eventConductorOpenDoor);//���¼�ֻ���ֹ��ƣ������Զ��ã��ֹ�Ҳ���ܽ���ĳһ���˿͡�  
        pBusQuestion->OutputLog("��ƱԱ���رճ���\n");  
        SetEvent(pBusQuestion->m_eventConductorClosedoor);  
  
        if( pBusQuestion->m_nCurStationIndex == pBusQuestion->m_nTotalStation)  
        {  
            bExitThread = TRUE;  
        }  
  
    }  
  
    return 0;  
}  
  
//�˿��߳�  
UINT CPassengerBusQuestion::Passenger_Proc(LPVOID lpParam)  
{  
    ThreadParam* pParam = (ThreadParam *)lpParam;  
    int index = pParam->index;  //�ڼ��ų˿ͣ�  
    int nGetOnStation = pParam->nGetOnStation;  //�ڼ�վ�ϳ�  
    BOOL bLastPerson  = pParam->bLastPerson;//�Ƿ������һ���ϳ�����  
    CPassengerBusQuestion *pBusQuestion = pParam->pBusQuestion;  
    delete pParam;  
  
    bool bExitThread = false;  
    while(!bExitThread)  
    {  
        ::WaitForSingleObject(pBusQuestion->m_eventConductorOpenDoor,INFINITE);  
        if(pBusQuestion->m_nCurStationIndex == nGetOnStation)//�жϹ������Ƿ񵽳�վ�ˡ�  
        {  
            pBusQuestion->OutputLog("�˿�%d:�� %s�Ѿ��ϳ�\n",index,StationName[nGetOnStation]);  
            bExitThread = TRUE;  
        }  
        ::Sleep(1000);  
    }  
    return 0;  
}  


//��������  
void CPassengerBusQuestion::OutputLog( char *format,... )  
{  
    m_cs.Lock();  
    char temp[1024]={0};  
    va_list arg_ptr;  
    va_start(arg_ptr, format);  
    int nWrittenBytes = vsprintf(temp, format, arg_ptr);  
    va_end(arg_ptr);  
    printf(temp);  
    m_cs.Unlock();  
}  