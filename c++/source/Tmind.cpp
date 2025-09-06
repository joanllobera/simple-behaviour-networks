#include "TMind.h"


TMind::TMind(string uniqueID)
{
	theNet = new TLlogicNetwork();

	this->uniqueID = uniqueID;

}


TMind::~TMind()
{
}

