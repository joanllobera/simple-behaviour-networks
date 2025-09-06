
#pragma once

#include "NetworkHeader.h"
#include "TParseString.h"
#include "TProposition.h"
#include "TWorldState.h"
#include "TResourceProposition.h"
#include "TResource.h"
#include "TResourceFactory.h"
#include "TGoal.h"
#include "TAction.h"
#include "TAct.h"
#include "TPercept.h"
#include "TNetworkParams.h"
#include "TDorer2GoalTrack.h"
#include "TDorer2Connection.h"
#include "TDorer2Competence.h"

#include <math.h>

TDorer2Competence::TDorer2Competence(TNetworkParams* params, string name, int index, int iNoOfGoals)
:TCompetence(params, name, index)
{
	m_dSigmaCompetence = m_pNetworkParams->GetSigma();
	m_dGainCompetence = m_pNetworkParams->GetGain();
	
	#ifdef NO_GOALTRACKING
		m_pGoalTrack = new TDorer2NoGoalTrack(iNoOfGoals);
	#else
		m_pGoalTrack = new TDorer2GoalTrack(iNoOfGoals);
	#endif

		

	m_dSumTaup = 0.0;
}

TDorer2Competence::~TDorer2Competence()
{
	delete m_pGoalTrack;

	// remove all resource links
	m_pResourceLinks.clear();

	// remove all resource propositions
	m_pResources.clear();
}


/*!

\brief It adds a link to a situation proposition
\return
\date 02.2014

\param pointer to the proposition that is a precondition of this module
			  pointer to the destination proposition

\sa AddEnergySource
*/


void TDorer2Competence::AddPropositionLink(TProposition* source, TProposition* dest)
{
	// create a new connection
	TConnection* newConnection = new TDorer2SituationLink(m_pNetworkParams, source, dest);
	AddEnergySource(newConnection, source, SITUATION_CONNECTION);
}


/*!

\brief It adds a link to a goal proposition
\return
\date 02.2014

\param pointer to the proposition that is a precondition of this module
pointer to the destination proposition

\sa AddEnergySource
*/



void TDorer2Competence::AddGoalLink(TProposition* source, TProposition* dest)
{
	// create a new connection
	TConnection* newConnection = new TDorer2GoalLink(m_pNetworkParams, source, dest);
	AddEnergySource(newConnection, source, GOAL_CONNECTION);
}


/*!

\brief It adds a link to a PROTECTED goal proposition (when dealing with negated propositions)
\return
\date 02.2014

\param pointer to the proposition that is a precondition of this module
pointer to the destination proposition

\sa AddEnergySource
*/



void TDorer2Competence::AddProtectedGoalLink(TProposition* source, TProposition* dest)
{
	// create a new connection
	TConnection* newConnection = new TDorer2ProtectedGoalLink(m_pNetworkParams, source, dest);
	AddEnergySource(newConnection, source, PROTECTED_GOAL_CONNECTION);
}


/*!

\brief It adds a link to a successor
\return
\date 02.2014

\param pointer to the proposition that is a precondition of this module
pointer to the destination proposition

\sa AddEnergySource
*/

void TDorer2Competence::AddSuccessor(TProposition* source, TProposition* dest)
{
	// create a new connection
	TConnection* newConnection = new TDorer2SuccessorLink(m_pNetworkParams, source, dest);
	AddEnergySource(newConnection, source, SUCCESSOR_CONNECTION);
}




/*!

\brief It adds a link to a conflictor proposition
\return
\date 02.2014

\param 
pointer to the proposition (effect) of the passed module
pointer to the proposition (precondition) of this module

\sa AddEnergySource
*/



void TDorer2Competence::AddConflictor(TProposition* source, TProposition* dest)
{
	// add the module
	TConnection* newConnection = new TDorer2ConflictorLink(m_pNetworkParams, source, dest);
	AddEnergySource(newConnection, source, CONFLICTOR_CONNECTION);
}

/*!

\brief It adds a link to a resource
\return
\date 02.2014

\param

\sa AddEnergySource
*/
void TDorer2Competence::AddResourceLink(TProposition* source, TProposition* dest)
{
	// create a new connection
	TConnection* newConnection = new TResourceLink(m_pNetworkParams, source, dest);
	m_pResourceLinks.push_back((newConnection));
}


/*!

\brief 
\date 02.2014

\details first the old activation is decayed. Then it calculates the activation of this module. This is done by summing
				 up the energy of each incoming connection to this module
*/


void TDorer2Competence::CalculateActivation()
{
	double			dPreviousActivation;

	// previous activation
	dPreviousActivation = m_dMainActivation * m_pNetworkParams->GetBeta();

	// extern activation 
	CalculateExternActivation();

	// spreading activation
	CalculateSpreadingActivation();

	// and sum them up
	m_dSpreadingActivation = dPreviousActivation + m_pGoalTrack->GetSumActivation();
}



/*!

\brief  It receives the external activation from goals (and does the goaltracking)
\date 02.2014

*/


void TDorer2Competence::CalculateExternActivation()
{
	list<TConnection*>::iterator pos;

	TConnection*			pCon;
	int						i;
	double					activation;



	// goal connections
	for(pos=m_pLinks[GOAL_CONNECTION]->begin();pos!=m_pLinks[GOAL_CONNECTION]->end();++pos)
	{
		// add the activation of this kind of link
		activation = (*pos)->GetActivation();
		i = (*pos)->GetSourceModule()->GetIndex();
		m_pGoalTrack->SetActivation(i, activation);
	}

	// protected goal connections
	for(pos=m_pLinks[PROTECTED_GOAL_CONNECTION]->begin();pos!=m_pLinks[PROTECTED_GOAL_CONNECTION]->end();++pos)
	{
		// add the activation of this kind of link
		pCon = (TConnection*) (*pos);
		activation = (*pos)->GetActivation();
		i = pCon->GetSourceModule()->GetIndex();
		m_pGoalTrack->SetActivation(i, activation);
	}
}


/*!

\brief calculates the activation  gained by activation spreading among the modules
\return activation spread to this module by other modules. The influence of direct goals is counted  within this
\date 02.2014

*/


void TDorer2Competence::CalculateSpreadingActivation()
{

	
	list<TConnection*>::iterator	pos;
	int						i;
	int						iNoOfGoals;

	// successors
	iNoOfGoals = m_pGoalTrack->GetNoOfGoals();
	for(pos=m_pLinks[SUCCESSOR_CONNECTION]->begin();pos!=m_pLinks[SUCCESSOR_CONNECTION]->end();++pos)
	{
		TDorer2SuccessorLink* pCon = dynamic_cast<TDorer2SuccessorLink*>(*pos); //! \todo remove this dynamic cast
		for ( i = 0; i < iNoOfGoals; i++)
			m_pGoalTrack->SetActivation(i,  pCon->GetGoalActivation(i));
	}

	// conflictors
		for(pos=m_pLinks[CONFLICTOR_CONNECTION]->begin();pos!=m_pLinks[CONFLICTOR_CONNECTION]->end();++pos)
	{
		TDorer2ConflictorLink* pCon = dynamic_cast<TDorer2ConflictorLink*>(*pos);  //! \todo remove this dynamic cast
		for ( i = 0; i < iNoOfGoals; i++)
			m_pGoalTrack->SetActivation(i, pCon->GetGoalActivation(i));
	}

}



/*!

 \brief  calculates the executability of the module.
	\details 
 calculates the executability of the module. This is done by multiplying
				 the value of each precondition. The result is powered to 1/number of 
				 preconditions (if FUZZY_SQRT_AND is defined)
				 Alternatively, if FUZZY_MIN_AND is defined, we simply use the minimum

*/

void TDorer2Competence::CalculateExecutability()
{
	//POSITION		pos;
	list<TProposition*>::iterator pos;
	int			n;
	double		value;

	n = 0;
	m_dExecutability = 1.0;
	
	// step through each precondition
	for(pos=m_pPreconditions.begin();pos!=m_pPreconditions.end();++pos)
	{
		value = ((TProposition*) (*pos))->GetValue();
		#if FUZZY_MIN_AND_MODULE
			// this version uses min as fuzzy and	
			if (value < m_dExecutability)
				m_dExecutability = value;
		#else 
			// this version uses product as fuzzy min
			m_dExecutability *= value;
		#endif
		n++;

	}

	// the executability is powered by 1 / number of preconditions
	#ifdef FUZZY_SQRT_AND
		if (n > 2)
			m_dExecutability = pow(m_dExecutability, 1.0 / (double) n);
		else if (n == 2)
			m_dExecutability = sqrt(m_dExecutability);
	#endif
}


/*!

\brief applies the transfer function to the module activation
\return sigmoid of the activation of a module
\date 02.2014

\param  activation of a module
\details  calculates the transfer function of a modules activity (the activity
				 that is spread). Current transfer function is a sigmoid function
				 (see Goetz 1997)
*/


double TDorer2Competence::Transfer(double dVal)
{
#ifdef NO_TRANSFER_FUNCTION
	return dVal;
#else
	return 1.0 / (1.0 + exp(m_dGainCompetence * (m_dSigmaCompetence - dVal)));
#endif
}

/*!

\brief sets the new activation of this module (used after completion of the activation spreading)
\return the new activation value
\date 02.2014
*/


double TDorer2Competence::SetToNewActivation()
{
	m_pGoalTrack->SetToNewActivation();
	return	TCompetence::SetToNewActivation();
}

/*!

\brief It returns the product of activation and executability
\return  It returns the product of activation and executability
\date 02.2014

*/



double TDorer2Competence::GetActivationAndExecutability()
{
	// Dorer2 uses Product
	return m_dExecutability * GetActivation();
}

double TDorer2Competence::GetTransferedActivation(int iGoalIndex)
{
	return Transfer(m_pGoalTrack->GetActivation(iGoalIndex));
}


/*!

\brief It checks this module if it can be executed
\return true if it this module can be executed
\date 02.2014

\param  The activation threshold
*/

bool TDorer2Competence::CheckForExecution(double dThreshold)
{
	double				activity;
	list<TConnection*>::iterator pos;
	
	TResourceLink*		pResourceLink;
	TResource*			pResource;

#if UNITYAGENT
	if (this->GetAction()->GetCallCount() > 0){
		return false;

	}
#endif


	activity = GetActivationAndExecutability(); //STEP 2 ALGORITHM (second list in Dorer 2004)

	// check if activity surpasses all resources thresholds
	for(pos=m_pResourceLinks.begin();pos!=m_pResourceLinks.end();++pos)
	{

		double					amountNeeded;
		pResourceLink = (TResourceLink*) (*pos);
		pResource = (TResource*) pResourceLink->GetSourceModule();

		// check activity against resource threshold
		if (activity <= pResource->GetActivation())   //STEP 3a ALGORITHM
			return FALSE;

		// check if enough resource units are available
		amountNeeded = ((TResourceProposition*) pResourceLink->GetDestinationProp())->GetAmountNeeded();
		if (amountNeeded > pResource->GetAmountAvailable()) //STEP 3b ALGORITHM
			return FALSE;
	}

	// found no resource conflicts, execute the action
	if (activity < 0.01)
		// check if a module without using resources is at least executable
		return FALSE;
	else
		ExecuteModule(activity); //STEP 4a ALGORITHM

	// update resources
	list<TConnection*>::iterator pos2;
	for(pos2 = m_pResourceLinks.begin(); pos2 != m_pResourceLinks.end(); ++pos2)
	{

		double					amountNeeded;
		pResourceLink = (TResourceLink*) (*pos2);
		pResource = (TResource*) pResourceLink->GetSourceModule();
	
		// reduce resource
		amountNeeded = ((TResourceProposition*) pResourceLink->GetDestinationProp())->GetAmountNeeded();
		pResource->ReduceAmountAvailable(amountNeeded);			//STEP 4b ALGORITHM
		pResource->ResetActivationLevel(dThreshold);			
	}

	return TRUE;
}


/*!

\brief It releases the resources used by this module, if it has been executed
\date 02.2014
*/


void TDorer2Competence::UpdateResourceUsage()
{
	list<TConnection*>::iterator pos;
	TResourceLink*	pLink;
	double				iUsage;

	if (m_bExecuted)
	{
		// this module has been executed in the previous cycle, so release resources
		for(pos=m_pResourceLinks.begin();pos != m_pResourceLinks.end(); ++pos)
		{
			pLink = (TResourceLink*) (*pos);
			iUsage = ((TResourceProposition*) pLink->GetDestinationProp())->GetAmountNeeded();
			((TResource*) pLink->GetSourceModule())->IncreaseAmountAvailable(iUsage);
		}
		
		SetExecuted(FALSE);
	}
}



/*!

\brief
\return
\date 02.2014

\param the rule
		a pointer to the perception factory
		a pointer to the resource factory
\details It parses a rule to connect the Competence Module. The syntax expected is:
if    klausi 1.0
and not susi 1.0
then  heigaus
effects
rot 1.0
and not weg 0.9
using resource 1
endif

(In TDorer2Competence, compared to TCompetence, the resources have been added)


\sa TCompetence::ParseRule
*/



bool TDorer2Competence::ParseRule(string rule, TAct* pActionFactory, TPercept* pPerceptionFactory, TResourceFactory* pResourceFactory)
{
	TParseString	parse(rule);
	string			temp;
	string			message;
	bool				bReady;

	double			value;
	int				iAmount;
	bool				bNot;
	TProposition*	pProposition;
	bool				bFirstConjunct;
	bool				bUsing = FALSE;
	TWorldState*	pState;
	TResource*		pResource;
	TResourceProposition* pResourceProp;

	// read the if
	if (!parse.ParseString(temp))
	{
		char buf[256];
		int bufval=sprintf(buf, "No rule detected ! Rule ignored \r\n \r\n %s", rule.c_str());
		SEND2LOG(buf);
		return FALSE;
	}
	if (temp.compare(STRING_IF) != 0)
	{
		char buf[256];
		int bufval=sprintf(buf, "No if detected ! Rule ignored \r\n\r\n %s", rule.c_str());
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

	// read action (the 'then' has been read above)
	parse.ParseString(temp);
	m_pAction = pActionFactory->GetAction(temp);
	if (m_pAction == NULL)
	{
		char buf[256];
		int bufval=sprintf(buf, "Action '%s' not existing! \r\n Rule ignored.", temp.c_str());
		SEND2LOG(buf);
		return FALSE;
	}


	#if STORIES
		
		m_pAction->SetModule(this);
	#endif

	SetName(temp);

	// read effects
	parse.ParseString(temp);
	if (temp.compare(STRING_EFFECT) != 0)
	{
		char buf[256];
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
		if (!parse.ParseString(temp))
			return FALSE;

		if (temp.compare(STRING_USING) == 0)
		{
			bReady = TRUE;
			bUsing = TRUE;
		}
		else if (temp.compare(STRING_END_IF) == 0)
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

			// value 
			value = parse.ParseDouble();
			if (parse.HasErrorOccured())
				// value not written, so default is 1.0
				value = 1.0;

			// create the new effect
			pProposition = new TProposition(this, pState, value, bNot);
			pState->SetUsed();
			m_pEffects.push_back(pProposition);

			bFirstConjunct = FALSE;
		}
	}

	// parse ressources
	if (bUsing)
	{
		bReady = FALSE;
		bFirstConjunct = TRUE;
		while (!bReady)
		{
			if (!parse.ParseString(temp))
				return FALSE;

			if (temp.compare(STRING_END_IF) == 0)
				bReady = TRUE;
			else if (bFirstConjunct || (temp.compare(STRING_AND) == 0))
			{
				// it is another effect
				if (!bFirstConjunct)
					// overread and
					parse.ParseString(temp);

				// read the amount
				iAmount = parse.ParseInteger();
				if (parse.HasErrorOccured())
					// value not written, so default is 1
					iAmount = 1;

				// store the new resource
				pResource = (TResource*) GetState(temp, pResourceFactory);
				if (pResource == NULL)
					return FALSE;
				pResourceProp = new TResourceProposition(this, pResource, iAmount);
				m_pResources.push_back(pResourceProp);


				pResource->SetUsed();

				bFirstConjunct = FALSE;
			}
		}
	}
	
	return TRUE;
}


/*!

\brief It connects the competence module within the network
\param the list of competence modules, the list of goals
\date 02.2014
\details  It connects the competence module with the other competence modules and goals. It also creates a connection between each resource proposition and resource object


\sa TCompetence::ConnectCompetence
*/


void TDorer2Competence::ConnectCompetence(list<TCompetence*> pCompetenceList, list<TGoal*> pGoalList)
{

	list<TResourceProposition*>::iterator pos;
	TResourceProposition*	pProp;

	TCompetence::ConnectCompetence(pCompetenceList, pGoalList);

	// connect the module to the resources
	for(pos=m_pResources.begin();pos!=m_pResources.end();++pos)
	{
		pProp = (TResourceProposition*) (*pos);
		// add resource link
		TWorldState* TWS=pProp->GetWorldState();
		TProposition* TP=TWS->GetProposition();

		AddResourceLink(TP, pProp);
	}
}

