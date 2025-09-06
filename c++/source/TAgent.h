

#pragma once
#include "Timepath_header.h"

#ifdef INTERNALAGENT
#include <time.h>

class TDorer2Network;

#if STORIES
	class TLlogicNetwork;
#endif
/*
#ifdef THREADED
#include "./Thread_classes/threaddesc.h"
#endif
*/

/*!

 \brief an agent that uses the behavior network and can be called externally
 \date 08-2010
 \details it contains some actions and some perceptions, and deals with the rule parsing (as the file is found in the desktop)
		  

		  how to use it: define a table of perceptions like PTHEHOUSEISRED 3, with a function like: double thehouseisred();
						when wanting to define the perception simply do: UpdatePerception(PTHEHOUSEISRED,thehouseisred);



\todo find a way to systematize the action callbacks, to not state the actions explicitly, so the agent doesn't have to be redefined each time
\todo BE CAREFUL the way communication is done with the outside ONLY ALLOWS 1 ACTION AT A TIME!!!if more are decided, the last will override the previous (see how whattodo is changed)
\todo clarify how to manage sustained actions automatically! add automatically a module to deal with this?¿

\todo CLARIFY all the actions involved in competence modules independent of story have to be declared BEFORE the ones involved in the story. FIND OUT WHY AND CORRECT



*/

class TAgent:TObject
{



// ACTIONS, RESOURCES AND PERCEPTIONS:...................................
#define P_IFEXPLORING 0001
#define P_BENEAR 0002

#define P_BDOES_NOT_GO 0003 //PA: B_DOES_NOT_GO
#define P_MIKE_JUMPS 0004 //PA: MIKE_JUMPS 

#define P_THEY_ARE_STILL_FAR 0005

	//#define P_BROKEN_BRIDGE 0006 	//perception 6 is they BOTH arrive to the broken bridge, as it presuposes some degree of ATP from the human, it is a PA not possible to solve by the system
//#define P_THEY_CONTINUE_TOWARDS_THE_BRIDGE 0006
#define P_THEY_ARRIVE_TO_A_BROKEN_BRIDGE 007
#define P_THEY_GO_TOWARDS_THE_BRIDGE 008 //this is: perceive they start going towards... to provoke a "sustained action"

#define P_HIS_FRIEND_JUMPS 0009 //PA: HIS:FRIEND:JUMPS

#define	P_HIS_FRIEND_FALLS 0010	//PA: his:friend_falls

#define P_HIS_FRIEND_DIES 0011 //PA 


#define P_MIKE_REACHES_THE_OTHER_SIDE 0012 //PA


	//------------------------------------ROLE RESOURCES (not used by now)
#define R_I_AM_MIKE 0001
#define R_I_AM_HIS_FRIEND 0002

//------------------------------------

#define APPROACH 0001
#define MIKE_AND_HIS_FRIEND_ARE_EXPLORING 0002
#define TIES_ARRIVE_AND_START_BOMBING 0003
#define COMMUNICATION_WITH_MOTHERSHIP 0004
#define THEY_GO_TOWARDS_THE_BRIDGE 005


#define A_WAVES 0006

#define BDOES_NOT_GO 0007 //PA: B_DOES_NOT_GO

#define THEY_ARRIVE_TO_A_BROKEN_BRIDGE 0008

#define THEY_CONTINUE_TOWARDS_THE_BRIDGE 0009


#define HIS_FRIEND_JUMPS 0010
#define HIS_FRIEND_FALLS 0011
#define HIS_FRIEND_DIES 0012
#define MOTHERSHIP_ORDERS_TO_JUMP 0013

#define MIKE_JUMPS 0014 //PA: MIKE_JUMPS 
#define MIKE_REACHES_THE_OTHER_SIDE 0015

#define MOTHERSHIP_SAVES_MIKE 0016
#define BOMB_REACHES_HIS_FRIEND 0017



public:
	TAgent(void);
	TAgent(char *cs);
	TAgent(char *frules, char *fstories);
	~TAgent(void);
	TAgent &operator=(const TAgent &a);

	
	int init();



#ifndef STORIES
	TDorer2Network* getNet(){return theNet;};
#else
	TLlogicNetwork* getNet(){return theNet;};
#endif



	double perception1(){ return perceptions[1];};
	double perception2(){ return perceptions[2];}
	double perception3(){ return perceptions[3];};
	double perception4(){ return perceptions[4];};
	double perception5(){ return perceptions[5];};
	double perception6(){ return perceptions[6];};
	double perception7(){ return perceptions[7];};
	double perception8(){ return perceptions[8];};
	double perception9(){ return perceptions[9];};
	double perception10(){ return perceptions[10];};
	double perception11(){ return perceptions[11];};
	double perception12(){ return perceptions[12];};
	double perception13(){ return perceptions[13];};
	double perception14(){ return perceptions[14];};


	void action0(double a){	} ;
	void action1(double a){	whattodo = 1;} ;
	void action2(double a){	whattodo = 2;} ;
	void action3(double a){	whattodo = 3;} ;
	void action4(double a){	whattodo = 4;} ;
	void action5(double a){	
		whattodo = 5;
	//	string astr="TEST TEST TEST ACTION 5 TEST";
	//	SEND2LOG(astr.c_str());
	} ;
	void action6(double a){	whattodo = 6;} ;
	void action7(double a){	whattodo = 7;} ;
	void action8(double a){	whattodo = 8;} ;
	void action9(double a){	whattodo = 9;} ;
	void action10(double a){	whattodo = 10;} ;
	void action11(double a){	whattodo = 11;} ;
	void action12(double a){	whattodo = 12;} ;
	void action13(double a){	whattodo = 13;} ;
	void action14(double a){	whattodo = 14;} ;
	void action15(double a){	whattodo = 15;} ;
	void action16(double a){	whattodo = 16;} ;
	void action17(double a){	whattodo = 17;} ;


	//char* update();
	int update();

	int updatePerception(int a, double value);
	int updateResource(string name, double value);

	///// JL07.10.2010 NOT NEEDED?!? it works with update perception!!
	//	int updateEstimation(string name, double value);

protected:
	
	
	void CreatePerceptions();
	void CreateResources();
	void CreateActions();


#ifndef STORIES
	TDorer2Network* theNet;
#else
	TLlogicNetwork* theNet;
#endif

	char* rulefilename;

//	string whattodo; //a simple way of sending the actions decided.
public:
	int whattodo; //a simple way of sending the action decided (and only 1)
protected:
	
	vector<double> perceptions; //a simple way of storing perceptions that will be updated externally

	/*
	double p1;
double p2;
double p3;
*/

	clock_t startclock;
/*
#ifdef THREADED
	bool bThread;
	threadDesc* pThread;

#endif
*/

};


#endif //#ifdef INTERNALAGENT


