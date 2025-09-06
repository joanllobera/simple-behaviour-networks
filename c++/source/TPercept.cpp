

#include "Timepath_header.h"
#include "NetworkHeader.h"
#include "TProposition.h"
#include "TWorldState.h"
#include "TPercept.h"

TPercept::TPercept()
{

}


TPercept::~TPercept()
{
	m_pFactoryObjects.clear();
}



/*!

\brief initializes all world states (e.g. as being not used)
\date 02.2014

\param
\details
\sa
*/

void TPercept::InitializeAll()
{

list<TWorldState*>::iterator pos;

for (pos = m_pFactoryObjects.begin();pos != m_pFactoryObjects.end(); pos ++){
	(*pos)->Initialize();
	}	
}


/*!

\brief  creates and adds a new TWorld state for this proposition
\return index of the added Proposition
\date 02.2014

\param name of the Proposition
pointer to the object that contains the truth value function
pointer to the truth value function to call
\todo
\details  creates and adds a new TWorld state for this proposition
\sa
*/

int TPercept::AddPerception(string name, TObject* pObj, PCallBackPerception pCallback)
{
	TWorldState* pState;
		pState = new TWorldState(name, m_pFactoryObjects.size(), pObj, pCallback);
		m_pFactoryObjects.push_back(pState);
		return m_pFactoryObjects.size();
}

#ifdef UNITYAGENT
/*!

\brief  creates and adds a new TWorld state for this proposition
\return index of the added Proposition
\date 02.2014

\param name of the Proposition
			  pointer to the object that contains the truth value function and callback
\todo
\details  creates and adds a new TWorld state for this proposition
\sa
*/

int TPercept::AddPerception(string name, TCallbackConverter* pObj)
{
	TWorldState* pState;
	pState = new TWorldState(name, m_pFactoryObjects.size(), pObj);
	m_pFactoryObjects.push_back(pState);
	return m_pFactoryObjects.size();
}

void TPercept::ResetCallbackCounters(){

	list<TWorldState*>::iterator pos;
	for (pos = m_pFactoryObjects.begin(); pos != m_pFactoryObjects.end(); pos++){
		(*pos)->ResetCallCounter();
	}
}

#endif


/*!

\brief calculates the activation of all percepts
\date 02.2014

\details calculates the activation of all percepts
\sa
*/

void TPercept::CalculateAllActivations()
{

list<TWorldState*>::iterator pos;

for (pos = m_pFactoryObjects.begin();pos != m_pFactoryObjects.end(); pos ++){
	if ((*pos)->IsUsed())
			(*pos)->GetTruthValue();
	}	

}


/*!

\brief searches for the perception in the list of perception names
\return pointer to the Worldstate (Perception) NULL, if name does not exist
\date 02.2014

\param name of the perception
\todo think if contained in a container different from a list this function would not accelerate the system
\details searches for the perception in the list of perception names
\sa
*/

TWorldState* TPercept::GetPerception(string name)
{	

	list<TWorldState*>::iterator	pos; 
	TWorldState*					pState;
	
	for(pos=m_pFactoryObjects.begin(); pos != m_pFactoryObjects.end(); ++pos){
		if((*pos)->GetName().compare(name)==0){
			pState=(*pos); 
			return pState;	
		}
	}
	return NULL;
}

