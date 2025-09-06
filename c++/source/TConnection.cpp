
#include "NetworkHeader.h"
#include "TEnergySpreader.h"
#include "TProposition.h"
#include "TNetworkParams.h"
#include "TConnection.h"

#include <math.h>

TConnection::TConnection()
{
	m_pNetworkParams = NULL;
	m_pSourceProp = NULL;
	m_pDestinationProp = NULL;
	m_dActivation = 0.0;
	m_dMaxActivation = 1.0;
}

TConnection::TConnection(TNetworkParams* params, TProposition* src, TProposition* dest)
{
	m_pNetworkParams = params;
	m_pSourceProp = src;
	m_pDestinationProp = dest;
	m_dActivation = 0.0;
	m_dMaxActivation = 1.0;
}

TConnection::~TConnection()
{
}




/*!

 \brief Get the energyspreader that is the source of the connection
 \date  03.98
 \return the module which is the source of this connection
 
 \sa TProposition::GetModule
*/
TEnergySpreader* TConnection::GetSourceModule()
{
	return m_pSourceProp->GetModule();
}


/*!

 \brief Get the energyspreader that is the destination of the connection
 \date  03.98
 \return the module which is the destination of this connection
 
*/

TEnergySpreader* TConnection::GetDestinationModule()
{
	return m_pDestinationProp->GetModule();
}

