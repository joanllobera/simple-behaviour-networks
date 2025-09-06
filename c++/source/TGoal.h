#pragma once

#include "Timepath_header.h"
#include "TEnergySpreader.h"

class TProposition;
class TPercept;
class TParseString;





/*!

 \brief represents a simple goal of an agent. See also TDorer2Goal
 \date 01.98
 \details It is a very simple goal with no relevance condition, only a condition to be achieved. It works with TCompetence . For a more sophisticated goal, see TDorer2Goal

 \sa TCompetence TDorer2Goal 
*/



class TGoal : public TEnergySpreader  
{

	
	public:
		TGoal(void);
		TGoal(int index);
		virtual ~TGoal();

		


		virtual bool		ParseGoal(string goal, TPercept* pPerceptionFactory);

		//! return the importance of the goal
		virtual double		GetActivation() { return m_dImportance; };
	
		//! \todo this pure virtual function is only introduced in order the casting works well when using goals that are TDorer2Goal 
		virtual void					CalculateSituationRelevance(){};

		//! return the condition of the goal
		TProposition*		GetCondition(){ return m_pCondition; };

	protected:
		bool				ParseGoalName(TParseString& parse, TPercept* pPerceptionFactory);
		void				SetImportance(double imp);

		double			m_dImportance;
		TProposition*	m_pCondition;
};	

