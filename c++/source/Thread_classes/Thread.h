#pragma once
// Thread.h : header file
//


#include "../Timepath_header.h"
#ifdef THREADED
#include "stdafx.h"
#include <afxtempl.h>	// MFC templates
#include <winbase.h>	// Synchronization functions


class CThread;


// @class CThread Class Members
// @base public | CObject
class CThread : public CWinThread
{
public:
	DECLARE_DYNCREATE(CThread)

	CThread();
	virtual ~CThread();

	typedef enum
	{ //HERE SHOULD BE THE KINDS OF THREADS THAT WE WANT
		
		threadType1 = 0,

		threadDescrip = 1
	} ThreadType;
	
	virtual void	Execute() {};
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();
	virtual BOOL	OnIdle( LONG lCount );
	virtual BOOL	PreTranslateMessage( MSG *pMsg );
	virtual LRESULT ProcessWndProcException( CException *e, const MSG *pMsg );
	virtual void	End() {};
	HANDLE			GetMutex();
	virtual int		Run();
	virtual int		Run_Thread() { return 0; }
	DWORD			WaitForThread(DWORD dwMilliseconds);
	void			Pause(bool bEnabled);
	bool			bIsPaused();
	void			SetWaitTime(DWORD _dwTime) { m_dwWait = _dwTime; }

protected:	

	bool			m_bPause;
	HANDLE			m_hMutex;

	DWORD			m_dwWait;
};
#endif