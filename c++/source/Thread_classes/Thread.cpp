
#include "stdafx.h"
#include "Thread.h"

#ifdef THREADED


IMPLEMENT_DYNCREATE(CThread, CWinThread)

CThread::CThread() : CWinThread()
{
	m_hMutex = CreateMutex(	NULL,                       // default security attributes
							FALSE,                      // initially not owned
							NULL);                      // unnamed mutex

	m_bPause = false;

	m_dwWait = 10;
}

CThread::~CThread()
{
	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
}

BOOL	
CThread::InitInstance() 
{ 
	return TRUE;//CWinThread::InitInstance(); 
}

int		
CThread::ExitInstance()  
{ 
	return CWinThread::ExitInstance(); 
}

BOOL	
CThread::OnIdle(LONG lCount)  
{ 
	return CWinThread::OnIdle(lCount); 
}

BOOL	
CThread::PreTranslateMessage(MSG * pMsg) 
{ 
	return CWinThread::PreTranslateMessage(pMsg); 
}

LRESULT 
CThread::ProcessWndProcException( CException * pE, const MSG * pMsg ) 
{ 
	return CWinThread::ProcessWndProcException(pE,pMsg);
}

int
CThread::Run()
{
	while (true)
	{
		if(!m_bPause)
			Run_Thread();

		Sleep(m_dwWait);
	}

	return 0;
}

DWORD
CThread::WaitForThread(DWORD dwMilliseconds)
{
	return WaitForSingleObject( 
					m_hMutex,			// handle to mutex
					dwMilliseconds);	// five-second time-out interval
}

void
CThread::Pause(bool bEnabled) 
{ 
	if(WaitForThread(INFINITE) == WAIT_OBJECT_0)
	{
		m_bPause = bEnabled; 
		ReleaseMutex(m_hMutex);
	}
}

bool
CThread::bIsPaused() 
{ 
	return m_bPause; 
}

#endif