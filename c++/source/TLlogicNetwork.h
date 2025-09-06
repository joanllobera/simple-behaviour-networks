#pragma once

#include "Timepath_header.h"


#include "networkheader.h"
#include "TDorer2Network.h"


class TActionResource;
class TCompetence;
class TCallbackConverter;


/*!

 \brief Extends the TDorer2Network to enable the processing of stories
 \date 02.2014
 \details Extends the TDorer2Network to enable the processing of stories
 
 \todo efficiency in accesses to elements could be improved if lists are moved to be vectors.
 
\todo (2014) the existence of callbacks has to be carefully controlled when using TCallbackConverter, and an exception or something similar should appear when not set correctly

 \sa  TDorer2Network TBehaviorNetwork
*/
class TLlogicNetwork : public TDorer2Network
{
public:
	TLlogicNetwork(void);
	TLlogicNetwork(char* pRuleFile);

	~TLlogicNetwork(void);

#if UNITYAGENT==0
	void	ResetNetwork(); 
#else
	void	ResetNetwork(string rules);
#endif


#if STORIES

	bool updateStories(double now);

	//! assumes the default context as granted, which provokes the start of activation in cascading events.
	void startStories(double now); 


	TActionResource* GetActionResource(string name);
	TActionResource* AddActionPerception(string name, TObject* pObj, PCallBackAction pFn, PCallBackPerception pCallback);
	void AddActionPerceptionRole(string name, TObject* pObj, PCallBackAction pFn, PCallBackPerception pCallback, string role);
#if UNITYAGENT
	TActionResource* AddActionPerception(string name, TCallbackConverter* pObj);
	void AddActionPerceptionRole(string name, TCallbackConverter* pObj, string role);


	//for debug purposes:
	TWorldState* GetPerception(string name){ return  m_pPercept->GetPerception(name); }
	list<TWorldState*> GetAllPerceptions(){return this->m_pPercept->GetAllPerceptions(); }
	TAction* GetAction(string name){ return  m_pAct->GetAction(name); }


#endif

#endif

#ifdef DEBUGWITHTIMESTEPS
	//! A function that will execute any action that has pos=1 and nec > 0.7 .To be used only for debugging purposes.
	string RunNecessaryToTrigger(double now);
#endif


protected:
	TActionResource* ConnectActionResource(string name);


	virtual TCompetence* CreateNewCompetence();
private:

#if STORIES

//-----------------------------------------------------------------------------------------------------------------------------
	//! \todo THESE ELEMENTS That COULD BE ENCAPSULATED INTO A STORY TYPE WITH A FACTORY, IN CASE SEVERAL STORIES ARE WANTED:

	static string makeATPlabel(string story);
	static string makeKlabel(string story);
	static string makeROLElabel(string name);
	static string makeNOTPlabel(int n);

	static string makeEstimatorlabel(string name);
	static string cleanstring(string rule);
	string printdoublehere(double intValue);

	int storyCount;

	//the "factory"
	list<TActionResource*>  pActRes;
	string storyLabel;

	//the methods of the factory (note they also add resources to m_pAct)
	bool initializeStory(string storyLabel); //creates the elements within the network, used by createStory
	TActionResource* AddActionResource(string name,string storyLabel);
public:
	bool storyEnded(){return( hasEnded );}; //function to see if a story has ended (goal story satisfied)	
private:	
	 double perceiveHasEnded(){
		if(hasEnded){ return 1.0;}
		else{return 0.0;}
		};

	//! to manage the null first action of a story
	void doNothing(double a){}; 
	
	//! does everything to finish a story,
	void endStory(double value);
	bool hasEnded;

protected:
	bool ParseRules(string rules);  

public:
	bool CreateStory(string story); 
#endif
};



