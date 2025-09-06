//Timepath_export_header.h
//This files includes whatever might be exported in the DLL
// only used if EXTERNALNET is NOT defined in Timepath_header.h,
// and INTERNALAGENT is defined

#pragma once


/*******************************************************************************************************************

T H E      A G E N T


*******************************************************************************************************************/
#ifndef THREADED
#include "TAgent.h"

//for AddDelay:
#include "TLlogicNetwork.h" 
#include "TActionResource.h"

TAgent* theAgent;
int val;// a simple validity check


#endif

#ifdef THREADED
/*******************************************************************************************************************

4   T H R E A D S

*******************************************************************************************************************/
#include "./Thread_classes/threaddesc.h"
#include "./Thread_classes/ThreadManager.h"
bool bThread;
threadDesc* pThread;

int val;// a simple validity check

//void ThreadedUpdate();

void ThreadedInit(char* rulefile){
	SEND2LOG("\n Generating thread...\n \n");

	bThread = FALSE;
	new ThreadManager();
	pThread = (threadDesc*)ThreadManager::pGet()->pCreate();

	pThread->TramaInit(rulefile);
	pThread->Pause(true);
	pThread->ResumeThread();

	//	ThreadedUpdate();
	string s = "";
	int action = 0;
	bThread = FALSE;

	//	pThread->setParamDESC(CThread::ThreadType::threadDescrip,theAgent, s); //potser hauria de ser &TheAgent? 
	pThread->setParamDESC(CThread::ThreadType::threadDescrip, s); //potser hauria de ser &TheAgent? 
	pThread->SetFinishBool(&bThread);

	pThread->Pause(false);//never stop again.
	//pThread->ResumeThread();



}
/*
void ThreadedUpdate(){

}
*/

#endif





/*******************************************************************************************************************

			    T H E      M A I N       F U N C T I O N S


*******************************************************************************************************************/
#ifndef THREADED

extern EXPORT_API int Init(char* therulesfile){

	val=0;


	string rulefile=therulesfile;

	bool namedeclared=(rulefile.size()>0);
	if(namedeclared){
		val=initlog(therulesfile);
	}
	else{
		char *cs=new char[128];
		sprintf(cs,"I haven't found any name of the rules file, so I am using %s \n",DEFAULTRULEFILENAME);
		SEND2LOG(cs);
		val=initlog(therulesfile);
	}

	if(val==1){
		if(namedeclared)
			theAgent= new TAgent(therulesfile);	
		else
			theAgent= new TAgent();

		val=theAgent->init();

		}

	return val; //is 1 if the rulefile was found and the agent was well initialized
}




extern EXPORT_API int Update(){
	int a=0;
	if(val)
		a=theAgent->update();


	#ifdef VERBOSE
	if(a > 0){
	
	string say= "I am now doing: ";

	char * cs= new char[128];
	sprintf(cs,"%d",a);
	say += cs;

	say += "\n";
	SEND2LOG((char*) say.c_str());	
	}
	#endif


	return a;
}



extern EXPORT_API int UpdatePerception(int a, double value){
	val=theAgent->updatePerception( a, value);
	return val;
}
/*
extern EXPORT_API int UpdateEstimation(char* name, double value){
	// Done in the Net directly through the list of resources:

	val=theAgent->updateEstimation( name, value);
	return val;
}
*/

extern EXPORT_API int UpdateTramaResource(char* name, double value){ //this updates MaxResourceAvailable (for example, legs, or energy, as there are in the "external world")
	val=theAgent->updateResource( name, value);
	return val;

}





//! adds a delay to tmin or to tmax
/*
extern EXPORT_API int SetDelay(char* actionName, double time,bool inTmin){
	int val=0;
	if(inTmin)
		val=theAgent->getNet()->GetActionResource(actionName)->SetTmin(time);
	else
		val=theNet->getNet()->GetActionResource(actionName)->SetTmax(time);
	return val;
}
*/
extern EXPORT_API int AddDelay(char* actionName, double time){
	//int val=0;
	string a=actionName;
	TActionResource* ar=NULL;
	ar=theAgent->getNet()->GetActionResource(a);
	if(ar != NULL){
		ar->AddDelay(time);
		val=1;
	}
	return val;
}


extern EXPORT_API int SetTimeLength(char* actionName, double time){ //note this is not duration!!
	//int val=0;
	string a=actionName;
	TActionResource* ar=NULL;
	ar=theAgent->getNet()->GetActionResource(a);
	if(ar != NULL){
		double tmin=ar->getTmin();
		ar->SetTmax( tmin+time);
		val=1;

	}
	return val;
}

#else //THREADED is defined



extern EXPORT_API int Init(char* therulesfile){

	val=0;

	string rulefile=therulesfile;

	bool namedeclared=(rulefile.size()>0);
	if(namedeclared){
		val=initlog(therulesfile);
	}
	else{
		char *cs=new char[128];
		sprintf(cs,"I haven't found any name of the rules file, so I am using %s \n",DEFAULTRULEFILENAME);
		rulefile=DEFAULTRULEFILENAME;
		SEND2LOG(cs);
		val=initlog(therulesfile);
	}

	if((val==1)){
		ThreadedInit((char*) rulefile.c_str());
		}

	return val; //is 1 if the rulefile was found and the agent was well initialized
}

extern EXPORT_API int Update(){
	//ThreadedUpdate(); //done in onInit, no? that's the point.
//	int a=getNewAction();

	int a=pThread->getNewAction();
	if (a != 0){
		string say= "I am now doing: ";
		char * cs= new char[128];
		sprintf(cs,"%d",a);
		say += cs;
		say += "\n";
		SEND2LOG((char*) say.c_str());
	}
	return a;
}



extern EXPORT_API int UpdatePerception(int a, double value){
	val=pThread->updatePerception( a, value);
	return val;
}


extern EXPORT_API int UpdateTramaResource(char* name, double value){ //this updates MaxResourceAvailable (for example, legs, or energy, as there are in the "external world")
	val=pThread->updateResource( name, value);
	return val;

}


//! adds a delay to tmin or to tmax
extern EXPORT_API int AddDelay(char* actionName, double time){
	//int val=0;
	int val=pThread->addDelay( actionName, time);
	
	return val;
}

extern EXPORT_API int SetTimeLength(char* actionName, double time){ //note this is not duration!!
	//int val=0;
	return pThread->SetTimeLength(actionName, time);
}


#endif





extern EXPORT_API void Close(){

closelog();
}



