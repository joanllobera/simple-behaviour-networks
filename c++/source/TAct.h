#pragma once

#include "Timepath_header.h"
class TAction;

#if UNITYAGENT
class TCallbackConverter;
#endif
/*!

\brief It handles all the actions of an agent
\date 02.2014

\details It handles all the actions of an agent (it is in fact a Taction Factory)
\sa TAct
*/

class TAct 
{
	public:
		TAct();
		virtual ~TAct();

		// public methods
		int		AddAction(string name, TObject* pObj, PCallBackAction pAction);

#if UNITYAGENT
		int		AddAction(string name, TCallbackConverter* pObj);
		void ResetCallbackCounters();

#endif

		TAction*	GetAction(string name);
		list<TAction*>::iterator	GetFirstActionPosition() { return m_pActions.begin(); };
			
		int getNumberOfActions(){ return  (int) m_pActions.size(); };

	private:
		list<TAction*>		m_pActions; 
};
