#pragma once

#include "Timepath_header.h"



/*!

 \brief abstract base class for goaltracking in Dorer2 Networks
 \date 01.98
 
  
*/
class TDorer2BaseGoalTrack 
{
	public:
		virtual double	GetSumActivation(void) const = 0;
		virtual void	SetActivation(int i, double val) = 0; 
		virtual double	GetActivation(int i) = 0;
		virtual int		GetNoOfGoals(void) const = 0;
		virtual void	SetToNewActivation() = 0;	
};



/*!

 \brief encapsules the goal tracking in TDorer2Network
 \date 01.98
 
  
*/

class TDorer2GoalTrack : public TDorer2BaseGoalTrack
{
	public:
		TDorer2GoalTrack(int iNoOfGoals);
		virtual ~TDorer2GoalTrack();

		// public methods
		void			Initialize();

		// attribute functions		
		int			GetNoOfGoals(void) const { return m_iNoOfGoals; };
		double		GetSumActivation(void) const;
		void			SetActivation(int i, double val); 
		double		GetActivation(int i) { ASSERT ((i >= 0) && (i < m_iNoOfGoals)); 
															return m_pdActivation[i]; };
		void			SetToNewActivation();

	private:
		int				m_iNoOfGoals;			// no of goals in the goal track
		double*			m_pdActivation;		// procentual values of goal influences
		double*			m_pdNewActivation;	// procentual values of goal influences
};



/*!

 \brief class that satisfies the interface to goaltracking
				 but does not perform it 
 \date 01.98 

  
*/


class TDorer2NoGoalTrack : public TDorer2BaseGoalTrack
{
	public:
		TDorer2NoGoalTrack(int iNoOfGoals);

		// public methods
		void			Initialize();

		// attribute functions		
		int			GetNoOfGoals(void) const { return 1; };
		double		GetSumActivation(void) const;
		void			SetActivation(int i, double val); 
		double		GetActivation(int i) { return m_dActivation; };
		void			SetToNewActivation();

	private:
		double			m_dActivation;	
		double			m_dNewActivation;
};

