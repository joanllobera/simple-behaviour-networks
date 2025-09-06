
#include "NetworkHeader.h"
#include "TAction.h"
#include "TAct.h"


TAct::TAct()
{

		
}

TAct::~TAct()
{

	m_pActions.clear();

}


/*!

\brief searches for the Action in the array of Action names.
\return the appropriate action
\date 2004

\param the name of the action searched
*/

TAction* TAct::GetAction(string name)
{	

		
	list<TAction*>::iterator	pos; 
	string		strName = name;
	TAction*		pAction;


	if (strName.find("action") == 0)
		strName = "action";

	for(pos=m_pActions.begin(); pos != m_pActions.end(); ++pos){
		if((*pos)->GetName().compare(strName)==0){
			pAction=(*pos); 
			return pAction;	
		}
	}

	return NULL;
}

/*-----------------------------------------------------------------------------
Function: void TAct::AddAction(LPCTSTR name, CObject* pObj, PCallBackAction pAction)
Date    : 22.02.99
-------------------------------------------------------------------------------
Parameter: 
Returnval: 
Description: 
-----------------------------------------------------------------------------*/


/*!

\brief adds an action to this action factory
\return number of actions in factory
\date 1999

\param name of the action
			  pointer to the object containing the action function
			  pointer to the action function
*/


int TAct::AddAction(string name, TObject* pObj, PCallBackAction pCall)
{
	TAction* pAction;		

	pAction = new TAction(name,(int) m_pActions.size(), pObj, pCall);

	m_pActions.push_back(pAction);
	return ((int) m_pActions.size());


}


int TAct::AddAction(string name, TCallbackConverter* pObj)
{
	TAction* pAction;
	pAction = new TAction(name, (int)m_pActions.size(), pObj);
	m_pActions.push_back(pAction);
	return ((int)m_pActions.size());

}

void TAct::ResetCallbackCounters(){

	list<TAction*>::iterator pos;
	for (pos = m_pActions.begin(); pos != m_pActions.end(); pos++){
		(*pos)->ResetCallCounter();
	}
}
