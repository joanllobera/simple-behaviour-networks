#include "NetworkHeader.h"
#include "TEnergySpreader.h"
#include "TWorldState.h"
#include "TProposition.h"

TProposition::TProposition()
{
	m_tWeigh = 0.0;
	m_bNegated = FALSE;
	m_pWorldState = NULL;
	m_pModule = NULL;
}

TProposition::~TProposition()
{
}


void TProposition::SetProposition(TEnergySpreader* module, TWorldState* state
											 , double weigh, bool negated)
{
	m_pModule = module;
	m_pWorldState = state;
	m_tWeigh = weigh;
	m_bNegated = negated;
}



/*!
\brief returns the fuzzy truth value of the proposition
\return the fuzzy truth value of the proposition
\date 1998
\details value of the proposition, that is the truth of the proposition or 1-truth 
			  for negated propositions
*/

double TProposition::GetValue() const
{
	if (m_bNegated)
		return (1.0 - m_pWorldState->GetMainActivation());		
	else
		return m_pWorldState->GetMainActivation();
}

/*!
\brief sets the fuzzy truth value of the proposition
\param the fuzzy truth value of the proposition
\date 1998
*/


void TProposition::SetValue(double val)
{
	m_pModule->SetActivation(val);		
}



string TProposition::ToString()
{
	string ret;
	string not;

	if (m_bNegated)
		not = "not ";

	char buffer[50];
	int a= sprintf_s(buffer,"%s  %1.3f", m_pWorldState->GetName().c_str(), m_tWeigh); 
	ret = buffer;

	return not + ret;
}


/*!

\brief called to set the world state according to this proposition
\date 1998
\details  called to set the world state according to this proposition which means
				 if this proposition is negated, the world state gets 0.0 else it gets 1.0
*/

void TProposition::SetWorldState()
{
	if (m_bNegated)
		m_pWorldState->SetActivation(0.0);
	else
		m_pWorldState->SetActivation(1.0);
}
