
#include "Timepath_header.h"
#include "TAction.h"
#include "TResource.h"
#include "TResourceProposition.h"
#include <time.h>


#if STORIES
#pragma once
#include "tobject.h"

class TAction;
class TLlogicNetwork;
class TLlogicCompetence;


/*!

 \brief The class where the actions forming a story are stored.
 
\details Useful to manage the causation spreading among the actions forming one or more stories.
 
\todo remove the possibility of having a negated condition as part of the context, as it is not used nor needed.
*/
class TActionResource : 	public TObject
{
public:

	TActionResource(string str); 

	~TActionResource(void);

	void connect(TResource* res, TAction* actionL, TWorldState* prop);
	
	void Add2Cause(TActionResource* pActRes); 
	void Add2Context(TActionResource* pActRes,bool isNegated); 


	void TActionResource::AddEstimator(TResource* e);

	void TActionResource::AddRole(TResource* e);

	
	void assumeAsGranted(double now);

	//! Updates the interval in which the action can be triggered (note if it delays tmin by x seconds, it does the same with tmax)
	int SetTmin(double time){
		double increase=time-tmin;
			tmin=time;
			tmax=tmax+increase;
			return 1;
	};

	//! Updates the ending instant of the interval (note if the time is smaller than tmin it will not do anything and return a 0)
	int SetTmax(double time){
		if(tmin < time){
			tmax=time;
			return 1;
		}
		else
			return 0;
	};


	//! Assumes the action as having occurred, this is, as part of the past
	void isPast();


	bool Update(double now);


	bool Check4Execution(); 

//	void assumeEstimation(double estimation);

	//!returns the name of the ActionResource
	string getName(){return name;}; 



	//! check the satisfaction of its context/precondition
	double getPos(){return pos;} 

	//! check the satisfaction of its cause/resource needed. \todo ?check execution of it ?
	double getNec(){return nec;	}

	/*
#ifdef UNITYAGENT //we also define static equivalents, which will be used as c callbacks:
	//! check the satisfaction of its context/precondition
	static double getStaticPos(TActionResource* tao){ return tao->pos; }

	//! check the satisfaction of its cause/resource needed. \todo ?check execution of it ?
	static double getStaticNec(TActionResource* tao){ return tao->nec; }
#endif
	*/



	//!returns the WorldState associated with the TActionResource
	TWorldState* getState(){return m_pState;}; 

	//!returns the Action associated with the TActionResource
	TAction* getAction(){return m_pAction;}; 

	//!returns the resource associated with the TActionResource
	TResource* getResource(){return m_pResource;}; 


	//!Negates the set of causal conditions. Useful to process NOTP.
	void negateCause(bool b=TRUE){causeNeg=b;}; 

	//! Add a delay to both tmin and tmax
	void AddDelay(double time) 
	{
		tmin=tmin+time; 
		tmax=tmax+time;};

	//! Get the triggering time. If it is -1 it has not been triggered
	double getTTime(){	return tt; }

	//! Get Tmin
	double getTmin(){return tmin;}


//! Set the triggering time of the action at some particular moment
	void setTTime() {tt=lastnow;};

	
	//! Set the triggering time of the action at some particular moment. Only used in debugging cases, when the time has to be given explicitly
	void setTTime(double time) {
		lastnow=time;
		tt=lastnow;};

	//! Set the fact of being an action with a cause external to the story
	void setIsExternal(bool b=TRUE) {IsInternal=!b;};

	//! Is the action external? this function tells whether an action has a cause external to the story
	bool getIsExternal(){return !IsInternal;};
		bool IsDoneBySomeOneElse(){return DoneBySomeoneElse;};

	

private:
	//! Updates the measure of what is present time, expecting them to be in floating point seconds
	void setLastNow(double now){this->lastnow=now;};

	bool updatePos(double now); 
//	double updateNec(double now);
	bool updateNec(double now);

	bool updateTau_u(double val);//!used in updateNec
//	bool checkEstimation(TLlogicCompetence* mod);
//	bool checkEstimation();



	string name;
	TAction* m_pAction; //4 when the action is to be executed
	TResource* m_pResource; //4 when it becomes the cause of something
	TWorldState* m_pState; //4 when it is the context of something, to THEN be embedded into a proposition and then a competence module precondition

	TResource* estimator; //4 when we are checking it was not done by someone else. It will revert in m_pState if so. (see TLlogicNetwork::updateActionInNetwork)
	
	TResource* role; //4 when checking if it can assume the role needed to perform the action


	bool HasRole;  //used in AddRole and in updateNec (could be just checking if it the pointer is null or not)
	bool HasEstimator; //used in AddEstimator and in updateNec (could be just checking if it the pointer is null or not)
	bool IsInternal; //used in parseStory and in updateNec
	bool DoneBySomeoneElse;//used in updateNec (Substitution principle)
	
	
	double pos;
	double nec;
	
//	double internalNec; //only used when an action is internal and has an external estimator, t

	double tau_u; //the amount given to the module will also be useful when considering if someone else has performed the action

	double tt; //trigger time in (floating point) seconds from start
	double tmin;
	double tmax;

	list<TActionResource*> context;
	list<TActionResource*> cause;

	list<bool> contextNeg;
	bool causeNeg; //for NOTP: NOT(a1 v a2 v a3)

	double lastnow;



};


#endif