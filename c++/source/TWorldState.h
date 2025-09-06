#pragma once

#include "Timepath_header.h"
#include "TEnergySpreader.h"

class TProposition;
class TCallbackConverter;


/*!
\brief A class containing the world as represented by an agent
\details
A class to contain the world as represented by an agent in the form of propositions. Each TWorldState
instance contains a proposition with a fuzzy truth value, which stand as propositions describing the state of the world
*/


class TWorldState : public TEnergySpreader 
{
	public:
		TWorldState(string name, int index, TObject* pObj, PCallBackPerception pCall);
#ifdef UNITYAGENT
	
		//for perceptions: (do NOT use this call for resources)
		TWorldState(string name, int index, TCallbackConverter * pObj);
		virtual void ResetCallCounter();
#endif
		virtual			~TWorldState();
		void			Initialize();


		double			GetTruthValue();
		virtual double	GetMainActivation() const;

		
		bool			IsUsed(void) const { return m_bUsed; };
		void			SetUsed(bool used = TRUE) { m_bUsed = used; };
		void			SetFuzzy(bool bFuzzy = TRUE) { m_bFuzzy = bFuzzy; };
		bool			GetFuzzy() const { return m_bFuzzy; };
		//void			SetFuzzyRound(double dRound) { m_dFuzzyRound = dRound; };
		double			GetFuzzyRound() const { return m_dFuzzyRound; };
		TProposition*	GetProposition() { return m_pProposition; };

	private:
		//! the proposition describing the state of the world
		TProposition*	m_pProposition;

		//! true, if the current World State is being used by current network
		bool			m_bUsed;				
#if UNITYAGENT==0
		//! the object containing the method used to perceive the state of the world
		TObject*					m_pObject;
		//! the method to perceive the state of the world
		PCallBackPerception	m_pTruthFunction;
#else  
		//! the method to perceive the state of the world
		TCallbackConverter* m_pObject;
#endif
		//! should we set to i0.0 the fuzzy values under a certain threshold ?
		bool			m_bFuzzy;
		//!  the threshold value  under which we set the fuzzy values at 0.0
		double			m_dFuzzyRound;

		TWorldState &operator=(const TWorldState &a);
};

