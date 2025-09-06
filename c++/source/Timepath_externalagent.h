
#include "Timepath_header.h"
#include "TLlogicNetwork.h"
TLlogicNetwork* theNet;
time_t starttime;

int val;


/*******************************************************************************************************************

			    T H E      M A I N       F U N C T I O N S


*******************************************************************************************************************/



//! it performs a complete perception / activation spreading / decision 
 EXPORT_API void Update(double now){
	if(val){ //initialization well done
		theNet->updateStory(now); //only if STORIES is defined
		theNet->PerformCycle(theNet->GetNetworkParams(),HORIZON_SPREADING_CYCLES); 
	}
}


//! adds a delay to tmin or to tmax
 EXPORT_API int SetDelay(char* actionName, double time, bool inTmin){
	int val = 0;
	if (inTmin)
		val = theNet->GetActionResource(actionName)->SetTmin(time);
	else
		val = theNet->GetActionResource(actionName)->SetTmax(time);
	return val;
}
//-------------------------------------------------------------------------------------------------------------------


//! Creates a new network
 EXPORT_API int Init(char* therulesfile){

	val = 0;

	string rulefile = therulesfile;

	bool namedeclared = (rulefile.size() > 0);
	if (namedeclared){
		val = initlog(therulesfile);
		if (!val){
			char* cs = new char[128];
			int i = sprintf(cs, "I haven't found the file with the name you gave me, this will generate an error \n");
			SEND2LOG(cs);
			val = initlog(therulesfile);
		}
	}
	else{
		char* cs = new char[128];
		int i = sprintf(cs, "I haven't found any name of the rules file, so I am using %s \n", DEFAULTRULEFILENAME);
		SEND2LOG(cs);
		val = initlog(therulesfile);
	}

	if (val == 1){
		//if(namedeclared)


#ifndef STORIES
		externalNet = new TDorer2Network(therulesfile);//the constructor already initializes the network
#else
		theNet = new TLlogicNetwork(therulesfile);

		//TIME initialisation.
		time(&starttime);
#endif
	}

	return val; //is 1 if the rulefile was found and the network was well initialized
}


//! it connects all the elements in the network and asumes everything is ready so the events involved in the story (if there are) can start to occur

extern EXPORT_API void Start(double now){

	theNet->ResetNetwork();
	theNet->startStory(now);


}



//! Adds an action to the network
extern EXPORT_API int AddAction(char* name, TObject* pObj, PCallBackAction pFn){

	string n = name;
	int val = 0;
	theNet->AddAction(n, pObj, pFn);
	val = 1;
	return val;
}

//! give an action and also function to estimate how much a certain action was already executed by some other agent
extern EXPORT_API void AddActionPerception(char* actionName, TObject* pObj, PCallBackAction percepCall, PCallBackPerception actCall){
	theNet->AddActionPerception(actionName, theNet, percepCall, actCall);
}

//! give an action and also function to estimate how much a certain action was already executed by some other agent, as well as the role needed to do it
extern EXPORT_API void AddActionPerceptionRole(char* actionName, TObject* pObj, PCallBackAction percepCall, PCallBackPerception actCall, char* role){
	theNet->AddActionPerceptionRole(actionName, theNet, percepCall, actCall,(string) role);
}


//! Adds a perception to the network
extern EXPORT_API int AddPerception(char* name, TObject* pObj, PCallBackPerception pFn){

	string n = name;
	int val = 0;
	theNet->AddPerception(n, pObj, pFn);
	val = 1;
	return val;
}


//! Adds a resource
extern EXPORT_API int AddResource(char* name, TObject* pObj, PCallBackPerception pFn, int iAmount){

	string n = name;
	int val = 0;
	theNet->AddResource(n, (TObject*)pObj, (PCallBackPerception)pFn, (int)iAmount);
	val = 1;
	return val;
}


//-------------------------------------------------------------------------------------------------------------------

