// Copyright (C) 1991 - 1999 Rational Software Corporation
#pragma once

#include <vector>
#include "Thread.h"


class ThreadManager
{
	public:
		
		ThreadManager();
		~ThreadManager();
		CWinThread *						pCreate();
		static inline ThreadManager *		pGet() { return m_pSingleton; };

		unsigned int				uiCreateMutex();
		void						DestroyMutex(unsigned int _uiMutex);
		void						WaitForMutex(unsigned int _uiMutex);
		void						ReleaseTheMutex(unsigned int _uiMutex);

	protected:

		static ThreadManager *		m_pSingleton;

		std::vector<HANDLE>			m_vMutex;
};

