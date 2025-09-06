#include "TDorer2GoalTrack.h"
#include <math.h>


TDorer2GoalTrack::TDorer2GoalTrack(int iNoOfGoals)
{
	m_iNoOfGoals = iNoOfGoals;
	m_pdActivation = new double[iNoOfGoals];
	m_pdNewActivation = new double[iNoOfGoals];
	Initialize();
}


TDorer2GoalTrack::~TDorer2GoalTrack()
{
	delete [] m_pdNewActivation;
	delete [] m_pdActivation;
}


void TDorer2GoalTrack::Initialize()
{
	int	i;

	for (i = 0; i < m_iNoOfGoals; i++)
	{
		m_pdActivation[i] = 0.0;
		m_pdNewActivation[i] = 0.0;
	}
}

void TDorer2GoalTrack::SetToNewActivation()
{
	int	i;

	for (i = 0; i < m_iNoOfGoals; i++)
	{
		m_pdActivation[i] = m_pdNewActivation[i];
		m_pdNewActivation[i] = 0.0;
	}
}

/*!

\brief sets the activation for goal with index i
\return
\date 02.2014

\param index of the goal
			  value for the activation

\details
sets the activation for goal with index i

if the absolute of the new value is larger than old  value, the old value is replaced

*/


void TDorer2GoalTrack::SetActivation(int i, double val)
{
	ASSERT ((i >= 0) && (i < m_iNoOfGoals)); 
	// take maximum value
	if (fabs(val) > fabs(m_pdNewActivation[i]))
		m_pdNewActivation[i] = val;
}

/*!

\brief Sums the activation of each goal
\date 02.2014

*/
double TDorer2GoalTrack::GetSumActivation() const
{
	int		i;
	double	dSum = 0.0;

	for (i = 0; i < m_iNoOfGoals; i++)
		dSum += m_pdActivation[i];

	return dSum;
}

TDorer2NoGoalTrack::TDorer2NoGoalTrack(int iNoOfGoals)
{
	m_dActivation = 0.0;
	m_dNewActivation = 0.0;
}

void TDorer2NoGoalTrack::SetToNewActivation()
{
	m_dActivation = m_dNewActivation;
	m_dNewActivation = 0.0;
}

void TDorer2NoGoalTrack::SetActivation(int i, double val)
{
	m_dNewActivation += val;
}

double TDorer2NoGoalTrack::GetSumActivation() const
{
	return m_dActivation;
}
