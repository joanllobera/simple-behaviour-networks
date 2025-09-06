

#include "NetworkHeader.h"
#include "TEnergySpreader.h"
#include "TProposition.h"
#include "TNetworkParams.h"
#include "TDorer2Competence.h"
#include "TDorer2Connection.h"


TDorer2SituationLink::TDorer2SituationLink(TNetworkParams* params, TProposition* src, TProposition* dest)
						: TConnection(params, src, dest)
{
}

//! return the activation of the destination link \sa TConnection::GetActivation
double TDorer2SituationLink::GetActivation() 
{ 
	double			dProposition;

	ASSERT(m_pDestinationProp != NULL);

	dProposition = m_pDestinationProp->GetValue();
	return m_dActivation = dProposition;
}

//! set the maximum activation of the connection to 1.0 \sa TConnection::SetMaxActivation
void TDorer2SituationLink::SetMaxActivation()
{
	m_dMaxActivation = 1.0;
}


TDorer2GoalLink::TDorer2GoalLink(TNetworkParams* params, TProposition* src, TProposition* dest)
						: TConnection(params, src, dest)
{
}


//! return the activation produced by the goal (its own activation x its importance x the source truth value ) \sa TConnection::GetActivation
double TDorer2GoalLink::GetActivation() 
{ 
	ASSERT(m_pSourceProp != NULL);
	ASSERT(m_pDestinationProp != NULL);

	m_dActivation = m_pNetworkParams->GetGamma() 
						* m_pSourceProp->GetModule()->GetActivation()
						* m_pDestinationProp->GetWeigh();
	return m_dActivation;
}

//! set the maximum activation of the connection to the gamma value of the parameters \sa TConnection::SetMaxActivation, TNetworkParams::GetGamma
void  TDorer2GoalLink::SetMaxActivation()
{
	m_dMaxActivation = m_pNetworkParams->GetGamma();
}

TDorer2ProtectedGoalLink::TDorer2ProtectedGoalLink(TNetworkParams* params, TProposition* src, TProposition* dest)
							: TConnection(params, src, dest)
{
}


//! return the activation produced by the goal (its own activation x its importance x the source truth value ) \sa TConnection::GetActivation
double TDorer2ProtectedGoalLink::GetActivation() 
{ 
	ASSERT(m_pSourceProp != NULL);
	ASSERT(m_pDestinationProp != NULL);



	m_dActivation = -m_pNetworkParams->GetDelta() 
						* m_pSourceProp->GetModule()->GetActivation() 
						* m_pDestinationProp->GetWeigh();
	return m_dActivation;
}

//! set the maximum activation of the connection to the gamma value of the parameters \sa TConnection::SetMaxActivation, TNetworkParams::GetDelta
void  TDorer2ProtectedGoalLink::SetMaxActivation()
{
	m_dMaxActivation = m_pNetworkParams->GetDelta();
}

TDorer2SuccessorLink::TDorer2SuccessorLink(TNetworkParams* params, TProposition* src, TProposition* dest)
							: TConnection(params, src, dest)
{
}


//! return the activation 
double TDorer2SuccessorLink::GetActivation() 
{ 
	return m_dActivation;
}

//! return the activation produced by the goal through its successor \sa TConnection::GetActivation
double TDorer2SuccessorLink::GetGoalActivation(int iGoalIndex) 
{
	double dActivation;
	double dProposition;

	ASSERT(m_pSourceProp != NULL);
	ASSERT(m_pDestinationProp != NULL);

	dActivation = ((TDorer2Competence*) m_pSourceProp->GetModule())->GetTransferedActivation(iGoalIndex);
	dProposition = (1.0 - m_pSourceProp->GetValue());

	dActivation = m_pNetworkParams->GetGamma() 
						* dProposition
						* dActivation
						* m_pDestinationProp->GetWeigh();
	if (iGoalIndex == 0)
		m_dActivation = dActivation;
	else
		m_dActivation += dActivation;
	
	return dActivation;
}

//! set the maximum activation of the connection to the gamma value of the parameters \sa TConnection::SetMaxActivation, TNetworkParams::GetGammaSquare
void TDorer2SuccessorLink::SetMaxActivation()
{
	m_dMaxActivation = m_pNetworkParams->GetGammaSquare();
}


TDorer2ConflictorLink::TDorer2ConflictorLink(TNetworkParams* params, TProposition* src, TProposition* dest)
							: TConnection(params, src, dest)
{
}

//! return the activation 
double TDorer2ConflictorLink::GetActivation() 
{
	return m_dActivation;
}

//! return the activation produced by the goal through its conflictor \sa TConnection::GetActivation
double TDorer2ConflictorLink::GetGoalActivation(int iGoalIndex) 
{ 
	double dProposition;
	double dActivation;

	ASSERT(m_pSourceProp != NULL);
	ASSERT(m_pDestinationProp != NULL);

	dActivation = ((TDorer2Competence*) m_pSourceProp->GetModule())->GetTransferedActivation(iGoalIndex);

	// if the precondition is false anyway, there is no inhibition: 
	dProposition = m_pSourceProp->GetValue();

	dActivation = -m_pNetworkParams->GetDelta() 
					* dProposition 
					* dActivation
					* m_pDestinationProp->GetWeigh();
	if (iGoalIndex == 0)
		m_dActivation = dActivation;
	else
		m_dActivation += dActivation;
	
	return dActivation;
}

//! set the maximum activation of the connection to the gamma value of the parameters \sa TConnection::SetMaxActivation, TNetworkParams::GetDeltaTimesGamma
void TDorer2ConflictorLink::SetMaxActivation()
{
	m_dMaxActivation = m_pNetworkParams->GetDeltaTimesGamma();
}



TResourceLink::TResourceLink(TNetworkParams* params, TProposition* src, TProposition* dest)
							: TConnection(params, src, dest)
{
}
