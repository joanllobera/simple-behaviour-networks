#include "TBehaviorNetwork.h"
#include "TDorer2Network.h"

#if STORIES
	#include "TLlogicNetwork.h"
#endif


#include "TAgent.h"


#ifdef INTERNALAGENT


TAgent::TAgent(void)
{
#ifndef STORIES
	theNet = new TDorer2Network();
#else 
	theNet = new TLlogicNetwork();
#endif

}

TAgent::TAgent(char *cs){
	TAgent();
	rulefilename=cs;


}


TAgent::~TAgent(void)
{
 delete theNet;
}


#ifdef DEBUGWITHTIMESTEPS

double now4debug=2.3;

#endif


int TAgent::init(){
//initialize the behavior network with the rules and connect everything together.

#ifndef STORIES
	theNet = new TDorer2Network(rulefilename);//the constructor already initializes the network
#else
	theNet = new TLlogicNetwork(rulefilename);

#endif

	for(int i=0; i < MAXPERCEPTIONS;i++){
		perceptions.push_back(0.0);

	}

//	whattodo="DO_NOTHING";
	whattodo=0;

	CreatePerceptions();

	CreateResources();

	CreateActions();

	theNet->ResetNetwork();


#if STORIES
	startclock=clock();
	double now=(double) startclock/CLOCKS_PER_SEC;

#ifdef DEBUGWITHTIMESTEPS
	 now=now4debug;
#endif

	theNet->startStory(now);
#endif

	/*
#ifdef THREADED
	bThread=FALSE;
	 new ThreadManager();
	pThread=(threadDesc2*)ThreadManager::pGet()->pCreate();
	pThread->Pause(true);
  	pThread->ResumeThread();
	
#endif
*/

	return 1;

}
	




string printdouble(double intValue){
 char *myBuff;

  // Create a new char array
  myBuff = new char[100];


  // Convert to string
  int i=sprintf(myBuff, "%f", intValue);

  string a=myBuff;

    // Delete the buffer
  delete[] myBuff;

  return a;

}
//char* TAgent::update(){
int TAgent::update(){

#if STORIES
	clock_t nowclock=clock();
	double now=(double)   (nowclock-startclock)/CLOCKS_PER_SEC;
#ifdef DEBUGWITHTIMESTEPS
	now4debug=now4debug+0.5;
	now=now4debug;
#endif
#endif


//	whattodo=""; //in case something was decided in the previous cycle
	whattodo=0;

	theNet->updateStory(now); //only if Timepath
	//	bool goalreached=((TDorer2Network*) theNet)->PerformCycle(theNet->GetNetworkParams(),1); 
	theNet->PerformCycle(theNet->GetNetworkParams(),HORIZON_SPREADING_CYCLES); 



//	char *cs=(char*) whattodo.c_str();
//	return cs;


	int a=whattodo;
	return a;

}



void TAgent::CreatePerceptions(){
	
	theNet->AddPerception("P_IFEXPLORING", this, (PCallBackPerception) &TAgent::perception1);
	theNet->AddPerception("P_BENEAR", this, (PCallBackPerception) &TAgent::perception2);
	theNet->AddPerception("P_BDOES_NOT_GO", this, (PCallBackPerception) &TAgent::perception3);
	theNet->AddPerception("P_MIKE_JUMPS", this, (PCallBackPerception) &TAgent::perception4);
	theNet->AddPerception("P_THEY_ARE_STILL_FAR", this, (PCallBackPerception) &TAgent::perception5);
//	theNet->AddPerception("P_THEY_CONTINUE_TOWARDS_THE_BRIDGE", this, (PCallBackPerception) &TAgent::perception6);
	theNet->AddPerception("P_THEY_ARRIVE_TO_A_BROKEN_BRIDGE", this, (PCallBackPerception) &TAgent::perception7);
	theNet->AddPerception("P_THEY_GO_TOWARDS_THE_BRIDGE", this, (PCallBackPerception) &TAgent::perception8);

}

void TAgent::CreateResources(){

	//NO RESOURCES BY NOW
//	theNet->AddResource("box",this, (PCallBackPerception) &TAgent::getBoxAvailable,  5.0);
}


//! \todo find out what is going on, why the actions of the competence modules have to be declared BEFORE the other ones (see action10)
void TAgent::CreateActions(){
	theNet->AddAction("NOTHING", this, (PCallBackAction) &TAgent::action0);
	theNet->AddAction("APPROACH", this, (PCallBackAction) &TAgent::action1 );
	theNet->AddAction("MIKE_AND_HIS_FRIEND_ARE_EXPLORING", this, (PCallBackAction) &TAgent::action2);

//theNet->AddActionPerception("MIKE_AND_HIS_FRIEND_ARE_EXPLORING", this, (PCallBackAction) &TAgent::action2, (PCallBackPerception) &TAgent::perception1);
//MISTAKE: it is not the same to perceive and event done by someone else that to trigger it and have someone else perceive it. The ActionPerception covers the 1st case.

	theNet->AddAction("TIES_ARRIVE_AND_START_BOMBING", this, (PCallBackAction) &TAgent::action3);
	theNet->AddAction("COMMUNICATION_WITH_MOTHERSHIP", this, (PCallBackAction) &TAgent::action4);
	//theNet->AddAction("THEY_GO_TOWARDS_THE_BRIDGE", this, (PCallBackAction) &TAgent::action5);
	theNet->AddActionPerception("THEY_GO_TOWARDS_THE_BRIDGE", this, (PCallBackAction) &TAgent::action5, (PCallBackPerception) &TAgent::perception8); 
		theNet->AddAction("A_WAVES", this, (PCallBackAction) &TAgent::action6);
	theNet->AddActionPerception("BDOES_NOT_GO",this,(PCallBackAction) &TAgent::action0, (PCallBackPerception) &TAgent::perception5); //action7 is nothing...
	theNet->AddActionPerception("THEY_ARRIVE_TO_A_BROKEN_BRIDGE",this,(PCallBackAction) &TAgent::action8, (PCallBackPerception) &TAgent::perception7); 
	theNet->AddAction("THEY_CONTINUE_TOWARDS_THE_BRIDGE",this,(PCallBackAction) &TAgent::action9); 

	theNet->AddActionPerceptionRole("HIS_FRIEND_JUMPS",this,(PCallBackAction) &TAgent::action10,  (PCallBackPerception) &TAgent::perception9, "HIS_FRIEND"); 	
	theNet->AddActionPerception("HIS_FRIEND_FALLS",this,(PCallBackAction) &TAgent::action11,  (PCallBackPerception) &TAgent::perception10); 	
	theNet->AddActionPerception("HIS_FRIEND_DIES",this,(PCallBackAction) &TAgent::action12,  (PCallBackPerception) &TAgent::perception11); 	
	theNet->AddAction("MOTHERSHIP_ORDERS_TO_JUMP",this,(PCallBackAction) &TAgent::action13); 	
	theNet->AddActionPerceptionRole("MIKE_JUMPS",this,(PCallBackAction) &TAgent::action14,  (PCallBackPerception) &TAgent::perception4, "MIKE"); 	
	//theNet->AddActionPerceptionRole("MIKE_REACHES_THE_OTHER_SIDE",this,(PCallBackAction) &TAgent::action15,  (PCallBackPerception) &TAgent::perception12, "MIKE"); 		
	theNet->AddActionPerception("MIKE_REACHES_THE_OTHER_SIDE",this,(PCallBackAction) &TAgent::action15,  (PCallBackPerception) &TAgent::perception12); //nope MIKE because 	it triggers also the anim of the other participant
	theNet->AddAction("MOTHERSHIP_SAVES_MIKE",this,(PCallBackAction) &TAgent::action16); 	
	theNet->AddAction("BOMB_REACHES_HIS_FRIEND",this,(PCallBackAction) &TAgent::action17); 

}



int TAgent::updatePerception(int a, double value){
/*
	switch(a){

		case 1:	p1=value;
			break;
		case 2: p2=value;
			break;
		case 3: p3=value;
			break;
	}
*/

	if(a <MAXPERCEPTIONS){
		perceptions[a]=value;
		return 1;
	}else
		return 0;


}

int TAgent::updateResource(string name, double value){

	TResource* res=this->getNet()->GetResource(name);

	int val=res->UpdateMaxAvailable(value);
	//return val;
	return 1;
}

/*
/// JL07.10.2010 NOT NEEDED?!? it works with update perception!!

int TAgent::updateEstimation(string name, double value){

	string estimName=this->getNet()->makeEstimatorlabel(name); //incredibly, you really need to call the net...	
	this->updateResource(estimName,value);
	return 1;



}*/




#endif //#ifdef INTERNALAGENT
