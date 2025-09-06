
#include "NetworkHeader.h"
#include "TProposition.h"
#include "TPercept.h"
#include "TParseString.h"

#include "TWorldState.h"
#include "TDorer2Network.h"
#include "TNetworkParams.h"
#include "TDorer2Goal.h"

TDorer2Goal::TDorer2Goal() 
{
	this->m_dRelevance = 1.0;
}


TDorer2Goal::~TDorer2Goal()
{
	
	m_pRelevanceList.clear();

#ifdef DORERGOALSHAVEEFFECTS
	m_pEffectsList.clear();
#endif
}





/*!

 \brief parses a goalstring and sets the parameters of the goal
 \date 04.98
 \param string containing the goal (syntax is: goal relevance_condition then goal_condition importance endgoal), 
		pPerceptionFactory where all the perceptions are listed
\return TRUE if no error occurred		

 
*/
bool TDorer2Goal::ParseGoal(string goal, TPercept* pPerceptionFactory)
{	
	TParseString	parse(goal);
	string			temp;
	//string			message;
	bool				bReady;
	double			value;
	bool				bNot;
	TProposition*	pProposition;
	TWorldState*	pState;
	bool				bFirstConjunct;

	// read the goal
	if (!parse.ParseString(temp))
	{

		char *buf=new char[128];
		int bufferval=sprintf(buf,"No goal detected ! Goal ignored \r\n \r\n %s", goal);
		SEND2LOG(buf);
			return FALSE;
	}

	if (parse.find(STRING_THEN_GOAL) == parse.npos)
		// it is an oldfashioned goal with no relevance conditions (and no then)
		return ParseGoalName(parse, pPerceptionFactory);

	// read preconditions
	bReady = FALSE;
	bFirstConjunct = TRUE;
	while (!bReady)
	{
		// not or name
		bNot = FALSE;
		parse.ParseString(temp);
		if (temp.compare(STRING_THEN_GOAL) == 0)
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
			pState = pPerceptionFactory->GetPerception(temp);
			if (pState == NULL)
			{
				char *buf=new char[128];
				int bufferval=sprintf(buf,"Proposition '%s' not existing! \r\n Goal ignored.", temp.c_str());
				SEND2LOG(buf);
				return FALSE;
			}
			// value 
			value = parse.ParseDouble();
			if (parse.HasErrorOccured())
				// value not written, so default is 1.0
				value = 1.0;

			// create the new precondition
			pProposition = new TProposition(this, pState, value, bNot);
			m_pRelevanceList.push_back(pProposition);
			bFirstConjunct = FALSE;
			pState->SetUsed();
		}
		else
			return FALSE;
	}

	if (!ParseGoalName(parse, pPerceptionFactory))
		return FALSE;


#ifdef DORERGOALSHAVEEFFECTS
//WHY WOULD A GOAL HAVE EFFECTS?!?!?

	// read effects
	
	parse.ParseString(temp);
	if (temp.compare(STRING_EFFECT) != 0)
	{
		char *buffer="";
		int bufferval=sprintf(buffer,"No effect detected ! Rule ignored \r\n\r\n %s", goal);
		SEND2LOG(buffer);
		return FALSE;
	}
#endif


	bReady = FALSE;
	bFirstConjunct = TRUE;
	while (!bReady)
	{
		// not or name
		bNot = FALSE;
		parse.ParseString(temp);

#if 0
		string test = temp;
		bool b = test.compare(temp);
		string datatest = "a ) test.compare of " + test + "and " + temp + "gives: ";
		if (b)
			datatest = datatest + "true";
		else
			datatest = datatest + "false";

		bool b2 = test.compare(STRING_END_GOAL);
		datatest = datatest + "\n b) test.compare of " + test + "and " + STRING_END_GOAL + "gives: ";
		if (b2)
			datatest = datatest + "true";
		else
			datatest = datatest + "false";



		char *buffer = new char[128];
		int bufferval = sprintf(buffer, datatest );
		SEND2LOG(buffer);
#endif


		if (temp.compare(STRING_END_GOAL) == 0)
			bReady = TRUE;

		else if (bFirstConjunct || (temp.compare(STRING_AND) == 0))
		{
#ifdef DORERGOALSHAVEEFFECTS
			// it is another effect
			if (!bFirstConjunct)
				// overread and
				parse.ParseString(temp);

			if (temp.compare(STRING_NOT) == 0)
			{
				bNot = TRUE;
				parse.ParseString(temp);
			}
			pState = pPerceptionFactory->GetPerception(temp);
			if (pState == NULL)
			{
				char *buffer="";
				int bufferval=sprintf(buffer,"Proposition '%s' not existing! \r\n Rule ignored.", temp);
				SEND2LOG(buffer);
				return FALSE;
			}
			// value 
			value = parse.ParseDouble();
			if (parse.HasErrorOccured())
				// value not written, so default is 1.0
				value = 1.0;

			// create the new effect
			pProposition = new TProposition(this, pState, value, bNot);
			m_pEffectsList.push_back(pProposition);

			bFirstConjunct = FALSE;
#endif //#ifdef DORERGOALSHAVEEFFECTS

#if WARNINGIFNOTDIRECTENDGOAL
	bReady = TRUE;
	char *buffer=new char[128];
	int bufferval=sprintf(buffer,"Either the goal being parsed has an effect, either you forgot endgoal, or wrote something wrong.\n ");
	SEND2LOG(buffer);
	//return FALSE;
#endif
		}
	}
	return TRUE;
}


/*!

 \brief updates the relevance of this goal.
 \date 11.98
\detail updates the relevance of this goal. The relevance is a function
				 of the fullfilment of the relevance conditions of the goal
\return (void)		

*/

void TDorer2Goal::CalculateSituationRelevance()
{
	list<TProposition*>::iterator pos;
	TProposition*	pProp;
	double value;

	m_dRelevance = 1.0;
	pos=m_pRelevanceList.begin();
	for(pos=m_pRelevanceList.begin();pos!=m_pRelevanceList.end();++pos)
	{
		pProp =  (*pos);
		value= pProp->GetValue();

#if FUZZY_MIN_AND
		if (value < m_dRelevance)
			m_dRelevance = value;
#else 
		m_dRelevance *= value;
#endif


	}
}


/*!

 \brief adds a relevance condition to the condition list
 \date 02.99
 \param name of the relevance condition 
			  flag for state of negation

\return TRUE if it was done without problems	

*/

bool TDorer2Goal::AddRelevanceCondition(string name, bool bNegated, TPercept* pPerceptionFactory)
{


	//string			message;
	string			strRelevance = name;

	TProposition*	pProposition;
	TWorldState*	pState;

	// lookup relevance proposition (need not be there)
	if (strRelevance.length()!=0)
	{
		pState = pPerceptionFactory->GetPerception(strRelevance);
		if (pState == NULL)
		{

			char *buf="";
			int bufval=sprintf(buf,"Proposition '%s' not existing! \r\n Goal ignored.", strRelevance.c_str());
			SEND2LOG(buf);
			return FALSE;
		}
		// create the new relevance condition
		pProposition = new TProposition(this, pState, 1.0, bNegated);
		m_pRelevanceList.push_back(pProposition);
		pState->SetUsed();
	}
	
	return TRUE;
}




/*!

 \brief gets the activation
 \date 11.99

\return the value of the goal as function of relevance and importance

*/


double TDorer2Goal::GetActivation()
{
		// standard value function is product
		return m_dImportance * m_dRelevance;
}