#pragma once

#include "Timepath_header.h"
#include "TProposition.h"

/*!

\brief An extension of TProposition to manage the amount of resources used
\date 2010
*/

class TResourceProposition : public TProposition  
{
	public:
		TResourceProposition(TEnergySpreader* module, TWorldState* state, double value);
		virtual ~TResourceProposition();

		double	GetAmountNeeded() const { return amountNeeded; };

		//! \date 05.2010 \brief To take into account that  resource usage is sensitive to situation s (still within Dorer 2004), this method updates Tau_u
		void SetAmountUsed(double value){ amountNeeded=value;}; 

	private:
		double	amountNeeded;
};
