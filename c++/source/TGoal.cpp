
#include "NetworkHeader.h"
#include "TProposition.h"

#include "TWorldState.h"
#include "TPercept.h"
#include "TParseString.h"
#include "TGoal.h"

TGoal::TGoal() 
{
	m_pCondition = NULL;
	SetImportance(1.0);
}

TGoal::TGoal(int index) 
{
	m_pCondition = NULL;
	SetIndex(index);
	SetImportance(1.0);
}

TGoal::~TGoal()
{
	if (m_pCondition != NULL)
		delete m_pCondition;
}



/*!

 \brief parses a goalstring and sets the parameters of the goal
 \date 04.98
 \param string containing the goal (syntax is: goal name importance endgoal), 
		pPerceptionFactory where all the perceptions are listed
\return TRUE if no error occurred		

 
*/


bool TGoal::ParseGoal(string goal, TPercept* pPerceptionFactory)
{	
	TParseString	parse(goal);
	string			temp;
	string			message;


	// read the goal
	if (!parse.ParseString(temp))
	{
		char *buf="";
		int bufval=sprintf(buf,"No goal detected ! Goal ignored \r\n \r\n %s", goal);
		SEND2LOG(buf);
		return FALSE;
	}
	
	return ParseGoalName(parse, pPerceptionFactory);
}


/*!

 \brief  parses the name of the goal and its importance
 \date 04.98
 \param pointer to a string containing the goalstring
\return TRUE if no error occurred		
\sa TGoal::ParseGoal

 
*/

bool TGoal::ParseGoalName(TParseString& parse, TPercept* pPerceptionFactory)
{
	string			name;
	string			message;

	double			value;
	bool				bNegated = FALSE;
	TWorldState*	pState;

	// read name
	if (!parse.ParseString(name))
	{

		char *buf=new char[128];
		int bufval=sprintf(buf,"No name of the goal detected! goal ignored.\r\n\r\n %s", parse);
		SEND2LOG(buf);
		return FALSE;
	}

	else if (name.compare(STRING_NOT) == 0)
	{
		// it is a negated goal
		bNegated = TRUE;
		parse.ParseString(name);
	}

	// get the index of the proposition 'name'
	pState = pPerceptionFactory->GetPerception(name);
	if (pState == NULL)
	{
		char *buf=new char[128];
		int bufval=sprintf(buf,"Proposition '%s' not existing! \r\n Goal ignored.", name.c_str());
		SEND2LOG(buf);
		return FALSE;
	}

	// get the importance of the goal
	value = parse.ParseDouble();
	if (parse.HasErrorOccured())
		// value not written, so default is 1.0
		value = 1.0;

	// set the goal
	m_pCondition = new TProposition(this, pState, value, bNegated);
	pState->SetUsed();
	SetName(name);
	SetImportance(value);
	return TRUE;
}


/*!

 \brief sets the importance and the activation of the goal
 \date 04.98
 \param importance of the goal
\return  (void)


 
*/

void TGoal::SetImportance(double imp)
{
	m_dImportance = imp;
	SetActivation(imp);
}


