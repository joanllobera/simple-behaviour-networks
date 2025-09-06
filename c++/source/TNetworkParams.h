

#include "Timepath_header.h"

#pragma once


/*!

\brief It contains the parameters for behavior networks (TBehaviorNetwork, TDorer2Network and TLlogicNetwork)
\date 28.05.98
*/

const  float GAMMA = 1.5;
const float DELTA = 1.5; //JL30082010, before it was 1.0

const float PHI = 0.4;
const float THETA = 2.0;
const float THRESHOLD_REDUCTION = 0.2;	// 10 % of THETA
const float NETWORK_PI = 1.8;
const float MAX_ACTIVATION = 4.0;

//#define BETA						0.5
const float BETA = 0.2; //JL30082010

const float MAX_ACTIVATION_SIGMOID = 1.0;
const float SIGMA = 0.5;	// must be MAX_ACTIVATION / 2
const float GAIN = 5.0;	// steepness of the sigmoid function
const float GAMMA_SIGMOID = 0.5;
const float DELTA_SIGMOID = 0.2;
const float PHI_SIGMOID = 0.1;
const float THETA_SIGMOID = 0.4;
const float TH_REDUCTION_SIGMOID = 0.04; //10% of theta sigmoid	


class TNetworkParams  
{



	

	public:
		TNetworkParams();
		TNetworkParams(TNetworkParams* params);
		virtual ~TNetworkParams();

		void		SetNetworkParams(TNetworkParams* params);

		// get functions
		double	GetPhi(void) const;
		double	GetGamma(void) const;
		double	GetDelta(void) const;
		double	GetPhiByGamma(void) const;
		double	GetDeltaByGamma(void) const;
		double	GetNetworkPi(void) const;
		double	GetTheta(void) const;
		double	GetThetaReduction(void) const;
		double	GetSigma(void) const;
		double	GetGain(void) const;
		double	GetBeta(void) const;

		double	GetPhiTimesGamma(void) const;
		double	GetGammaSquare(void) const;
		double	GetDeltaTimesGamma(void) const;

		// set functions
		void		SetPhi(double val);
		void		SetGamma(double val);
		void		SetDelta(double val);
		void		SetNetworkPi(double val);
		void		SetTheta(double val);
		void		SetThetaReduction(double val);
		void		SetSigma(double val);
		void		SetGain(double val);
		void		SetBeta(double val);


	private:
		double	m_dPhi;
		double	m_dGamma;
		double	m_dDelta;
		double	m_dTheta;
		double	m_dThetaReduction;
		double	m_dSigma;
		double	m_dGain;
		double	m_dBeta;



		double	m_dPhiByGamma;
		double	m_dDeltaByGamma;
		double	m_dPhiTimesGamma;
		double	m_dGammaSquare;
		double	m_dDeltaTimesGamma;
};

