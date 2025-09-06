
#pragma once

#include "Timepath_header.h" 

class TWorldState;
class TScreenParamsNetwork;
class TCallbackConverter;



/*!

\brief It handles all the perceptions of an agent 
\date 02.2014

\details It handles all the perceptions of an agent (it is in fact a Tperception Factory)
\sa TAct
*/

class TPercept 
{
	public:
		TPercept();
		virtual ~TPercept();

		void InitializeAll();

		int AddPerception(string name, TObject* pObj, PCallBackPerception pCallback);

#ifdef UNITYAGENT
		int	AddPerception(string name, TCallbackConverter* pObj);
		void ResetCallbackCounters();
#endif
		void CalculateAllActivations();
		TWorldState* GetPerception(string name);


		list<TWorldState*> GetAllPerceptions(){ return m_pFactoryObjects; };
	protected:

		list<TWorldState*>			m_pFactoryObjects;
};

