
#include "NetworkHeader.h"
#include "TResourceProposition.h"


TResourceProposition::TResourceProposition(TEnergySpreader* module, TWorldState* state, double value)
	: TProposition(module, state, value, FALSE)
{
	amountNeeded = (int) value;
}

TResourceProposition::~TResourceProposition()
{
}
