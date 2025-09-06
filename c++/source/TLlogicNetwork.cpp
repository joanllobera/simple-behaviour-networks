#include "TLlogicNetwork.h"


#include "TParseString.h"

#include "TGoal.h"
#include "TDorer2Goal.h"

#include "TResource.h"
#include "TActionResource.h"

#include "TCompetence.h"
#include "TDorer2Competence.h"
#include "TLlogicCompetence.h"
#include "TProposition.h"

//#include "TDorer2Network.h"

#include "TResourceProposition.h"
#include "TResourceFactory.h"
#include "TNetworkParams.h"

#ifdef UNITYAGENT
#include "TCallbackConverter.h"
#endif



TLlogicNetwork::TLlogicNetwork() :TDorer2Network()
{
}

#if UNITYAGENT==0
TLlogicNetwork::TLlogicNetwork(char *pRuleFile) : TDorer2Network(pRuleFile)
{
}
#endif

TLlogicNetwork::~TLlogicNetwork(void)
{
}



/*!

 \brief parses a text file and embedds a story within an existing EBN
 \return TRUE, if it found at least 1 rule and no errors in the case it found 1 or more stories. 
 \date 2010
 
 \param  the string to be parsed
 

 \details  It parses the Rules. It executes TBehaviorNetwork::ParseRules, 
			and then checks if there are stories. If there are, it tries to integrate them inside the Network.
			This functions is called by TLlogicNetwork::ResetNetwork
 
 \sa TBehaviorNetwork::ParseRules , TLlogicNetwork::ResetNetwork

*/

#if STORIES

bool TLlogicNetwork::ParseRules(string rules) 
{

	bool result=FALSE;
	string temp=STRING_START_STORY;
	size_t start=rules.find(STRING_START_STORY);

	string onlytherules="";
	string onlythestory="";

	bool bStoryInRules = (start != rules.npos); //there is a story
	if (bStoryInRules) 
	{
		onlytherules=rules.substr(0,start);
		 onlythestory=rules.substr(start+temp.size(),rules.size());
	}
	else {
		onlytherules=rules;
	}
	
	result=TBehaviorNetwork::ParseRules(onlytherules);

	if (bStoryInRules){
#if VERBOSE
		stringstream logstream;
		logstream << "the story is: " << onlythestory ;
		SEND2LOG(logstream.str().c_str());
#endif

		bool result2 = CreateStory(onlythestory);
		result = result & result2;

#if VERBOSE
		char* buf = new char[128];
		int bufferval = sprintf(buf, " I created  %d story(ies) \n. \n \n ******************************** \n \n  ", storyCount);
		SEND2LOG(buf);
#endif
		
	}
	return (result);
	
}
#endif



	




/*!

 \brief It restarts the network, parses the rule file, creates all the modules, etc. and connects everything
 \return TRUE, if at least one rule could be parsed
 \date 05.2010
\details It restarts the network, parses the rule file and creates all the modules / resources. It expects to have all the actions previously added
redefined from TBehaviorNetwork::ResetNetwork() only to have a consistent log file coherent with having stories.
 
 \param 
 \return (void) 
 \sa   TBehaviorNetwork::ResetNetwork()

*/

#if UNITYAGENT==0
void	TLlogicNetwork::ResetNetwork(){
	storyCount=0;
	TLlogicNetwork::ParseRules(rules);

#if STORIES	
	int numrules=(int) GetNoOfRules();
	int numgoals=(int) m_pGoalList.size();
	int numactions=m_pAct->getNumberOfActions();

	char buf[256];
//#if VERBOSE

//#else
	int bufferval=sprintf(buf," I created  %d story(ies) \n. \n \n ******************************** \n \n  ",storyCount);
//#endif
	SEND2LOG(buf);
#endif

}
#else
void	TLlogicNetwork::ResetNetwork(string rules){

#if STORIES
	storyCount = 0;
#endif

	TLlogicNetwork::ParseRules(rules);
	
}

#endif



/*!

 \brief   factory function
 \date 05.2010 
 \return (void)
 \details It creates a new competence empty module of the kind TLlogicCompetence, which ensures that it can process the actions in a story, as well as actions needing resources,
			such as TDorer2Competence modules can.

 \sa  TDorer2Network::CreateNewCompetence

*/

TCompetence* TLlogicNetwork::CreateNewCompetence()
{
	return new TLlogicCompetence(m_pNetworkParams, "", GetNoOfRules(),(int) m_pGoalList.size());
}






/*!

\brief ensures a label with no strange characters is created (for example, ATPS1)
\date 05.2010
\param name of the story (for example, S1)
\return the cleaned label created in string format


*/


#if STORIES
string TLlogicNetwork::makeATPlabel(string story){

			//we ensure there is nothing wrong:
			TParseString temp=story;
			temp.OverreadLeadingSpaces();
			story=(string) temp.substr(temp.GetPosition(),temp.size());

			//we make the label
			string ATPlabel=STRING_ATP;
			ATPlabel+=story;
			return ATPlabel;				
}

/*!

 \brief ensures a label with no strange characters is created (for example, KS1)
 \date 05.2010 
 \param name of the story (for example, S1)
 \return the cleaned label created in string format


*/

string TLlogicNetwork::makeKlabel(string story){

			//we ensure there is nothing wrong:
			TParseString temp=story;
			temp.OverreadLeadingSpaces();
			story=(string) temp.substr(temp.GetPosition(),temp.size());

			//we make the label
			string Klabel=STRING_DEFAULT_CONTEXT;
			Klabel+=story;
			return Klabel;				
}

string TLlogicNetwork::makeROLElabel(string name){

			TParseString temp=name;
			temp.OverreadLeadingSpaces();
			name=(string) temp.substr(temp.GetPosition(),temp.size());

			string ROLElabel=STRING_ROLE;
			ROLElabel+=name;
			return ROLElabel;

}

/*!

 \brief ensures a label with no strange characters is created (for example, ESTa1)
 \date 05.2010 
 \param name of the action (for example, a1)
 \return the cleaned label created in string format


*/

string TLlogicNetwork::makeEstimatorlabel(string name){

			//we ensure there is nothing wrong:
			TParseString temp=name;
			temp.OverreadLeadingSpaces();
			name=(string) temp.substr(temp.GetPosition(),temp.size());

			//we make the label
			string label=STRING_ESTIMATOR;
			label+=name;
			return label;				
}



/*!

 \brief ensures a label with no strange characters is created (for example, NOTP0)
 \date 05.2010 
 \param which amount of label are we creating
 \return the cleaned label created in string format


*/

string TLlogicNetwork::makeNOTPlabel(int intValue){

  char *myBuff;
  string strRetVal;

  // Create a new char array
  myBuff = new char[100];

  // Set it to empty
  memset(myBuff,'\0',100);

  // Convert to string
  _itoa(intValue,myBuff,10);

  // Copy the buffer into the string object
  strRetVal = myBuff;
  
  // Delete the buffer
  delete[] myBuff;

  string NOTPlabel=STRING_NONE_OF_PREVIOUS;
 NOTPlabel=NOTPlabel + strRetVal;
	return NOTPlabel;
}



/*!

 \brief returns a double in string format
 \date 06.2010
 \param intValue is the double to be converted to string

*/

string TLlogicNetwork::printdoublehere(double intValue){
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



/*!

\brief ensures a label has no strange items, such as spaces,tabs, etc.
\date 05.2010
\param the string to be cleaned

*/


string TLlogicNetwork::cleanstring(string rule){
			TParseString temp=rule;
			temp.OverreadLeadingSpaces();
			rule=(string) temp.substr(temp.GetPosition(),temp.size());
			return rule;
}


/*!

 \brief Adds everything needed to process a new story
 \date{05.2010}
 \details Adds a goal, a new effect and an actionResource representing the default context 
 \param rule is the name of the story (for example, S1)
 \sa  TLlogicNetwork::CreateStory(string story)
*/

bool TLlogicNetwork::initializeStory(string rule){
					

			hasEnded=0.0;		

			string ATPlabel=makeATPlabel(rule);
			string Klabel=makeKlabel(rule);

			//add its acomplishment as a perception:---------------------------------------------
		//	TActionResource* actionL=new TActionResource(ATPlabel);
		//	connectActionResource(actionL); 		

			

//#if UNITYAGENT==0
		m_pPercept->AddPerception(ATPlabel, this, (PCallBackPerception) &TLlogicNetwork::perceiveHasEnded); //This is necessary for the effects to be added to the modules

/*#else
			m_pPercept->AddPerception(ATPlabel, this, (PCallBackPerception) perceiveHasEnded); //This is necessary for the effects to be added to the modules

#endif
			*/

		
	
			
			//add goal----------------------------------------------------------------------
			TGoal*			pNewGoal;
			pNewGoal = CreateNewGoal();
			pNewGoal->SetIndex(m_pGoalList.size());

			
			char* cs=new char[128];
				
			/*

			the Goal Condition is important when the relevance condition is satisfied. So, the goal condition should match the effect of the actions in the plot,
			this is, ATPS1, and the relevance condition should be the default context story context K:
			
			goal K (Relevance Condition) then ATPS1 1.0 (Goal Condition and importance) endgoal
			*/
			int i=sprintf(cs,"%s %s %s %s %f %s",STRING_GOAL, Klabel.c_str() , STRING_THEN_GOAL, ATPlabel.c_str(), 1.0 , STRING_END_GOAL );
			
			string stringGoal=cs;

			bool b=((TDorer2Goal*) pNewGoal)->ParseGoal(stringGoal, m_pPercept); //a new goal with the label name 
			if (b)
				// add the new goal to the list.
				m_pGoalList.push_back(pNewGoal);
			else{
				// an error has been detected within the rule
				delete pNewGoal;
				SEND2LOG("Error Creating story goal. \n parsing stopped.");
				return FALSE;
			}
			

	
			
	return TRUE;



}




/*!
 \brief It makes the story start
 \return TRUE, if at least one rule could be parsed
 \date 05.2010
\details It finds the default context of the story within the list of tactionresources, and assumes it as granted, 
		which starts triggering the cascade of possibilities within the list of tactionresources.

 \param  now the time in (floating point) seconds
*/
void TLlogicNetwork::startStories(double now){

	string rule=this->storyLabel;
	string name=makeKlabel(rule).c_str();		
	list<TActionResource*>::iterator pos2;
	for(pos2=this->pActRes.begin();pos2!=pActRes.end();pos2++){
		if((*pos2)->getName()==name){
			(*pos2)->assumeAsGranted(now);
			pos2=pActRes.end();
			pos2 --;
		}
	}
}


/*!
 \brief It update all the action resources in the network, 
 \return TRUE, if all the action resources were found and updated
 \date 05.2010
\details updates the necessities and the possibilities and the resources needed and available so the story is updated within EBN
 \param  now the time in (floating point) seconds since it started
*/
bool TLlogicNetwork::updateStories(double now){

	bool b=TRUE;

	if(!storyEnded()){
	//lastnow=now; //to then update the tt

	list<TActionResource*>::iterator pos;
	// \todo all these can be threaded in parallel
	for(pos=pActRes.begin();pos!=pActRes.end();pos++){
		bool b2 =(*pos)->Update( now);
		b= b & b2;
	}
	return b;
	}
	else 
		return FALSE;
}

/*!
 \brief connects the action to the Behavior Network
 \return the actionResource, if it could find the action and connect everythin. NULL otherwise
 \date 2010
\details connects the action to the Behavior Network
			it connects it to an action existing in the action factory 
			adds a resource in the resource factory connected to the possibility value in here.
			adds a perception in the perception factory

			if needed, it also adds the new ActionResource to the ActionResource list of the network.
 
			The function pressuposes the action exists in the list of TActions, otherwise it creates an assertion.

 \param  the action / resource
 
 \sa   TActionResource::connect
*/


TActionResource* TLlogicNetwork::ConnectActionResource(string label){
	string name=label;
	TActionResource* actionL=GetActionResource(name);
	if(actionL==NULL){
		actionL = new TActionResource(name);
		//adds it to the list
		pActRes.push_back(actionL); 
	};



	TAction* m_pAction=GetAction(name); 
	if (m_pAction==NULL){

		string a="sorry, the action ";
		a+=name;
		a +=" does not exist";
		SEND2LOG(a.c_str());
		ASSERT(m_pAction!=NULL);

		return NULL;
	}
	else{
		m_pAction->setActRes(actionL);
	}

	TResource* res= this->GetResource(name);
	if (res == NULL)  //I check if the resource has not been added previously
	{
		//the function it will call when checking the action is needed, i. e., has a cause
		res=AddGetResource(name, (TObject*) actionL, (PCallBackPerception) &TActionResource::getNec, 1.0); 
		res->SetSpendable(FALSE);
	}

	//adding it to the perceptions list: 
	TWorldState* prop=m_pPercept->GetPerception(name); //check it does not exist
	if (prop == NULL ){

		m_pPercept->AddPerception(name, actionL,(PCallBackPerception) &TActionResource::getPos);
		prop=m_pPercept->GetPerception(name); 
	}

	actionL->connect(res, m_pAction, prop);

	return actionL;
}




/*!
\brief sets an action as a new actionresource
\return
\date 05.2010

\param the name of the action to be set as a new actionResource
\todo
\details  adds the action resource (i. e., an action with context and cause)
			within the list of action resources, checks whether it should be added 
			to the special module, and connects it to the right action, 
			perception and resource. If the action does not exist, it creates it
\sa
*/
TActionResource* TLlogicNetwork::AddActionResource(string name, string storyLabel){
	name=cleanstring(name);
	TActionResource* actionL=ConnectActionResource(name); 
	TLlogicCompetence* module=(TLlogicCompetence*) GetCompetence(name);
	if(module==NULL){//there is no such module with this action , so CREATE A NEW MODULE

			string actionName=name;
			string ATPlabel=makeATPlabel(storyLabel);
			//	if actionName(will check possibility calculated from its context) then actionName (will run action)
			// effect ATP_S1 using actionName (will check necessity calculated from its cause)
			char* cs=new char[128];
	
			int i=sprintf(cs,"%s %s %s %s %s %s %s %s %s",STRING_IF,(char*) actionName.c_str(),STRING_THEN_SKILL,(char*) actionName.c_str(),STRING_EFFECT,(char*) ATPlabel.c_str(),STRING_USING,(char*) actionName.c_str(),STRING_END_IF);
			string specialrule=cs;

			TLlogicCompetence* pNewCompetence = (TLlogicCompetence*) CreateNewCompetence();
						// add the new rule to the list of competences
			
			pNewCompetence->setUpdateTriggerTime(TRUE);
			if (ParseCompetenceRule(pNewCompetence, specialrule))
				{
				m_pCompetenceList.push_back(pNewCompetence);
				ConnectCompetence(pNewCompetence); 
				}
			else{
				// an error has been detected within the rule
				delete pNewCompetence;
				i=sprintf(cs," Error adding a module for the story module action %s \n ",(char*) actionName.c_str());
				SEND2LOG(cs);
				return NULL;
				}

	}else{
		//add an ATP_S1 effect 
		string effectLabel=makeATPlabel(storyLabel);

		TWorldState* pState=m_pPercept->GetPerception(effectLabel);
		TProposition* pEff = new TProposition(module, pState, 1.0, FALSE);

		module->AddEffect(pEff,this->m_pCompetenceList,this->m_pGoalList);//already avoids repetitions, and if it isnew connects in the network
		module->setUpdateTriggerTime(TRUE);
		pState->SetUsed();

		TWorldState* actionLstate = actionL->getState();

		//add its context as a precondition needed:
		TProposition* pProposition = new TProposition(module, actionLstate, 1.0, FALSE);
		actionLstate->SetUsed();
		module->AddPrecondition(pProposition,this->m_pCompetenceList);

		//add its cause as a resource used:
		
		TResource* pResource=actionL->getResource();
		if (pResource == NULL){
			SEND2LOG("the actionResource has not been added as a proper resource");
		}
		else{
			TResourceProposition* pResourceProp = new TResourceProposition(module, pResource, 1.0);
			module->AddResource(pResourceProp);
			pResource->SetUsed();
		}
	}

	return actionL;
}







/*!

\brief It searches the TAction* within the list of actions
\return a pointer to the action with the same name, null if it does not exist
\date 02.2014

\param the name of the Action
\details It searches the TAction* within the list of actions
*/

TActionResource* TLlogicNetwork::GetActionResource(string name){		
	list<TActionResource*>::iterator	pos; 
	string		strName = name;
	TActionResource*		pAction;

	for(pos=this->pActRes.begin(); pos != this->pActRes.end(); ++pos){
		if((*pos)->getName().compare(strName)==0){
			pAction=(*pos); 
			return pAction;	
		}
	}
	return NULL;
}


/*!

 \brief adds an Action but also with a method to estimate whether it occured, for example, it was done by some other agent (see principle of Substitution)

 \date 07.2010
 
\details adds an Action but also with a method to estimate whether it occured, for example, 
			it was done by some other agent (see principle of Substitution). If it was called twice,
			the second callback of the estimator would override the first one, and the action would be twice in the list of TActions, \todo which I am not sure
			 what effect it would have, but probably bad.

			It SHOULD NOT be confused with TLlogicNetwork::AddActionResource, which is called when parsing the story to connect the causes and contexts of the actions,
			assuming the TAction exists, and not carrying about estimators or stuff like that.

 \param  the name of the action, and the object and method to be called
 
*/

TActionResource* TLlogicNetwork::AddActionPerception(string name,TObject* pObj,  PCallBackAction pFn, PCallBackPerception pCallback){

	this->AddAction(name, pObj, pFn);
	TAction* act=this->GetAction(name);

	TActionResource* actR=this->GetActionResource(name);
	
	if(actR==NULL){
		actR=new TActionResource(name);
		pActRes.push_back(actR);
	}

	string estimName=makeEstimatorlabel(name);

	TResource* est=this->AddGetResource(estimName, pObj, pCallback, 1.0);


	actR->AddEstimator( est );
	
	act->setActRes(actR);

	return actR;
}



/*!

\brief adds an Action using TLlogicNetwork::AddActionPerception, but also associates it with a role
\date 07.2010

\details   adds an Action using TLlogicNetwork::AddActionPerception, but also associates it with a role

\param  the name of the action, the object and method to be called, and the name of the role associated with performing the action
\sa TLlogicNetwork::AddActionPerception

*/


void TLlogicNetwork::AddActionPerceptionRole(string name, TObject* pObj, PCallBackAction pFn, PCallBackPerception pCallback, string role){

	string r=makeROLElabel(role);

	bool isequal=FALSE; 
	TResource* pRole=NULL;
	//we check the role was not added already.
	pRole =  m_pResourceFactory->GetResource(r);
	if (pRole ==NULL)
		pRole =this->AddGetResource(r, pObj, pCallback, 1.0);
	
	TActionResource* pActRes=AddActionPerception(name, pObj,   pFn, pCallback);
	pActRes->AddRole(pRole);
}


#if UNITYAGENT

/*!

\brief adds an Action but also with a method to estimate whether it occured, for example, it was done by some other agent (see principle of Substitution)

\date 02.2014

\details
adds an Action but also with a method to estimate whether it occured, for example,
it was done by some other agent (see principle of Substitution). If it was called twice,
the second callback of the estimator would override the first one, and the action would be twice in the list of TActions, \todo which I am not sure
what effect it would have, but probably bad.

It SHOULD NOT be confused with TLlogicNetwork::AddActionResource, which is called when parsing the story to connect the causes and contexts of the actions,
assuming the TAction exists, and not carrying about estimators or stuff like that.

\param  the name of the action, and the object dealing with the callback to the action performance and the value estimation

*/

TActionResource* TLlogicNetwork::AddActionPerception(string name, TCallbackConverter* pObj){

	this->AddAction(name, pObj);
	TAction* act = this->GetAction(name);

	TActionResource* actR = this->GetActionResource(name);

	if (actR == NULL){
		actR = new TActionResource(name);
		pActRes.push_back(actR);
	}

	string estimName = makeEstimatorlabel(name);

	TResource* est = this->AddGetResource(estimName, pObj, 1.0);


	actR->AddEstimator(est);

	act->setActRes(actR);

	return actR;
}



/*!

\brief adds an Action using TLlogicNetwork::AddActionPerception, but also associates it with a role
\date 07.2010

\details adds an Action but also with a method to estimate whether it occured, for example,
it was done by some other agent (see principle of Substitution). If it was called twice,
the second callback of the estimator would override the first one, and the action would be twice in the list of TActions, which I am not sure
it would have what effect, but probably bad.

It SHOULD NOT be confused with TLlogicNetwork::AddActionResource, which is called when parsing the story to connect the causes and contexts of the actions,
assuming the TAction exists, and not carrying about estimators or stuff like that.


\param  the name of the action, the object and method to be called, and the name of the role associated with performing the action
\sa TLlogicNetwork::AddActionPerception

*/


void TLlogicNetwork::AddActionPerceptionRole(string name, TCallbackConverter* pObj, string role){

	string r = makeROLElabel(role);

	bool isequal = FALSE;
	TResource* pRole = NULL;
	//we check the role was not added already.
	pRole = m_pResourceFactory->GetResource(r);
	if (pRole == NULL)
		pRole = this->AddGetResource(r, pObj, 1.0);

	TActionResource* pActRes = AddActionPerception(name, pObj);
	pActRes->AddRole(pRole);
}

#endif






/*!

 \brief parses a text file and embedds a story within an existing EBN
 \return TRUE, if it could do it 
 \date 2010
 
 \param  the string to be parsed
 

 \details adds the goals and modules needed to process the story,
			integrates the actions within the other modules and adds the effects
			for them. The story can be parsed just after adding the skills, or later on.
			One or several stories can be added. 
			
The expected structure of a story script is:

Llogic:  

S1:

a1. a2. 

if a31 then a32 endif
if a33 then a34 and a35 endif
if notp then a36 endif

\todo Add in CreateStory parsing method to manage negated conditionals, such as IF NOT X1 THEN Y1
\todo  by now no managment of sub-scene elements such as SS1, or jump forward (although dynamic addition of modules, contexts and causes is there)

 \sa  TLlogicNetwork::ParseRules , which is the main function parsing the rule file provided

*/

bool TLlogicNetwork::CreateStory(string story){
	bool result=TRUE;




	TParseString	str(story);

	string			temp;
	string			rule;

	//accessory variables used in the creation process:
	TActionResource* pCause = nullptr;
	TActionResource* pContext = nullptr;


	int				iLastPosition;
	int				iRuleStart;
	//int				iItemStart;//the beginning of an IF x THEN y


	// search for points, ifs and endifs
	iLastPosition =(int) str.GetPosition();

	list<TActionResource*> listBlockNoCause; //useful when NOTP appears

	bool				bWithinRule = FALSE;

	bool				bFirstIf = TRUE;
	bool				bWithinIf = FALSE;
	bool				bEndBlock = FALSE;


	int notpcount=0;
	double additionaldelay=0;

	while (str.ParseString(temp))
	{


	char buf[256];
#if VERBOSE
	int bufval=sprintf(buf,"parsing new string: %s \n",temp.c_str());
	SEND2LOG(buf);
#endif

		if(temp.find(STRING_COLON)< temp.npos ) //starting a new story
		{
			storyCount++;

			//NOTE: by now no managment of sub-scene elements such as SS1:, or similar elements.
			iRuleStart = iLastPosition;
			
			string strpoint=STRING_COLON;
			
			rule = str.substr(iRuleStart,str.GetPosition()-iRuleStart);
			rule= rule.substr(0,rule.find(STRING_COLON)); //we remove the colon
			
			rule=cleanstring(rule);
			
			str.erase(iRuleStart,str.GetPosition()-iRuleStart); 
			str.SetPosition(str.GetPosition()-(str.GetPosition()-iRuleStart));
			

			//for each new Story:


			//create a story label
			this->storyLabel=rule;



			//add the default context as a dummy action and as an action resource------------------------------------
			string Klabel=makeKlabel(rule);
			
			this->AddAction(Klabel,this,(PCallBackAction) &TLlogicNetwork::doNothing);
			
			// set it as resource and as a proposition, 
			pContext = ConnectActionResource(Klabel);


	
			pCause= pContext;


			bool b=initializeStory(rule); //create the goal
			if(!b)
				return FALSE;

		}
		else if (temp.find(STRING_POINT) < temp.npos ) //there is a point
		{
			// end of an action detected
			iRuleStart = iLastPosition;
			
			string strpoint=STRING_POINT;
			
			rule = str.substr(iRuleStart,str.GetPosition()-iRuleStart);
			rule= rule.substr(0,rule.find(STRING_POINT)); //we remove the point;

			str.erase(iRuleStart,str.GetPosition()-iRuleStart); 
			str.SetPosition(str.GetPosition()-(str.GetPosition()-iRuleStart));
			iLastPosition=str.GetPosition();

			rule=cleanstring(rule);
			TActionResource* res=AddActionResource(rule,this->storyLabel);
	
			res->Add2Cause(pCause);
			res->Add2Context(pContext,FALSE);

			pContext = pCause;
			pCause =  res;

			res->AddDelay(additionaldelay);
			additionaldelay=0.0;


			#if VERBOSE
				int bufval=sprintf(buf,"New Story Action! : %s \n",rule.c_str());
				SEND2LOG(buf);
			#endif

		}
		else if (temp.compare(STRING_IF) == 0)
		{
			 /* structure to parse is:
				IF x1 THEN y1
				IF x2 THEN y2
				IF NOTP THEN yN
		*/
			if (!bWithinRule && !bWithinIf)
			{
				// start of a IF x THEN y rule detected
				bWithinRule = TRUE;
				bWithinIf = TRUE;
			}
			else
			{
				SEND2LOG("An if statement where it shouldn't be. \r\n Parsing Stopped.");
				return FALSE;
			}
		}
		else if (temp.compare(STRING_END_IF) == 0) //optional, in this sintax. It is simply skipped
			{
				str.erase(0,str.GetPosition()); 
				str.SetPosition(0);
			}
		else if(bWithinRule){
			if (temp.compare(STRING_THEN_PLOT) == 0)
			{
				bWithinIf=FALSE;
			}	
			else if(bWithinIf==TRUE)   
			{
				if (temp.compare(STRING_NONE_OF_PREVIOUS) == 0){
				bEndBlock=TRUE;
				}
				else if(bFirstIf){
					pContext = pCause;
					bFirstIf = FALSE;

					TActionResource* res=AddActionResource(temp,this->storyLabel); 
					
					res->Add2Context(pContext,FALSE); 
					//note the x in IF x THEN y is a resource with no cause. Therefore:
					res->setIsExternal();
					pCause=res; //for the next iteration
				}
				else{ //not the first, not the last if

					TActionResource* res=AddActionResource(temp,this->storyLabel);	
					res->Add2Context(pContext,FALSE);

					//note the x in IF x THEN y is a resource with no cause :
					res->setIsExternal();
					pCause=res; //for the next iteration
				}
			
			}
			else if(!bWithinIf & bWithinRule)
			{
				
				// end of a rule detected
				iRuleStart = iLastPosition; 
				rule = str.substr(iRuleStart,str.GetPosition()-iRuleStart); 
				rule=cleanstring(rule); //the action to add

				string tosend=str.substr(0,str.GetPosition());
				tosend=cleanstring(tosend);
#if VERBOSE
				tosend="New story rule added: " + tosend;
				tosend=cleanstring(tosend) + "\n";
				SEND2LOG((char*) tosend.c_str());
#endif
				str.erase(0,str.GetPosition()); 
				str.SetPosition(0);
				bWithinRule=FALSE;

				if (! bEndBlock) //no NOTP just found previously
				{
					TActionResource* res=AddActionResource(temp, this->storyLabel);

					res->Add2Cause(pCause);
					res->Add2Context(pContext,FALSE);

					listBlockNoCause.push_back(pCause);

		

				}
				else //we are in the NOTP case.
				{

				TActionResource* res=AddActionResource(temp, this->storyLabel);


			//! \todo check if ¿I neeed this¿

			//	string NOTPlabel=makeNOTPlabel(notpcount);
			//	notpcount++;
			//	this->AddAction(NOTPlabel,this,(PCallBackAction) &TLlogicNetwork::doNothing); 
		
 				for(list<TActionResource*>::iterator pos=listBlockNoCause.begin();pos !=listBlockNoCause.end(); pos++){

					additionaldelay=additionaldelay + DEFAULT_MEANDELAY;
					res->Add2Cause((*pos));
				}
				res->AddDelay(additionaldelay);
				res->negateCause();
				listBlockNoCause.clear();

				res->Add2Context(pContext,FALSE);
				pCause=pContext; //the cause after the block will be the element before the if block
				
				bFirstIf=TRUE; //READY FOR NEXT BLOCK
				bEndBlock=FALSE;
				}

			}
		}else{ //if it is not followed by a point, an IF or within a IF x THEN y block
			SEND2LOG("Found something that is neither an action and neither in an IF block \r \n  Parsing stopped");
			ASSERT(TRUE);
			return FALSE;
		}
		iLastPosition = str.GetPosition();
	}


	string Klabel=makeKlabel(this->storyLabel);
	string Endlabel=makeKlabel(Klabel);
	this->AddAction(Endlabel,this,(PCallBackAction) &TLlogicNetwork::endStory);
			

	TActionResource* res=AddActionResource(Endlabel,this->storyLabel);

	pContext=this->GetActionResource(Klabel);
	res->Add2Context(pContext,FALSE);
	
	res->Add2Cause(pCause); //already good from previous in the loop
	


	


	return TRUE;

}


/*
#ifdef UNITYAGENT
void TLlogicNetwork::endStory(double value, TLlogicNetwork* tlo){
	tlo->hasEnded=TRUE; //this will provoke that ATPS1 appears to be completed: 1.0
	
#ifndef STORYEND
	string a="Story ";
	string label=this->storyLabel;
	a += label;
	a+= "has ended\n \n";
#endif

#ifdef STORYEND
	string a=STORYEND;

#endif
	SEND2LOG((char*) a.c_str());

//#ifdef CONSOLE_APP
//	printf((char*) a.c_str());
//#endif
	

//	string Klabel=makeKlabel(this->storyLabel);

//	this->GetActionResource(Klabel)->assumeAsCancelled; //this will even make the goal be not relevant

}
#else
*/
void TLlogicNetwork::endStory(double value){
//! \todo check carefully the different options here and debug which is best
#ifndef STORYEND
	string a = "Story ";
	string label = this->storyLabel;
	a += label;
	a += "has ended\n \n";
#endif

#ifdef STORYEND
	string a = STORYEND;

#endif
	SEND2LOG((char*)a.c_str());
	/*
	#ifdef CONSOLE_APP
	printf((char*) a.c_str());
	#endif

	*/


	//	string Klabel=makeKlabel(this->storyLabel);

	//	this->GetActionResource(Klabel)->assumeAsCancelled; //this will even make the goal be not relevant

}

//#endif


#endif






#ifdef DEBUGWITHTIMESTEPS



/*!

 \brief A function that will execute any action that has pos=1 and Tau_u < nec .To be used only for debugging purposes.
 
 \date 2010
 
 \param  the time in floating point seconds
 
\todo update considering the redefinition of tttime, and where the now is now processed.

*/
string TLlogicNetwork::RunNecessaryToTrigger(double now){

	string info;
	list<TActionResource*>::iterator pos;


	int size=pActRes.size();
	vector<double> taus;

	for (pos=this->pActRes.begin(); pos!=pActRes.end();pos++){
		
		bool b= (*pos)->updatePos(now);
		double tau_u=MAX_TAU_U;
		if(b){
			tau_u=(*pos)->updateNec(now);
		}
		taus.push_back(tau_u);
	}


	
	info= "The pos/nec and tt time of the actions performed are: ";

	int n=0;
	int n2=0;
	for (pos=this->pActRes.begin(); pos!=pActRes.end();pos++){

		double a=(*pos)->getPos();
		double b=(*pos)->getNec();
		double c=taus[n];
	
		if ((a==1.0)  & b >= c){
			//(*pos)->getAction()->PerformAction(1.0, m_pNetworkParams->GetPhi()); //not good because the module is not set to "be triggered"

			list<TCompetence*>::iterator pos3;
			for(pos3=this->m_pCompetenceList.begin();pos3!=m_pCompetenceList.end();pos3++){
				string name=(*pos3)->GetAction()->GetName();		
				if((*pos)->getName()==name){
					(*pos3)->ExecuteModule(1.0);
					(*pos)->setTTime(now);
				}	
			}

			double p=(*pos)->getPos();
			double n=(*pos)->getNec();
			double d=(*pos)->getTTime();
			string name=(*pos)->getName();
			n2 ++;
			info += "action: ";
			info +=name;
			info += ", pos: ";
			info= info + printdoublehere(p);
			info += ", nec: ";
			info += printdoublehere(n);
			info += ", tt: ";
			info += printdoublehere(d);
			info += " ; \n";
			SEND2LOG((char*) info.c_str());
		}
		n++;

//	if ((m_pObject != NULL) && (m_pAction != NULL))
//		(m_pObject->*m_pAction)(activation);

	}





return info;

}
#endif

