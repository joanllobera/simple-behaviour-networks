#pragma once


#include "Timepath_header.h" 

class TWorldState;
class TEnergySpreader;

/*!

\brief The basis of preconditions, relevance conditions and effects 
\date 02.2014
\details The basis of preconditions, relevance conditions and effects
\sa TWorldState TEnergySpreader
*/


class TProposition 
{
	public:
		TProposition();
		TProposition(TEnergySpreader* module, TWorldState* state, double value, bool neg) 
						{ SetProposition(module, state, value, neg); };
		virtual ~TProposition();

		// set and gets
		void		SetProposition(TEnergySpreader* module, TWorldState* state, double value
										, bool negated = FALSE);
		TEnergySpreader* GetModule() { return m_pModule; };
		void		SetWorldState();
		TWorldState* GetWorldState() { return m_pWorldState; };
		void		SetValue(double val);
		double	GetValue() const;
		double	GetWeigh() { return m_tWeigh; };
		bool		IsNegated() const { return m_bNegated; };
		bool		IsSame(TProposition* prop) const { return (m_pWorldState == prop->GetWorldState()); };
		bool		IsIdentic(TProposition* prop) const { return (IsSame(prop) && (m_bNegated == prop->IsNegated())); };
		bool		IsInvers(TProposition* prop) const { return (IsSame(prop) && (m_bNegated != prop->IsNegated())); };
		string	ToString();

	private:
		//! The module that contains the proposition
		TEnergySpreader*	m_pModule;	
		//! pointer to the object containing the value of the prop
		TWorldState*		m_pWorldState; 
		//! for expectation or importance
		double				m_tWeigh;	
		//! true for proposition with 'not'
		bool					m_bNegated;		
};

