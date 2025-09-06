// TCompetence.cpp: implementation of the TCompetence class.
//
//////////////////////////////////////////////////////////////////////



#include "NetworkHeader.h"

#include "TAct.h"
#include "TPercept.h"
#include "TAction.h"
#include "TWorldState.h"
#include "TProposition.h"
#include "TParseString.h"
//#include "TScreenParamsNetwork.h"
#include "TWorldState.h"
#include "TGoal.h"
//#include "TGoalTrack.h"
#include "TNetworkParams.h"
#include "TCompetence.h"
#include "TConnection.h"

#include <math.h>

TCompetence::TCompetence(TNetworkParams* params, string name, int index)
					:TEnergySpreader(name, index)
{


	m_pNetworkParams = params;
	m_pAction = NULL;
	m_iNoOfSources = 0;

	for (int i = 0; i < NO_OF_CONNECTION_TYPES; i++){
		m_pLinks.push_back( new list<TConnection*>); 
		}

	m_dSpreadingActivation = 0.0;
	m_dTransferedActivation = 0.0;
	m_dExternActivation = 0.0;
	m_dExecutability = 0.0;

	m_bExecuted = FALSE;
}

TCompetence::~TCompetence()
{
	int	i;

	m_pPreconditions.clear();
	m_pEffects.clear();

	for (i = 0; i < NO_OF_CONNECTION_TYPES; i++)
	{
		m_pLinks[i]->clear();
	}
}

/*!

\brief creates a new connection to the passed energy source.
\return
\date 02.2014

\param pointer to the new connection
			  pointer to the proposition of the source module
			  type of the link
\todo check the managment of source types
\details
\sa
*/

void TCompetence::AddEnergySource(TConnection* newConnection, TProposition* srcProp, int iSourceType)
{
	m_pLinks[iSourceType]->push_back(newConnection);
	srcProp->GetModule()->OnAddedToDestination();	
	m_iNoOfSources++;
}


/*!

\brief connects this competence module
\return
\date 02.2014

\param

connects this competence which means connect it to
- its preconditions (perceptions)
- its goals
- successor competence modules
- predecessor modules
- conflictor modules

\todo
\details 
\sa
*/




void TCompetence::ConnectCompetence(list<TCompetence*> pCompetenceList, list<TGoal*> pGoalList)
{
	list<TProposition*>::iterator	pos;  //preconditions
	list<TCompetence*>::iterator	pos2;	//competence modules
	list<TProposition*>::iterator	pos3;  //effects

	list<TGoal*>::iterator	pos4;  

	list<TProposition*>				pPreconditions;

	list<TProposition*>				pEffects;
	TProposition*		pSource;
	TProposition*		pDest;
	TCompetence*		pCompetence;
	TGoal*				pGoal;
	bool					bInhibition;
	bool					bExcitation;

	// connections due to the preconditions of this module 
	// (proposition, predecessor, conflictor)
	for(pos=m_pPreconditions.begin(); pos != m_pPreconditions.end(); ++pos)
	{
		pDest = (*pos);

		// add as energy source
		AddPropositionLink(pDest->GetWorldState()->GetProposition(), pDest);
		
		// look for predecessor and conflictor links
		for(pos2=pCompetenceList.begin(); pos2 != pCompetenceList.end(); ++pos2)
		{
			pCompetence = (TCompetence*) (*pos2);
			pEffects = pCompetence->GetEffects();

		// go through the effects of the module
			for(pos3=pEffects.begin(); pos3 != pEffects.end(); ++pos3)
			{
				pSource = (TProposition*) (*pos3);
				if (pSource->IsIdentic(pDest))
				{
					// is it a predecessor
					AddPredecessor(pSource, pDest);
					pCompetence->AddSuccessor(pDest, pSource);
					//! \todo !!! this is not completely correct (recheck what this means?)
				}
				else if (pSource->IsInvers(pDest))
				{
					// the other module is a conflictor of the new one
					pCompetence->AddConflictor(pDest, pSource);
					//! \todo !!! this is not correct, because it might have been added for this  proposition before
				}
			}
		}
	}

	// connections due to the effects of this module 
	// (goals, protected goals, successors)
	pEffects = m_pEffects;

for(pos=pEffects.begin(); pos != pEffects.end(); ++pos) 	//now pos is for effects, not for perceptions
	{
		bInhibition = FALSE;
		bExcitation = FALSE;
		pDest = (*pos);
	
		
		// look for successor and conflictor links
		for(pos2=pCompetenceList.begin(); pos2 != pCompetenceList.end(); ++pos2)
		{
			pCompetence = (TCompetence*) (*pos2);
			pPreconditions = pCompetence->GetPreconditions();

			// go through the preconditions of the module

			for(pos3=pPreconditions.begin(); pos3 != pPreconditions.end(); ++pos3)
			{
				pSource = (TProposition*) (*pos3);
				
				// is it a successor
				if (pSource->IsIdentic(pDest))
				{
					AddSuccessor(pSource, pDest);
					pCompetence->AddPredecessor(pDest, pSource);
					bExcitation = TRUE;
				}
				else if (pSource->IsInvers(pDest))
				{
					AddConflictor(pSource, pDest);
					bInhibition = TRUE;
				}
			}
		}

		// add goals, protected goals
		for(pos4=pGoalList.begin(); pos4 != pGoalList.end(); ++pos4)

		{
			pGoal = (TGoal*) (*pos4);;
			pSource = pGoal->GetCondition();
			if (pSource->IsIdentic(pDest))
			{
				AddGoalLink(pSource, pDest);
				bExcitation = TRUE;
			}
			else if (pSource->IsInvers(pDest))
			{
				AddProtectedGoalLink(pSource, pDest);
				bInhibition = TRUE;
			}
		}


	}
}




double TCompetence::SetToNewActivation() 
{ 
	m_dTransferedActivation = Transfer(m_dSpreadingActivation);
	return (SetActivation(m_dSpreadingActivation));
}



/*!

\brief checks if this module if it can be executed
\return 
\date 02.2014

\param  activation threshold


\details  checks if this module if it can be executed

\sa 
*/


bool TCompetence::CheckForExecution(double dThreshold)
{
	double	activity;

	activity = GetActivationAndExecutability();
	if (activity > dThreshold)
	{
		
		ExecuteModule(activity);

		return TRUE;
	}
	else
		// module not executed
		return FALSE;
}



/*!

\brief
\return
\date 02.2014

\param activation*executability of the module
\todo
\details it executes the action associated with this module
\sa
*/




void TCompetence::ExecuteModule(double activation)
{
	// execute action
	m_pAction->PerformAction(activation);
	m_bExecuted = TRUE;
}


/*!

\brief parses a rule
\return returns true if the parsing went fine
\date 02.2014

\param the string containing the rule, the action factory and the perception factory
\todo check whether in the rule preconditions we are really using numbers (for what?)
\details

parses a rule string like
if    prop1 1.0
and not prop2 1.0
then  action1
effects
prop3 1.0 
and not prop4 0.9

\sa
*/


bool TCompetence::ParseRule(string rule, TAct* pActionFactory, TPercept* pPerceptionFactory)
{
	TParseString	parse(rule);
	string			temp;
	string message;
	bool				bReady;
	double			value;
	bool				bNot;
	TProposition*	pProposition;
	TWorldState*	pState;
	bool				bFirstConjunct;

	// read the if
	if (!parse.ParseString(temp))
	{		
		char *buf="";
		int bufval=sprintf(buf,"No rule detected ! Rule ignored \r\n \r\n %s", rule);
		SEND2LOG(buf);
		return FALSE;
	}
	if (temp.compare(STRING_IF) != 0)
	{
		char *buf="";
		int bufval=sprintf(buf,"No if detected ! Rule ignored \r\n\r\n %s", rule);
		SEND2LOG(buf);
		return FALSE;
	}

	// read preconditions
	bReady = FALSE;
	bFirstConjunct = TRUE;
	while (!bReady)
	{
		// not or name
		bNot = FALSE;
		parse.ParseString(temp);
		if (temp.compare(STRING_THEN_SKILL) == 0)
			bReady = TRUE;
		else if (bFirstConjunct || (temp.compare(STRING_AND) == 0))
		{
			// it is another precondition
			if (!bFirstConjunct)
				// overread and
				parse.ParseString(temp);
			
			if (temp.compare(STRING_NOT) == 0)
			{
				bNot = TRUE;
				parse.ParseString(temp);
			}
			
			pState = GetState(temp, pPerceptionFactory);
			if (pState == NULL)
				return FALSE;

			// value 
			value = parse.ParseDouble();
			if (parse.HasErrorOccured())
				// value not written, so default is 1.0
				value = 1.0;

			// create the new precondition
			pProposition = new TProposition(this, pState, value, bNot);
			m_pPreconditions.push_back(pProposition);

			bFirstConjunct = FALSE;
			pState->SetUsed();
		}
		else
			return FALSE;
	}

	// read action (the 'then' has been read above
	parse.ParseString(temp);
	m_pAction = pActionFactory->GetAction(temp);
	if (m_pAction == NULL)
	{
		char *buf="";
		int bufval=sprintf(buf,"Action '%s' not existing! \r\n Rule ignored.", rule.c_str());
		SEND2LOG(buf);
		return FALSE;
	}
	SetName(temp);

	// read effects
	parse.ParseString(temp);
	if (temp.compare(STRING_EFFECT) != 0)
	{

		char *buf="";
		int bufval=sprintf(buf,"No effect detected ! Rule ignored \r\n\r\n %s", rule.c_str());
		SEND2LOG(buf);
		return FALSE;
	}

	bReady = FALSE;
	bFirstConjunct = TRUE;
	while (!bReady)
	{
		// not or name
		bNot = FALSE;
		parse.ParseString(temp);
		if (temp.compare(STRING_END_IF) == 0)
			bReady = TRUE;
		else if (bFirstConjunct || (temp.compare(STRING_AND) == 0))
		{
			// it is another effect
			if (!bFirstConjunct)
				// overread and
				parse.ParseString(temp);

			if (temp.compare(STRING_NOT) == 0)
			{
				bNot = TRUE;
				parse.ParseString(temp);
			}
			
			pState = GetState(temp, pPerceptionFactory);
			if (pState == NULL)
				return FALSE;

			value = parse.ParseDouble();
			if (parse.HasErrorOccured())
				// value not written, so default is 1.0
				value = 1.0;

			// create the new effect
			pProposition = new TProposition(this, pState, value, bNot);
			m_pEffects.push_back(pProposition);

			bFirstConjunct = FALSE;
		}
	}
	return TRUE;
}



/*!

\brief converts the connections of the module to a rule string
\return the string containing the connections of the module
\date 02.2014

\param 
\todo
\details
\sa
*/



string TCompetence::ToString()
{
	string				ret;
	list<TProposition*>::iterator	pos;

	TProposition*		pSource;

	ret= sprintf("%s ",  STRING_IF);

	// add all preconditions
	int i = 0;
	for(pos=m_pPreconditions.begin(); pos != m_pPreconditions.end(); ++pos)
	{
		pSource = (*pos);
		if (i == 0)
			ret += "     ";
		else
		{
			ret += " ";
			ret += STRING_AND;
			ret += " ";
		}
		ret += pSource->ToString();
		i++;
	}
	
	// add the action
	ret += m_pAction->GetName();

	// add the effects

	return ret;
}





/*!

\brief instantiates the condition and effect variables with constants

\return
\date 02.2014

\param  

\details  
\sa
*/

bool TCompetence::Instantiate(list<string> objects, list<string> variables, list<string> conditions
									, string action, list<string> effects, TPercept* pPercept)
{
	list<string>::iterator pos;
	TParseString	temp;
	bool				bNegated;
	TProposition*	pProposition;
	TWorldState*	pState;

	for(pos=conditions.begin(); pos!=conditions.end();++pos)
	{
		temp = (TParseString) (*pos);

		// create a new proposition
		bNegated = DecodeProposition(temp, objects, variables);
		pState = GetState(temp, pPercept);
		if (pState == NULL)
			return FALSE;

		pProposition = new TProposition(this, pState, 1.0, bNegated);
		m_pPreconditions.push_back(pProposition);

		pState->SetUsed();
	}

	// action
	temp = (TParseString) action;
	DecodeProposition(temp, objects, variables);
	SetName(temp);

	// effects
	for(pos=effects.begin(); pos!=effects.end();++pos)
	{
	temp = (TParseString) (*pos);

		// create a new proposition
		bNegated = DecodeProposition(temp, objects, variables);
		pState = GetState(temp, pPercept);
		if (pState == NULL)
			return FALSE;

		pProposition = new TProposition(this, pState, 1.0, bNegated);
		m_pEffects.push_back(pProposition);

	}
	
	return TRUE;
}
									

/*!

\brief this function checks whether a proposition is negated or not

\return it returns TRUE if the proposition is negated
\date 02.2014

\param

\details a negated proposition is encoded by a '0' at the end of the name,
				 positive propositions are encoded by a '1'

\todo check if this function is really necessary, or if it should be placed with parser functions				 
\sa
*/

bool TCompetence::DecodeNegated(string &str)
{
		char	flag = str.at(str.size() - 1);
		
	str = str.substr(0,str.size()-1);
	if (flag == '0')
		return TRUE;
	else
		return FALSE;
}




/*!

\brief instantiates the condition and effect variables with constants

\return bool saying if it is true or negated 
\date 02.2014

\param  reference to the proposition's name
		the list of propositions and 
		the list of variables
\todo the list of variables is almost not used, see if it can be simplified

\details
\sa
*/
bool TCompetence::DecodeProposition(TParseString &prop, list<string> objs, list<string> vars)
{
	bool		bNegated;
	list<string>::iterator pos1;
	list<string>::iterator pos2;

	string	src;
	string	dest;

	ASSERT(objs.size()==vars.size());  

	bNegated = DecodeNegated(prop);
	
	prop.Replace('(', '-');
	prop.Replace(',', '-');

	// replace each occurence of the variable by the constant
	pos2=vars.begin();
	for(pos1=objs.begin(); pos1 !=objs.end();++pos1)
	{
		dest = (*pos1);
		src = (*pos2);
		++pos2;

		while(prop.find(src) != prop.npos)
			prop.replace(prop.find(src),prop.find(src)+src.size(),dest); 
		//! \todo ensure this is properly doing what it should
	}
	
	return bNegated;
}



/*!

\brief 
\return the world state corresponding to the proposition
\date 02.2014

\param the name of the proposition
		a pointer to the preception factory
\todo 
\details
\sa
*/

TWorldState* TCompetence::GetState(string prop, TPercept* pPerceptionFactory)
{
	string			message;
	TWorldState*	pState;
	
	pState = pPerceptionFactory->GetPerception(prop);
	if (pState == NULL)
	{

		char *buf=new char[256];
		int bufval=sprintf(buf,"Proposition '%s' not existing! \r\n Rule ignored.", prop.c_str());
		SEND2LOG(buf);
	}
	return pState;
}

//! \todo check if we want to recycle the function SortLinks, which is commented. Otherwise, remove it
/*
void	TCompetence::SortLinks()
{
	int					i;
	POSITION				pos0;
	POSITION				pos1;
	POSITION				pos2;
	POSITION				pos3;
	TEnergySpreader	*pComp1;
	TEnergySpreader	*pComp2;
	TConnection			*pCon1;
	TConnection			*pCon2;

	// just neccessary for successors and conflictors
	for (i = SUCCESSOR_CONNECTION; i < NO_OF_CONNECTION_TYPES; i++)
	{
		pos1 = m_pLinks[i]->GetHeadPosition();
		while (pos1 != NULL)
		{
			pos0 = pos1;
			pCon1 = (TConnection*) m_pLinks[i]->GetNext(pos1);
			pComp1 = pCon1->GetSourceProp()->GetModule();
			pos2 = pos1;
			while (pos2 != NULL)
			{
				pos3 = pos2;
				pCon2 = (TConnection*) m_pLinks[i]->GetNext(pos2);
				pComp2 = pCon2->GetSourceProp()->GetModule();
				if (pComp1 == pComp2)
				{
					// we have the same source module, so remove link from list
					// at set it behind the other
					m_pLinks[i]->RemoveAt(pos3);
					m_pLinks[i]->InsertAfter(pos0, pCon2);
				}
			}
		}
	}
}

*/