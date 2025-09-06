#pragma once

#include "../Timepath_header.h"

#ifdef THREADED
#include "thread.h"
#include <vector>



#include "../TAgent.h"

//for AddDelay:
#include "../TLlogicNetwork.h" 
#include "../TActionResource.h"




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

//	void					setParamDESC(int tipo, TAgent* a, string message);
	
	void					setParamDESC(int tipo, string message);

	//void					setParamPYR(int tipo, TAgent* a);
	void					setParamPYR(int tipo, string message);
	void					SetFinishBool(bool * _pBool) { m_pFinish = _pBool; }

	int						getNewAction();				



	int						TramaInit(char* rulesfile);
	int						updatePerception(int a,double value){return agent->updatePerception( a, value);}
	int						updateResource(char* name,double value){return agent->updateResource( name, value);}
	int						addDelay(char* name,double time);
	int						SetTimeLength(char* actionName,double time);

private:

	 struct Storage
	{ 		
		char* name;
		int val;
		double value;

	} percep, resour,delay,time;

	
	int _type;

	TAgent* agent;
	bool *		m_pFinish;
	string say;
	int action;
	list<int> actions;

	int val;// a simple validity check



};
#endif