



//#include "StdAfx.h"
#include ".\threadDesc.h"
//#include ".\ARpfm.h"
#ifdef THREADED

IMPLEMENT_DYNCREATE(threadDesc, CThread)

threadDesc::threadDesc(void)
{

}

threadDesc::~threadDesc(void)
{
}






int threadDesc::Run_Thread()
{ 

	switch(_type){
		case CThread::ThreadType::threadDescrip :
			{
				while(1){			
					int action=agent->update();

					if(action != 0){
						actions.push_back(action);
					}				
					Sleep(50);//CPU overload avoidance
				}
			}
			break;
/*		case CThread::ThreadType::threadGaussPyr :
			{
				
			}
			break;*/
	}

	m_bPause = true;
	*m_pFinish = 1;
	
	return 1;
}



int		threadDesc::getNewAction(){

	int action=0;


	if(actions.size() > 0 ){
		action =(int) (*actions.begin());//horrible stuff!!
		actions.pop_front();

		//______________________________________________
		string s="the actions listed are:";
		list<int>::iterator i;
		for(i=actions.begin(); i!= actions.end(); i++){
			char* buf=new char[128];
			sprintf(buf,"%d",(*i) );
			s += buf;
			s += ", ";
		}
		s += "\n";
		SEND2LOG((char*) s.c_str());
	}	
	return action;


}


int		threadDesc::TramaInit(char* rulesfile){

		agent= new TAgent(rulesfile);	
		val=agent->init();//this should also be done inside the thread
		return val;
}



int		threadDesc::addDelay(char* name,double time){
		string a=name;
		TActionResource* ar=NULL;
		ar=agent->getNet()->GetActionResource(a);
		if(ar != NULL){
			ar->AddDelay(time);
			val=1;
		}
		return val;
	}

int		threadDesc::SetTimeLength(char* actionName,double time){  //note this is not duration!!

	//int val=0;
	string a=actionName;
	TActionResource* ar=NULL;
	ar=agent->getNet()->GetActionResource(a);
	if(ar != NULL){
		double tmin=ar->getTmin();
		ar->SetTmax( tmin+time);
		val=1;

	}
	return val;
}







void threadDesc::End()
{
	if(WaitForThread(INFINITE) == WAIT_OBJECT_0)
	{
		SuspendThread();
		ReleaseMutex(m_hMutex);
	}
}


//void threadDesc::setParamDESC(int tipo, TAgent* a, string message)
void threadDesc::setParamDESC(int tipo, string message)
{
	_type=tipo;
//	agent=a;
	say=message;
	action=0;

	

}
/*
void threadDesc::setParamPYR(int tipo)
{
	_type = tipo;
	_init = NULL;
	_end= NULL;
	_idRef = NULL;
	


}*/

#endif