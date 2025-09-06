#pragma once


#include "Timepath_header.h"

#include "TGoal.h"

class TDorer2Network;



/*!

 \brief represents a goal of an agent with relevance conditions, this is, having TDorer2Competence modules
 \date 01.98

 \sa TDorer2Competence TGoal 
*/

class TDorer2Goal : public TGoal
{
	public:
		TDorer2Goal();
		virtual ~TDorer2Goal();


		virtual bool		ParseGoal(string goal, TPercept* pPerceptionFactory);

		virtual double		GetActivation();

		//! gives the relevance value of the goal
		double				GetRelevance() const { return m_dRelevance; };
		void					CalculateSituationRelevance();

	protected:

		bool					AddRelevanceCondition(string name, bool bNegated, TPercept* pPerceptionFactory);


		double			m_dRelevance;

		list<TProposition*>	m_pRelevanceList;

#ifdef	DORERGOALSHAVEEFFECTS
		list<TProposition*>	m_pEffectsList;
#endif

};	

