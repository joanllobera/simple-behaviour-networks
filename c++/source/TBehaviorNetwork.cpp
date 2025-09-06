
#include "NetworkHeader.h"

#include "TObject.h"
#include "TEnergySpreader.h"
#include "TGoal.h"
#include "TAct.h"
#include "TPercept.h"
#include "TAction.h"
#include "TNetworkParams.h"

#include "TConnection.h"
#include "TCompetence.h"
#include "TParseString.h"
#include "TProposition.h"
#include "TWorldState.h"


#include <time.h>
#include "TBehaviorNetwork.h"




TBehaviorNetwork::TBehaviorNetwork()
{
	m_pAct = new TAct();
	m_pPercept = new TPercept();

#if UNITYAGENT==0
	m_RulesFileName=DEFAULTRULEFILENAME;
#endif
}

#if UNITYAGENT==0
TBehaviorNetwork::TBehaviorNetwork(char *pRuleFile)
{
	m_pAct = new TAct();
	m_pPercept = new TPercept();
	m_RulesFileName=pRuleFile;
}
#endif

void TBehaviorNetwork::Initialize()
{

	m_dThreshold = m_pNetworkParams->GetTheta();

	// seed random generator (only needed for random action selection)
	// srand((unsigned) time(NULL));
		
}
	
TBehaviorNetwork::~TBehaviorNetwork()
{
	DeleteNetwork();

	delete m_pAct;
	delete m_pPercept;


	delete m_pNetworkParams;


}

void TBehaviorNetwork::DeleteNetwork()
{

	m_pCompetenceList.clear();
	m_pGoalList.clear();

}


#if USEBEHAVIORNETWORKSWITHOUTRESOURCES
/*!

 \brief returns sum of activation in the network
 \return sum of activation in the network
 \date 06.2010
 \details  calculates the new input to the network and the spreading of activation
				 within the network. Only one cycle of activation spreading is executed.
				 It is assumed, that there is real perception available, that is, the
				 agent is either connected to the server, or provided with some logged
				 or fake perception

*/



double TBehaviorNetwork::DoActivationSpreading()
{

	list<TCompetence*>::iterator pos;
	
	TCompetence*	module;
	double			activity;

	// first calculate all executabilities (and initialize sumtaup)
	for(pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end();++pos)
	{

		module = (TCompetence*) (*pos);
		module->CalculateExecutability();
	}


	for(pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end();++pos)
	{

		module = (TCompetence*) &(*pos);
		module->CalculateActivation();
	}

	// calculate the activation spreading of competences
	//CalculateSpreading();

	// calculate and set are separated, so that new activations depend all on the 
	// current activations of modules and not of their new energy
	activity = 0.0;


	for(pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end();++pos){
		activity += ((TCompetence*) &(*pos))->SetToNewActivation();
	}

	return activity;
}
#endif

/*!

 \brief selects an action
 \date 1998
 \details first : calculates the activation of each module
				 second: sets the new activation of each module and determines the 
							module(s) with highest activation
				 third : executes the module(s) with highest activation, if it is 
							higher than the threshold

\todo there is a version for random action selection implemented that is commented. Make it available
*/

bool TBehaviorNetwork::DoActionSelection()
{

	list<TCompetence*>::iterator pos;
	TCompetence*	module;
	int				iNoOfExecutedModules = 0;

	list<TCompetence*> listOfOrderedCompetences;


	for(pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end();++pos)
	{

		module = (TCompetence*) (*pos);
		module->SetExecuted(FALSE);
		if (module->GetExecutability() > 0.01)
		{
			// sort the executable modules by activation and executability
			InsertSorted(listOfOrderedCompetences, module);
		}
	}

	// check if highest module is higher than threshold 
	for(pos = listOfOrderedCompetences.begin(); pos != listOfOrderedCompetences.end();++pos)
	{
		module = (TCompetence*) (*pos);
		if (module->CheckForExecution(m_dThreshold))
		{
			// only execute one module is standard
			iNoOfExecutedModules++;
			pos = listOfOrderedCompetences.end();
		}
	}

	if (iNoOfExecutedModules == 0){
		return FALSE;
	}
	else
	{
		// at least one action has been executed, reset theta
		m_dThreshold = m_pNetworkParams->GetTheta();

		return TRUE;
	}
}



/*!

 \brief inserts the module into the list sorted by the activation*executability
 \date 1999
 \param thelist is the list of competence modules within the network, the module is the particular element to insert

*/
void TBehaviorNetwork::InsertSorted(list<TCompetence*> thelist, TCompetence* module)
{

	list<TCompetence*>::iterator pos;
	list<TCompetence*>::iterator oldPos;


	TCompetence*	compare;
	double		value = module->GetActivationAndExecutability();


	for(pos = thelist.begin(); pos != thelist.end();++pos)
	{
		oldPos = pos;
		compare = (TCompetence*) (*pos);
		if (value >= compare->GetActivationAndExecutability())
		{
	
			thelist.insert(oldPos, module);
			return;
		}
	}

	// it is the lowest object so far
	thelist.push_back(module);
}





/*!

 \brief inserts the module into the list sorted by the activation*executability
 \date 11.1998
 
*/

void TBehaviorNetwork::ReduceThreshold()
{
	m_dThreshold -= m_pNetworkParams->GetThetaReduction();
}


/*!

 \brief calls the update function to update all propositions
 \date 2014

*/

void TBehaviorNetwork::UpdatePerception()
{ 
		m_pPercept->CalculateAllActivations();
	
}


/*!
\brief connects a new competence with the other competence modules and goals that are related to it
\return
\date 02.2014

\param
\todo
\details   connects a new competence with the other competence modules and goals that are related to it
\sa  TCompetence::ConnectCompetence

*/

void	TBehaviorNetwork::ConnectCompetence(TCompetence* pNewCompetence){
		pNewCompetence->ConnectCompetence(m_pCompetenceList, m_pGoalList);
}



/*!

\brief this method parses the string containing the rules and tries to create the goals and competence modules 
\return it returns TRUE if at least one rule could be parsed
\date 02.2014

\param the string containing the rules
\todo
\details  this method parses the string containing the rules and tries to create the goals and competence modules
\sa   TDorer2Network  TLlogicNetwork
*/


bool TBehaviorNetwork::ParseRules(string rules)
{

	
	TCompetence*	pNewCompetence;
	TGoal*			pNewGoal;
	TParseString	str(rules);


	string			temp;
	string			message;
	string			rule;

	bool			bWithinRule = FALSE;
	bool			bWithinGoal = FALSE;
	int				iLastPosition;
	int				iRuleStart;
	int				iGoalStart;
	
	// delete the existing network
	DeleteNetwork();
	// and initialize Worldstates
	m_pPercept->InitializeAll();

	// search for ifs and endifs
	iLastPosition =(int) str.GetPosition();
	
	
	
	while (str.ParseString(temp))
	{

#if VERBOSE
	char buf[256];
	int bufval=sprintf(buf,"parsing new string: %s \n",temp.c_str());
	SEND2LOG(buf);
#endif
	
		// if
		if (temp.compare(STRING_IF) == 0)
		{
			if (!bWithinRule && !bWithinGoal)
			{
				// start of a rule detected
				iRuleStart = iLastPosition;
				bWithinRule = TRUE;
			}
			else
			{
				SEND2LOG("If without endif or endgoal! \r\n Parsing Stopped.");

				return FALSE;
			}
		}
		
		else if (temp.compare(STRING_GOAL) == 0)
		{
			if (!bWithinGoal && !bWithinRule)
			{
				// start of a goal detected
				iGoalStart = iLastPosition;
				bWithinGoal = TRUE;
			}
			else
			{
				SEND2LOG("Goal without endgoal or endif! \r\n Parsing Stopped.");
				return FALSE;
			}
		}
		
		else if (temp.compare(STRING_END_IF) == 0)
		{
			if (bWithinRule)
			{
				// end of a rule detected
				rule = str.substr(iRuleStart,str.GetPosition()-iRuleStart);
				str.erase(iRuleStart,str.GetPosition()-iRuleStart); 
				str.SetPosition(str.GetPosition()-(str.GetPosition()-iRuleStart));

				pNewCompetence = CreateNewCompetence(); // this will create a TCompetence or a TDorer2Competence, depending on what is needed.
				if (ParseCompetenceRule(pNewCompetence, rule))
				{
					// connect the new rule to the other modules, perceptions and goals
					ConnectCompetence(pNewCompetence);

					// add the new rule to the list of competences
					m_pCompetenceList.push_back(pNewCompetence);
				}
				else
					// an error has been detected within the rule
					delete pNewCompetence;

				bWithinRule = FALSE;
			}
			else
			{
				SEND2LOG("Endif without if! \r\n Parsing Stopped.");
				return FALSE;
			}
		}
	
		// endgoal
		else if (temp.compare(STRING_END_GOAL) == 0)
		{
			if (bWithinGoal)
			{
				// end of a rule detected
				rule = str.substr(iGoalStart,str.GetPosition()-iGoalStart);
				str.erase(iGoalStart,str.GetPosition()-iGoalStart);  
				str.SetPosition(str.GetPosition()-(str.GetPosition()-iGoalStart));

				

				pNewGoal = CreateNewGoal();// this will create a TGoal or a TDorer2Goal, depending on what is needed.
				pNewGoal->SetIndex(m_pGoalList.size());

				bool b=pNewGoal->ParseGoal(rule, m_pPercept);
				if (b)
					// add the new rule to the list of competences
					m_pGoalList.push_back(pNewGoal);
				else
					// an error has been detected within the rule
					delete pNewGoal;

				bWithinGoal = FALSE;
			}
			else
			{
				SEND2LOG("Endgoal without Goal! \r\n Parsing Stopped.");
				return FALSE;
			}
		}
	
		iLastPosition = str.GetPosition();
	}
	
	if (GetNoOfRules() > 0)
	{

#if VERBOSE	
		int numrules = (int)GetNoOfRules();
		int numgoals = (int)m_pGoalList.size();
		int numactions = m_pAct->getNumberOfActions();

		char* buf = new char[128];
		int bufferval = sprintf(buf, "I have connected  the Network.  I found %d competence module(s), %d goal(s) and I also have %d action(s) listed \n", numrules, numgoals, numactions);
		SEND2LOG(buf);
#endif

		return TRUE;
	}


	else
	{
		SEND2LOG("No Rules detected !"); 
		return FALSE;
	}
	
return FALSE;
}



/*-----------------------------------------------------------------------------
Function: bool TBehaviorNetwork::ParseCompetenceRule(TCompetence* pCompetence, LPCTSTR rule)
Date    : 16.04.99
-------------------------------------------------------------------------------
Parameter: pointer to the competence module, that should contain the rule
			  string that contains the rule to parse
Returnval: (void)
Description: calls the modules parse function. Needed because ParseRule interface
				 differs from network to network
-----------------------------------------------------------------------------*/



/*!

\brief
\return This function parses the rule that corresponds to the competence module
\date 02.2014

\param pCompetence is a pointer to the module that should integrate the rule. Rule is a string describing the rule 
\todo
\details  This function parses the rule that corresponds to the competence module. The actual parsing method is defined in TCompetence
\sa  TCompetence::ParseRule
*/


bool TBehaviorNetwork::ParseCompetenceRule(TCompetence* pCompetence, string rule)
{
	return pCompetence->ParseRule(rule, m_pAct, m_pPercept);
}

int TBehaviorNetwork::GetNoOfRules() const 
{
 
	return m_pCompetenceList.size();
}






void TBehaviorNetwork::SetThreshold(double val) 
{
	m_dThreshold = val; 
}

TNetworkParams* TBehaviorNetwork::GetNetworkParams() 
{
	return m_pNetworkParams;
}



TCompetence* TBehaviorNetwork::CreateNewCompetence() 
{
	return NULL;
}

TGoal* TBehaviorNetwork::CreateNewGoal()
{
	return NULL;
}



#if USEBEHAVIORNETWORKSWITHOUTRESOURCES
bool TBehaviorNetwork::PerformCycle(TNetworkParams* params, int iNoOfCycles)
{
	int	i = 0;
	bool	bSelectedAction;
	bool	bReachedGoal;

	// set the network parameters to the new ones
	m_pNetworkParams->SetNetworkParams(params);

	do
	{
		// repeat until an action has been selected
		bSelectedAction = FALSE;

		// update the goal relevances
		UpdatePerception();

		do
		{
			// reduce the threshold
			TBehaviorNetwork::ReduceThreshold();
			// and do a further activation spreading
			DoActivationSpreading();

			// do the action selection in old behavior network semantik selecting
			// the most active module
			bSelectedAction = TBehaviorNetwork::DoActionSelection();
		} while (!bSelectedAction);

		// reset theta
		m_dThreshold = m_pNetworkParams->GetTheta();

		// check if we reached the goal
		bReachedGoal = CheckGoalReached();

		i++;
		
	} while (!bReachedGoal && (i < iNoOfCycles));
	
	return bReachedGoal;
}
#endif


/*!

\brief this function adds a perception and the corresponding callback within the perception factory
\return
\date 02.2014

\param   name of the perception
a pointer to the object containing the callback function
a pointer to the callback function that returns the truth value

\todo
\details
\sa TPercept
*/

void TBehaviorNetwork::AddPerception(string name, TObject* pObj, PCallBackPerception pFn)
{
	m_pPercept->AddPerception(name, pObj, pFn);
}



/*!

\brief It adds an action and the callback needed to execute the action
\return
\date 02.2014

\param   name of the perception
pointer to the object that contains the callback function
a pointer to the callback function that executes the action
\todo
\details
\sa  TAct
*/


void TBehaviorNetwork::AddAction(string name, TObject* pObj, PCallBackAction pFn)
{
	m_pAct->AddAction(name, pObj, pFn);
}






#if UNITYAGENT==0

/*!

\brief  this function restarts the network and connects all the elements
\return
\date 02.2014

\param 
\details
\sa
*/


void TBehaviorNetwork::ResetNetwork()
{

	ifstream m_RulesFile;
	m_RulesFile.open(m_RulesFileName);
	string rules;

	int length;
	char c;
	c=(char) "";

	while(!m_RulesFile.eof()){
		m_RulesFile.get(c);
		rules=rules+c;
	}
	ParseRules(rules);



#ifndef STORIES	//just for the logfile...
	char buf[256];
	int numrules=(int) GetNoOfRules();
	int numgoals=(int) m_pGoalList.size();

	int bufferval=sprintf(buf,"I have reset the Network. I found %d rules, %d goals \n ", numrules, numgoals);
	SEND2LOG(buf);
#endif
}









#else



/*!

\brief  this function restarts the network and connects all the elements
\return
\date 02.2014

\param a string containing the rules
\details
\sa
*/


void TBehaviorNetwork::ResetNetwork(string rules)
{

	ParseRules(rules);



#ifndef STORIES	//just for the logfile...
	char buf[256];
	int numrules = (int)GetNoOfRules();
	int numgoals = (int)m_pGoalList.size();

	int bufferval = sprintf(buf, "I have reset the Network. I found %d rules, %d goals \n ", numrules, numgoals);
	SEND2LOG(buf);
#endif
}







/*!

\brief this function adds a perception and the corresponding callback within the perception factory
\return
\date 02.2014

\param   name of the perception
a pointer to the object containing the callback function
a pointer to the callback function that returns the truth value

\todo
\details
\sa TPercept
*/


void TBehaviorNetwork::AddPerception(string name, TCallbackConverter* pObj)
{
	m_pPercept->AddPerception(name, pObj);
}



/*!

\brief It adds an action and the callback needed to execute the action
\return
\date 02.2014

\param   name of the perception
pointer to the object that contains the callback function
a pointer to the callback function that executes the action
\todo
\details
\sa  TAct
*/


void TBehaviorNetwork::AddAction(string name, TCallbackConverter* pObj)
{
	m_pAct->AddAction(name, pObj);
}







#endif



bool TBehaviorNetwork::CheckGoalReached()
{
	bool		bGoalReached = TRUE;
	TGoal*	pGoal;

	list<TGoal*>::iterator pos;

	for(pos=m_pGoalList.begin();pos !=m_pGoalList.end();++pos)
	{
			pGoal = (TGoal*) (*pos);
		if (pGoal->GetCondition()->GetValue() > 0.5){
			bGoalReached = TRUE;
			return TRUE;
			}
		else
			bGoalReached = FALSE;
	}
	return bGoalReached;
}

