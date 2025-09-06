
#include "Timepath_header.h"
#include "NetworkHeader.h"


#include <string>
#include <iostream>
#include <conio.h>
#include "TEnergySpreader.h"

#include <math.h>

TEnergySpreader::TEnergySpreader()
{
	Initialize();
}

TEnergySpreader::TEnergySpreader(string name, int index)
{
	m_strName = name;
	m_iIndex = index;
	m_dMainActivation = 0.0;
	m_dActivationChange = 0.0;
	m_dMaxActivation = 1.0;
	m_iNoOfDestinations = 0;
	m_bSelected = FALSE;



}

void TEnergySpreader::Initialize()
{
	m_strName="";
	m_iIndex = 0;
	m_dMainActivation = 0.0;
	m_dActivationChange = 0.0;
	m_dMaxActivation = 1.0;
	m_iNoOfDestinations = 0;
	m_bSelected = FALSE;
	
	#ifdef ACTIVATION_HISTORY
		for (int i = 0; i < MAX_HISTORY; i++)	
			m_adHistory[i] = 0.0;
		m_iNoOfHistory = 0;
	#endif

	#ifdef ACTIVATION_DISTRIBUTION
		for (int j = 0; j < NO_OF_SLOTS; j++)
			m_aiDistribution[j] = 0;
	#endif
}

TEnergySpreader::~TEnergySpreader()
{
}


/*!

\brief
\return if the new activation of the module changed
\date 1998

\param the activation value to be set
\todo
\details It sets the new activation of the module. Before it calculates the
				 change in activation.   all methods should use this function instead 
				 of manipulating m_dMainActivation directly

				 It can store the activation history if needed for debugigng or visualisaton purposes
\sa
*/


double TEnergySpreader::SetActivation(double activation)
{ 
	m_dActivationChange = activation - m_dMainActivation;

	#ifdef ACTIVATION_HISTORY
		m_adHistory[m_iNoOfHistory] = activation;
		m_iNoOfHistory++;
		if (m_iNoOfHistory >= MAX_HISTORY)
			m_iNoOfHistory = MAX_HISTORY - 1;
	#endif

	#ifdef ACTIVATION_DISTRIBUTION
		int slot = (int) (activation * 10.0);
		if (slot >= NO_OF_SLOTS)
			slot = NO_OF_SLOTS - 1;
		if (slot < 0)
			slot = 0;
		m_aiDistribution[slot]++;
	#endif
	
	return (m_dMainActivation = activation); 
}


