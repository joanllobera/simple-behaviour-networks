#pragma once

#include "thread.h"
//#include "util.h"
#include <vector>
/*
#include ".\\include\\cxcore.h"
#include ".\\include\\cv.h"
#include ".\\include\\highgui.h"
*/
using namespace std;

class threadDesc:
	public CThread
{
public:
	threadDesc(void);
	~threadDesc(void);

	DECLARE_DYNCREATE(threadDesc)

	virtual int				Run_Thread();
	virtual void			End();

	void					setParamDESC(int tipo,  int start,int endo ,int id);
//	void					setParamDESC(int, int,int,IplImage***,int,vector<feature*>&,double***);
	//void					setParamPYR(int,int,int,double*,IplImage***/*,IplImage****/);
	void					setParamPYR(int tipo);
	void					SetFinishBool(bool * _pBool) { m_pFinish = _pBool; }

private:
	


	int _type;
	bool *		m_pFinish;
	int _init;
	int _end;
	int _idRef;
	
};
