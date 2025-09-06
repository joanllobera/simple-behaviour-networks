
#include "Timepath_header.h"
#include "TMind.h"

#include "TActionResource.h"
#include "TCallbackConverter.h"
#include "TNetworkParams.h"

#include<sys/timeb.h>
double getMilliCount(){ //adapted from http://www.cplusplus.com/forum/general/43203/
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;

	double nCountSecs = (double)nCount;
	nCountSecs /= 1000;
	return nCountSecs;
}

double starttime = NULL;

double getTimePassed(){
	double nSpan = getMilliCount() - starttime;
	if (nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}

//---------------------------------------------------------
vector<TMind *>	minds;

#if EXPORTTESTFUNCTIONS

EXPORT_API void testAddName(char* str){

	string temp = " in testAddName: ";
	temp = temp + str;

	//! \todo correct this, check if it is a problem
	//SEND2LOG( temp.c_str());//this seems to make it crash at 1-February-2014 (even if before it did not)
}

EXPORT_API void testLog2Unity(logCallback pFn){

	pFn("This string is created in a dll, writing to console through a direct callback. does this appear in unity console?");
	pLogUnity("This string is created in a dll, writing to console through a STORED callback. , does this appear in unity console?");
}





//! it tests whether the callback works 
EXPORT_API double testPerception(int mindID, int perceptionID, UnityPerception pFn){

	TCallbackConverter* pObj = new TCallbackConverter();
	pObj->setPerceptionCallback(mindID,perceptionID, pFn);

	return pObj->perceptionCallback();


}


//! it tests whether the perceptions exist within the net
EXPORT_API double testPerceptionInNet(int mindID, char* name){

	TWorldState* p = minds[mindID]->getNet()->GetPerception(name);
	if (p == NULL)
		return -1.0;
	else
		return p->GetTruthValue();

}

//! it tests whether the action callback works
EXPORT_API void testAction(int mindID, int actionID, UnityAction pFn, double activation){

	TCallbackConverter* pObj = new TCallbackConverter();
	pObj->setActionCallback(mindID, actionID, pFn);

	pObj->actionCallback(activation);

}


//! it tests whether the action exists within the net, and performs it
EXPORT_API int testActionInNet(int mindID, char* name, double activation){
	

	
	TAction* a = minds[mindID]->getNet()->GetAction(name);
	if (a == NULL)
		return -1;
	else{
		a->PerformAction(activation);
		return 1;
	}

}
#endif

/*******************************************************************************************************************
T H E      M A I N       F U N C T I O N S
*******************************************************************************************************************/


//! Sets up the log function in order it registers information within unity
 EXPORT_API void InitLog(logCallback pFn){

	setUnityLogCallback(pFn);

}

//! creates a Mind and returns an int which is an ID neeeded for any ulterior communication with the .dll 
 EXPORT_API int CreateNet(char * id){

	TMind* mind = new TMind(id);

	minds.push_back(mind);

	int where = static_cast<int>(minds.size()) - 1;
	mind->seTMindID(where);

	return where;
}






//! Adds a perception to the network
 EXPORT_API int AddPerception(int mindID, int perceptionID, char* name, UnityPerception pFn){

	string n = name;
	int val = 0;

	TCallbackConverter* pObj = new TCallbackConverter();
	pObj->setPerceptionCallback(mindID,perceptionID, pFn);


	minds[mindID]->getNet()->AddPerception(n, pObj);
	val = 1;
	return val;
}





//! Adds an action to the network
EXPORT_API int AddAction(int mindID, int actionID, char* name, UnityAction pFn){
	string n = name;
	int val = 0;

	TCallbackConverter* pObj = new TCallbackConverter();

	pObj->setActionCallback(mindID,actionID, pFn);

	minds[mindID]->getNet()->AddAction(name, pObj);
	

	val = 1;
	return val;
}

//! Adds a resource
EXPORT_API int AddResource(int mindID, int resourceID, char* name, UnityPerception pFn, double iAmount){
//extern EXPORT_API int AddResource(int mindID, int resourceID, char* name, double iAmount){
	string n = name;
	int val = 0;

	TCallbackConverter* pObj = new TCallbackConverter();
	pObj->setPerceptionCallback(mindID,resourceID, pFn);

	minds[mindID]->getNet()->AddResource(n, pObj, iAmount);
	val = 1;
	return val;
}

//! Updates the amount available a resource
//NO, we are doing this inside the PErformCycle.
/*
extern EXPORT_API int UpdateResourceAvailable(int mindID, char* name, double newMaxAvailable){

	string n = name;

	minds[mindID]->getNet()->GetResource(n)->UpdateMaxAvailable( newMaxAvailable);
	return 1;
}
*/
EXPORT_API double GetResourceNotUsed(int mindID, char* name){

	string n = name;
	return minds[mindID]->getNet()->GetResource(n)->GetAmountAvailable();
}


#if STORIES
//! give an action and also function to estimate how much a certain action was already executed by some other agent 
EXPORT_API void AddActionPerception(int mindID, int perceptionID, int actionID, char* actionName, UnityPerception percepCall, UnityAction actCall){

	TCallbackConverter* pObj = new TCallbackConverter();
	pObj->setActionCallback(mindID,actionID, actCall);
	pObj->setPerceptionCallback(mindID,perceptionID, percepCall);

	minds[mindID]->getNet()->AddActionPerception(actionName, pObj);

}

//! give an action and also function to estimate how much a certain action was already executed by some other agent, as well as the role needed to do it
EXPORT_API void AddActionPerceptionRole(int mindID, int perceptionID, int actionID, char* actionName, UnityPerception percepCall, UnityAction actCall, char* role){
	TCallbackConverter* pObj = new TCallbackConverter();
	pObj->setActionCallback(mindID, actionID, actCall);
	pObj->setPerceptionCallback(mindID, perceptionID, percepCall);


	minds[mindID]->getNet()->AddActionPerceptionRole(actionName, pObj, (string)role);
}

#endif






//! it performs a complete perception / activation spreading / decision 
EXPORT_API void UpdateNet(int mindID){

	TLlogicNetwork* theNet = minds[mindID]->getNet();
#if STORIES
		double now = getTimePassed();
		theNet->updateStories(now); 
#endif
		theNet->PerformCycle(theNet->GetNetworkParams(), HORIZON_COMPLETE_CYCLES);
	
}



//! it connects all the elements in the mind (an extended behavior network), as well as the story or stories if they are defined
EXPORT_API void ConnectNet(int mindID,char* rules){

	string therules = rules;

	minds[mindID]->getNet()->ResetNetwork(therules);

#if VERBOSE
	std::stringstream logString;
	logString << "Mind " << mindID << " connected";
	SEND2LOG(logString.str().c_str());
#endif
}


EXPORT_API void ClearNets() {
	minds.clear();
}


#if STORIES


//! sets a delay to tmin 
EXPORT_API int SetMinDelay(int mindID, char* actionName, double time) {
	int val = 0;

	val = minds[mindID]->getNet()->GetActionResource(actionName)->SetTmin(time);
	return val;
}

//! sets a delay to tmax
EXPORT_API int SetMaxDelay(int mindID, char* actionName, double time) {
	int val = 0;

	val = minds[mindID]->getNet()->GetActionResource(actionName)->SetTmax(time);
	return val;
}


//! it creates a new story within an existing (and connected) mind
EXPORT_API void CreateStory(int mindID, char* plot){
	string theplot = plot;

	//!todo: use the resetNetwork with the passing of rules
	minds[mindID]->getNet()->CreateStory(theplot);

	if (starttime == NULL){
		//TIME initialisation
		starttime = getMilliCount();
	}



}




//! it starts all the stories declared
EXPORT_API void StartStories(){

	vector<TMind*>::iterator	pos;
	double timepassed = getTimePassed();

	//! \todo these elements can be threaded
	for (pos = minds.begin(); pos != minds.end(); ++pos){
		(*pos)->getNet()->startStories(timepassed);
			}
	


}

#endif
