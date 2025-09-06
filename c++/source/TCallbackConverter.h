#pragma once

#include "Timepath_header.h"


#ifdef UNITYAGENT


/*!

\brief A class to convert the callbacks to C or C# functions

\details 

A class to convert the callbacks to C or C# functions

Useful to integrate the calls to C# functions into the C++ classes of this plugin.

*/

class TCallbackConverter : TObject
{


	int callCounter = 0;
	int agentID;
	int perceptionID;
	int actionID;
	UnityPerception pptr = nullptr; //! a pointer to a perception function, C-style, making a call within Unity (or other external one)
	UnityAction aptr = nullptr;


	PCallBackPerception ipptr = nullptr;  //! a pointer to an internal perception method, C++ style, making a call within a C++ Object
	PCallBackAction iaptr = nullptr;  //! a pointer to an internal action method, C++ style, making a call within a C++ Object
	TObject* obj = nullptr;   //! the object containing the perception method, or the action method, in C++ style


public:
	TCallbackConverter(void);
	~TCallbackConverter(void);


	//! \todo RECHECK if the use of callCounter cnanot simply be replaced my calling TDorer2Network::PerformCycle with a number of Cycles of 1.

	void resetCallCounter(){
		callCounter = 0;

	}
	int getCallCount(){
		return callCounter;
	}

	
	
	void setPerceptionCallback(int mindID,int perceptionID, UnityPerception pptr2){
		agentID = mindID;
		this->perceptionID = perceptionID;
		pptr = pptr2;
	}

	//! If the value to be considered is from an internal variable
	void setInternalPerceptionCallback(TObject* obj2, PCallBackPerception toptr2){
		ipptr = toptr2;
		obj = obj2;
	}

	double perceptionCallback(void){
		
		double d = 0.12345;
		
#if VERBOSE==2
		stringstream s;
		s << "the callback address is : " << pptr  <<"the agentID: " << this->agentID;

		SEND2LOG(s.str().c_str());
#endif		
		

		if (pptr != nullptr)
			d=this->pptr(this->agentID,this->perceptionID); 
		else if ((obj != nullptr) && (ipptr != nullptr))
			return(obj->*ipptr)();
		
		callCounter++;
#if VERBOSE==2
		stringstream s2;
		s2 << "the callback value  is : " << d << "the agentID: " << this->agentID;

		SEND2LOG(s2.str().c_str());
#endif		


		return d;
	}


	void setActionCallback(int mindID, int actionID, UnityAction aptr2){
		agentID = mindID;
		this->actionID = actionID;
		aptr = aptr2;
	}

	void setInternalActionCallback(TObject* obj2, PCallBackAction aptr2){
		iaptr = aptr2;
		obj = obj2;
	}


	
//	void actionCallback( double d){ 
	void actionCallback(double d, double d2) {

		if (aptr != nullptr)
			this->aptr(this->agentID,this->actionID,d,d2);
		else if((obj != nullptr) && (iaptr != nullptr))
			(obj->*iaptr)(d);
	
		callCounter++;
	}

};



#endif

