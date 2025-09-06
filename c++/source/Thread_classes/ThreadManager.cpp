// Copyright (C) 1991 - 1999 Rational Software Corporation

// ooooooooooooooooooooooooo   Include Files   ooooooooooooooooooooooooo


#include "../Timepath_header.h"
#ifdef THREADED


#include "StdAfx.h"
#include "ThreadManager.h"

#include "threadDesc.h"

// ooooooooooooooooooooooooo    Definitions    ooooooooooooooooooooooooo



ThreadManager * ThreadManager::m_pSingleton = NULL;

ThreadManager::ThreadManager()
{
	if (m_pSingleton != NULL)
		return;

	m_pSingleton = this;
}

ThreadManager::~ThreadManager()
{
	m_pSingleton = NULL;
}

CWinThread * ThreadManager::pCreate()
{
	CWinThread * pThread = NULL;
	
	pThread = AfxBeginThread(RUNTIME_CLASS(threadDesc), THREAD_PRIORITY_HIGHEST, 0, CREATE_SUSPENDED);		
	
	return pThread;
}

unsigned int
ThreadManager::uiCreateMutex()
{
	HANDLE hMutex = CreateMutex( NULL, FALSE, NULL );
	if (hMutex)
	{
		m_vMutex.push_back(hMutex);
		return (unsigned int)(m_vMutex.size() - 1);
	}

	return 999999;
}

void
ThreadManager::DestroyMutex(unsigned int _uiMutex)
{
	if (_uiMutex != 999999)
	{
		ReleaseMutex(m_vMutex[_uiMutex]);
		CloseHandle(m_vMutex[_uiMutex]);
	}
}

void
ThreadManager::WaitForMutex(unsigned int _uiMutex)
{
	if (_uiMutex != 999999)
	{
		HANDLE hMutex = m_vMutex[_uiMutex];
		DWORD dwWaitResult = WaitForSingleObject( 
						hMutex,					// handle to mutex
						INFINITE);				// five-second time-out interval

		if (WAIT_FAILED == dwWaitResult)
		{
			TRACE("\nError Wait Mutex %d\n",_uiMutex);
		}
	}
}

void
ThreadManager::ReleaseTheMutex(unsigned int _uiMutex)
{
	if (_uiMutex != 999999)
	{
		ReleaseMutex(m_vMutex[_uiMutex]);
	}
}


#endif