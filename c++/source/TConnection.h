#pragma once
#include "Timepath_header.h"
class TProposition;
class TScreenParamsNetwork;
class TEnergySpreader;
class TNetworkParams;

/*!

 \brief class describing an energy connection between two modules
 \date 1998
 \details  class describing an energy connection between two modules. The
				 TEnergySpreader object is the source of the energy, the object
				 containing this connection is the destination.
				 May be it is more efficient to have separate classes for conflictor
				 connections, not connections.

 
*/


class TConnection //: public TObject  
{
	public:
		TConnection();
		TConnection(TNetworkParams* params, TProposition* src, TProposition *dest);
		virtual ~TConnection();

		// energy functions

		//! return the activation of the connection
		virtual double		GetActivation() { return m_dActivation; };

		//! Adjust the maximum activation of the connection
		virtual void		SetMaxActivation() { m_dMaxActivation = 1.0; };



		TEnergySpreader*	GetSourceModule();
		TEnergySpreader*	GetDestinationModule();

		//! Get the proposition that is at the source (typically associated with a world state)
		TProposition*		GetSourceProp() { return m_pSourceProp; };

		//! Get the proposition that is at the source (typically associated with a precondition or resource within a Competence Module)
		TProposition*		GetDestinationProp() { return m_pDestinationProp; };

		//! Set the values of the network parameters
		void					SetNetworkParams(TNetworkParams* params) { m_pNetworkParams = params; };

	protected:
	
		//! proposition of the energy source
		TProposition*		m_pSourceProp;			

		//! proposition of the destination module 
		TProposition*		m_pDestinationProp;	

		//! activation of the connection
		double				m_dActivation;

		//! maximum activation of the connection
		double				m_dMaxActivation;
		
		//! Network params to find the successive activation level
		TNetworkParams*	m_pNetworkParams;
};


