#include "TActionResource.h"

#if STORIES

#include "TLlogicNetwork.h"
#include "TLlogicCompetence.h"

#include "TProposition.h"
#include "TResource.h"


TActionResource::TActionResource(string str)
{
	this->name=str;
	//this->triggered=FALSE;
	this->tt=-1;
	this->tmin= DEFAULT_MINDELAY;
	this->tmax= DEFAULT_MAXDELAY;
	
	this->pos=0;
	this->nec=0;

	this->causeNeg=FALSE;

	m_pAction=NULL;
	m_pResource=NULL; 
	m_pState=NULL; 


	estimator=NULL;
	role=NULL;

	HasRole=FALSE;
	HasEstimator=FALSE;
	IsInternal=TRUE;
	DoneBySomeoneElse=FALSE;
	
} 


TActionResource::~TActionResource(void)
{
}


/*!

 \brief It connects the actionResource to the elements within the Behavioral Network
 
 \date 06.2010
\details It connects the actionResource to the elements within the Behavioral Network

 \param  res the TResource, actionL the proper behavior to execute, state the state of the world to be checked
 \sa TLlogicNetwork::connectActionResource

*/
void TActionResource::connect(TResource* res, TAction* actionL, TWorldState* state){
	m_pAction=actionL; 
	m_pResource=res; 
	m_pState=state;
}


//!Adds an element to the list of causes

void TActionResource::Add2Cause(TActionResource* pActRes){
		cause.push_back(pActRes);
}


//! Adds an element to the list of contextual conditions
void TActionResource::Add2Context(TActionResource* pActRes, bool isNegated){

	context.push_back(pActRes);
	contextNeg.push_back(isNegated);

}




/*!

 \brief it updates the possibility and necessity values of the ActionResource from the present time, its estimator, its cause and its context. 
 \return false if the context is NOT satisfied
 \date 06.2010
\details 
 tries to update the possibility and necessity values. 
			If it can update both or none of both, it returns TRUE
			If it can update the possibility but not the necessity, it returns FALSE

 \param  now the time in (floating point) seconds since the story started

*/

bool TActionResource::Update(double now){

	bool b=TRUE;
#ifdef ONLYONCE
	if(this->tt < 0){ //will be -1 if not triggered. THIS IF will make  a story action be triggered only once. OTherwise, updatePos will reset the value and it will be triggered while in the right interval
#endif
		b= this->updatePos(now);
	
		if(b){
			b=this->updateNec(now);
			}
		this->setLastNow(now);

#ifdef ONLYONCE	
}
#endif
	return b;


}




/*!

 \brief  it tells whether the action is considered to be executed by someone else or has to actually be executed in a TLlogicCompetence module.
 \return false if it should not ACTUALLY be triggered by the competence module
 \date 06.2010
\details 
  it tells whether the action is considered to be executed by someone else or has to actually be executed in a TLlogicCompetence module.
  It checks whether the action is external to the story, or if it is assumed by some other character.

  It also sends to the log file information about what is considered to be happening.
*/


bool TActionResource::Check4Execution(){
	bool reallydoit=TRUE;
	string info= "";

	if(this->IsDoneBySomeOneElse())
		reallydoit=false;
	if(this->getIsExternal())
		reallydoit=false;
	this->setTTime();



	//---------------------------get values for printing
	string name=this->getName();
	double p=this->getPos();
	double n=this->getNec();

	double e=0.0;
	if(this->HasEstimator)
		e=this->estimator->GetTruthValue();
	double d=this->getTTime();



	//----------------------------now, update everything:
	this->isPast();

	if( this->HasRole && (role->GetAmountAvailable() >= 1.0) ){ 
		this->role->ReduceAmountAvailable(1.0); //the reduction could be of less than 1 for complicated scenarios, but this is not implemented at the moment.
	}
	else{
	//for very complex scenarios you could "regenerate" the ability of assuming a particular role, but not currently implemented.
	}

	if(reallydoit){
		this->nec=1.0; 
		this->getResource()->UpdateMaxAvailable(nec);
	}

	//------------------------------print information
	info += "Action: ";
	info +=name;
	if(this->getIsExternal())
		info+=", estimated to have occurred at: ";
	else if(this->IsDoneBySomeOneElse())
		info+=", estimated to have been performed by someone else at: ";
	else
	info += ", triggered at: ";

	double intValue=d;
	char *myBuff;
	myBuff = new char[100];
	int i=sprintf(myBuff, "%f", intValue);
	string s=myBuff;
	info += s;
	info +=", \n";


#if VERBOSE
			
	info += "                 pos: ";
	intValue=p;
			
	i=sprintf(myBuff, "%f", intValue);
	s=myBuff;
	info= info +s;
				
	if(this->HasEstimator){
		if(this->IsDoneBySomeOneElse()){
			
		}
		else{
			info += ", nec: ";
			intValue=n;
				
			i=sprintf(myBuff, "%f", intValue);
			s=myBuff;
			info += s;
		}
		intValue=e;	
		i=sprintf(myBuff, "%f", intValue);
		s=myBuff;
		info +=", est ";
		info += s;
	}else{
		info += ", nec: ";
		intValue=n;
				
		i=sprintf(myBuff, "%f", intValue);
		s=myBuff;
		info += s;
	}
/*
		intValue=this->getNec();	
		i=sprintf(myBuff, "%f", intValue);
		s=myBuff;
		info +=", FINAL nec: ";
		info += s;

		intValue=this->getPos();	
		i=sprintf(myBuff, "%f", intValue);
		s=myBuff;
		info +=", pos: ";
		info += s;

		intValue=this->tau_u;	
		i=sprintf(myBuff, "%f", intValue);
		s=myBuff;
		info +=", FINAL Tau_U: ";
		info += s;
*/

		info += " ; \n";


#endif
	delete[] myBuff;
	SEND2LOG((char*) info.c_str());	

	
return(reallydoit);
}



/*!

 \brief it moves the action to its past region
 \return 
 \date 10.2010
\details updates the possibility  value, and also the the resources expected to be needed in order the action cannot occur anymore
 
 \todo 		check if the checking of the NOTP case should not be done in updatePos from the delays agreed.
 \param 

*/


void TActionResource::isPast()
{

	

		this->pos=0.0; 			

		//this->updateTau_u(MAX_TAU_U);
//		this->updateTau_u(MAX_TAU_U);

		if(this->getState()!=NULL){ //this prevents an actionPerception that is not in the story to provoke a crash
			this->getState()->GetTruthValue(); //to make it update the pos value in the worldstate	
		}else{

			char* cs=new char[128];
			sprintf(cs,"I didn't update the state of actionPerception %s to be in the past because it is not in a plot, and so is not a state to be perceived. You should correct this in the definition of your actions",this->getName().c_str());
			SEND2LOG(cs);
		}


		if(this->causeNeg){
			list<TActionResource*>::iterator i;
			for(i=cause.begin();i!=cause.end();i++){
				if((*i)->tt < 0 ){

					(*i)->setTTime(0.0); //to ensure it does not appear as not triggered
					(*i)->isPast();
				//! \todo  check if this should not be done in updatePos from the delays agreed.
				}
			}
		}
}




/*!

 \brief it updates the possibility value of the ActionResource 
 \return false if the context is NOT satisfied
 \date 06.2010
\details updates the necessities and the possibilities and the resources needed and available so the story is updated within EBN.
 
 The possibility is estimated from the necessity of the context, it affects the executability of the module of the action.

 \param  now the time in (floating point) seconds since the story started

*/
bool TActionResource::updatePos(double now){
	
	double temp=1.0;
	double temp2=1.0;
	list<TActionResource*>::iterator pos;
	list<bool>::iterator pos2;

	
	pos2=contextNeg.begin();
	for(pos=context.begin();pos!=context.end();pos++){
		if(!(*pos2))
			temp=(*pos)->getNec();
		else
			temp=1.0-(*pos)->getNec();
		pos2++;
		if (temp < temp2){ //uses min as fuzzy and	
			temp2= temp;
		}
	}

#ifdef DEBUGWITHTIMESTEPS
	if (temp2 < REASONABLE_CONDITION_SATISFACTION) //some contextual condition has to occur / be satisfied
		return FALSE;
#endif

	//depending on time windows, the action should not be possible anymore:


	temp=-1;
	temp2=-1;
	double temp3=-1;
	for(pos=context.begin();pos!=context.end();pos++){
		temp=(*pos)->getTTime();
		if(temp2 < temp)
			temp2= temp;
	}

	for(pos=cause.begin();pos!=cause.end();pos++){
		temp=(*pos)->getTTime();
		if(temp3 < temp)
			temp3= temp;
	}

	
	if((temp2 > -1) & (temp2 + tmin < now ) )	//the last contextual element + the minimum delay to it have occurred: its time to check the cause.
	//all the conditions have been met, and the additional delay.
		this->pos =1.0;

	if( (temp3 > -1)&(temp3 + tmax < now)){ 
		//the last cause and the delay have passed. this action cannot be triggered anymore
		//this is generally solved in isPast. However, the y action as in if x then y has not been triggered and it is past. So:
		this->pos=0.0;

		this->tt=0; //in order it does not get updated anymore

#if VERBOSE
		string s=this->getName();
		s =" Action:  " + s;
		s += " is estimated to NOT have occurred but to be part of the past \n";
		SEND2LOG((char*) s.c_str());
#endif
		return FALSE;
	}
	return TRUE;

}
	

/*!

 \brief it updates the necessity value of the ActionResource 
 \return  a bool that tells if it could update the amount of resources expected to be needed  (Tau_u, in Dorer's notation)
 \date 06.2010
\details 
 
from the necessity of the cause, and considering the fact if it is internal or external, 
		it estimates the necessity of the event, and if it is done by someone else or not.
		
		On a second step, taking into account the temporal constraints, it adjust the actual value of the necessity,
				which in EBN translates into the amount of "Action as Resource" available
				and also estimates the amount of "Action as Resource" needed  according to the time intervals

 \param  now the time in (floating point) seconds since the story started

\todo Adapt a bit if the case where an element can have several potential causes might be considered

*/

bool TActionResource::updateNec(double now){

//the amount needed. This is Tau_U changed from s, in Dorer's notation. It has to be updated within the network
double tau_u=MAX_TAU_U;

if( (this->getTTime() == -1)  & (this->getPos()==1) ){  //action not triggered but possible

	string s (this->getName());
	string s2 ("HIS_FRIEND_FALLS");

	double temp=0.0;
	double maxtemp=0.0;
	list<TActionResource*>::iterator pos;

	double ttcause=now; //worse case
	if(causeNeg)  // NOTP case
			{
					bool NoneTriggered=true;
					for(pos=cause.begin();pos!=cause.end();pos++){ //check no cause is satisfied
						temp=(*pos)->getNec(); // a v b v c
						if(temp > maxtemp){ 
							maxtemp =temp;
						}
						if((*pos)->getTTime() > -1) //some has been triggered
							NoneTriggered=false;
					}
					if(NoneTriggered){
							ttcause=context.back()->getTTime();
							this->nec= 1- maxtemp; // nec=not(a v b v c)
					}
			}	
	else if(getIsExternal()){ //external as the x in IF x THEN y
		double temp=estimator->GetTruthValue();
		if(this->getNec() <= temp){	
				this->nec =temp; //we can set it directly
				ttcause=context.back()->getTTime();
		}
	}
	else{
		for(pos=cause.begin();pos!=cause.end();pos++){ //check the causes are OK
				//temp=(*pos)->getResource();
				temp=(*pos)->getNec();
			if(temp > maxtemp){ //any cause is enough
				maxtemp =temp;
				double ttcausecand=(*pos)->getTTime();
			 if(ttcausecand > -1) //the cause has been triggered
				ttcause=ttcausecand; //if several causes as candidates, this might be redone to take the minimum one. TODO
			}
		}
	}



	temp = 0;

	double necByCauses=maxtemp;

	double temp_ramp=0;
	//set the necessity value (for causation spreading)	
	if(this->getNec() <= necByCauses){ //NOTE: maxtemp will be 0 if the event is external or NOTP
		
		//temp= maxtemp; //the necessity of the cause, as necByCause
		if( (now - ttcause > tmin ) & (now-ttcause < tmax)) {  //so, if it is within the right interval

			temp_ramp=( (now - ttcause) - tmin ) /(tmax -tmin); 
			//but using a ramp does not take into consideration if the cause is an estimated one.
			//In this case, maxtemp might be smaller than 1, and should be taken into consideration. so:
			
			temp_ramp=necByCauses*temp_ramp;			
			//this->nec=temp_ramp; //this should make it softer, but maybe it is better to just avoid that

			 if(this->getNec() < necByCauses) 
				this->nec =temp_ramp; 
			} 
	} //	if(this->getNec() <= maxtemp)

	if( now < tmin + ttcause )   //shouldn't be possible, if the pos. is not 1
		tau_u=MAX_TAU_U; //the amount of elements expected to be used is too high
	else if( now > tmax +ttcause)
		tau_u=0.0;
	else 
		tau_u=MAX_TAU_U*(1-  ( (now - ttcause) - tmin ) /(tmax -tmin));


	double necByRamp=temp_ramp;

 //The substitution principle: event done by ZELIG or event done by ME: ( x or (not(x) then y)
	if(HasEstimator && IsInternal){
		//it has some estimation worked out
		double est=estimator->GetTruthValue();
		
		if(est > necByRamp){ 
			this->nec=est;
			DoneBySomeoneElse=TRUE; //if in the decisional part it is decided the action should be triggered, it will considered to have been done by someone else		
		}
		else if(DoneBySomeoneElse) { 
		//in the following iteration, if the action has not been executed we want to check if the necessity that would be achieved internally
		//is not bigger than the one estimated to be achieved externally, and in this case put everything back to normal
			if(est < necByCauses){
				this->nec=necByCauses;
				//in this case the previously aproved substitution and now it is seen difrently
				DoneBySomeoneElse=FALSE; 
			}
		}
	}

	if( this->HasRole && (role->GetAmountAvailable() < 1.0)   ){
		DoneBySomeoneElse=TRUE; //if the role has been assigned in a previous action
	}




	//set the amount this action is available (for action selection). This is Tau_R changed from s in Dorer's notation
	this->getResource()->UpdateMaxAvailable(this->getNec());


	}
	else{ //if it has been triggered, there is nothing to do, just return 0
	}

	bool b=updateTau_u(tau_u);
	return(b);
	
}  




/*!

 \brief it updates the value of Tau_u in the resource from the one in TACtionResource (calculated in updateNec)
 \date 10.2010
\details 


 \param  

*/

bool TActionResource::updateTau_u(double val){

	this->tau_u=val;


	string s=this->getName();
	bool b=FALSE;

		//put the right value into the amount available
		TAction* a=this->getAction(); 
		if(a!=NULL){ //the action pointer will be null if the actionResource has been added but is not used in the rules/story

			TDorer2Competence* module = a->GetModule();			
			if (module==NULL) { //the action  is a dummy thing like K
				}
			else{
				TLlogicCompetence* mod = (TLlogicCompetence*) module;
				b= mod->UpdateResourceNeeded(this->tau_u,s);
			}
		}

	return(b);
}


/*!

 \brief it assumes the actionResource has occurred / the condition is satisfied
 \date 06.2010
\details 
 it assumes the actionResource has occurred / the condition is satisfied.
 It is useful to start a story by assuming the default context K.
 (also for debugging purposes)


 \param  now the time in (floating point) seconds since the story started

*/



void TActionResource::assumeAsGranted(double now){

			this->setTTime(now);
			this->pos=1.0;
#ifdef DEBUGWITHTIMESTEPS
			this->nec=REASONABLE_CONDITION_SATISFACTION+0.1;
#else
			this->nec=1.0;
#endif
}


		

/*!

 \brief it updates the necessity of occurrence of the action
 \date 06.2010
\details 
 it checks whether the context of the action has occurred in a reasonable degree, and if it is so 
 it updates the necessity of occurrence according to the estimated value obtained from an external estimator.
 It is useful to plug the estimators of the different actions involved in the story plot.
 

 \param  e fuz is the resource associated with the estimation of occurrence of a particular action, which has a callback function associated to fuzzy truth value 

*/


void TActionResource::AddEstimator(TResource* e)
{
	this->estimator = e;
	this->HasEstimator=TRUE;

}



void TActionResource::AddRole(TResource* e)
{
	this->role = e;
	this->HasRole= TRUE;

}



#endif