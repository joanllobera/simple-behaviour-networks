#include "NetworkHeader.h"
#include "TNetworkParams.h"

TNetworkParams::TNetworkParams()
{
	SetGamma(GAMMA);
	SetPhi(PHI);
	SetDelta(DELTA);
	SetBeta(BETA);
	SetTheta(THETA);
	SetThetaReduction(THRESHOLD_REDUCTION);
	SetSigma(SIGMA);
	SetGain(GAIN);
}

TNetworkParams::TNetworkParams(TNetworkParams* params) 
{
	SetNetworkParams(params); 
}

TNetworkParams::~TNetworkParams()
{
}

void TNetworkParams::SetNetworkParams(TNetworkParams* params)
{
	SetGamma(params->GetGamma());
	SetPhi(params->GetPhi());
	SetDelta(params->GetDelta());
	SetSigma(params->GetSigma());
	SetGain(params->GetGain());
	SetBeta(params->GetBeta());
	SetTheta(params->GetTheta());
	SetThetaReduction(params->GetThetaReduction());
}

void TNetworkParams::SetGamma(double val)
{
	m_dGamma = val; 
	m_dPhiByGamma = m_dPhi / m_dGamma; 
	m_dDeltaByGamma = m_dDelta / m_dGamma; 
	m_dPhiTimesGamma = m_dPhi * m_dGamma;
	m_dGammaSquare = m_dGamma * m_dGamma;
	m_dDeltaTimesGamma = m_dDelta * m_dGamma;	
}

void TNetworkParams::SetDelta(double val)
{
	m_dDelta = val;
	m_dDeltaByGamma = m_dDelta / m_dGamma; 
	m_dDeltaTimesGamma = m_dDelta * m_dGamma;
}
		
void TNetworkParams::SetPhi(double val)
{
	m_dPhi = val; 
	m_dPhiByGamma = m_dPhi / m_dGamma; 
	m_dPhiTimesGamma = m_dPhi * m_dGamma;
}

void TNetworkParams::SetNetworkPi(double val) { m_dBeta = val; }
void TNetworkParams::SetTheta(double val) { m_dTheta = val; }
void TNetworkParams::SetThetaReduction(double val) { m_dThetaReduction = val; }
void TNetworkParams::SetSigma(double val) { m_dSigma = val; }
void TNetworkParams::SetGain(double val) { m_dGain = val; }
void TNetworkParams::SetBeta(double val) { m_dBeta = val; }

double TNetworkParams::GetPhi(void) const { return m_dPhi; }
double TNetworkParams::GetGamma(void) const { return m_dGamma; }
double TNetworkParams::GetDelta(void) const { return m_dDelta; }
double TNetworkParams::GetPhiByGamma(void) const { return m_dPhiByGamma; }
double TNetworkParams::GetDeltaByGamma(void) const { return m_dDeltaByGamma; }
double TNetworkParams::GetNetworkPi(void) const { return m_dBeta; }
double TNetworkParams::GetTheta(void) const { return m_dTheta; }
double TNetworkParams::GetThetaReduction(void) const { return m_dThetaReduction; }
double TNetworkParams::GetSigma(void) const { return m_dSigma; }
double TNetworkParams::GetGain(void) const { return m_dGain; }
double TNetworkParams::GetBeta(void) const { return m_dBeta; }

double TNetworkParams::GetPhiTimesGamma(void) const { return m_dPhiTimesGamma; }
double TNetworkParams::GetGammaSquare(void) const { return m_dGammaSquare; }
double TNetworkParams::GetDeltaTimesGamma(void) const { return m_dDeltaTimesGamma; }
