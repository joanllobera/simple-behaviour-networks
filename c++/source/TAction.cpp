

#include "NetworkHeader.h"
#include "TAction.h"

#include <math.h>

#ifdef UNITYAGENT
	#include "TCallbackConverter.h"
#endif

#ifdef NO_ACTION_PARAMETER
#include "TNetworkParams.h"
#endif


TAction::TAction(string name, int index, TObject* pObj, PCallBackAction pFn)
{
#if UNITYAGENT==0
	SetAction(name, index, pObj, pFn);
	m_dMinActivation = 1000.0;
	m_dMaxActivation = 0.0;
	m_dMeanActivation = 0.0;
	m_dVarianz = 0.0;
	m_lNoOfActionPerformed = 0L;
	m_dDecisiveness = 0.0;

#if STORIES
	pActRes = NULL;
	this->mod = NULL;
#endif
#else
	TCallbackConverter* pObj2 = new TCallbackConverter();
	pObj2->setInternalActionCallback(pObj,pFn);
	TAction(name, index, pObj2);
#endif
	
}
#ifdef UNITYAGENT

TAction::TAction(string name, int index, TCallbackConverter*  pObj)
{
	this->m_pObject = pObj;
	this->m_strName = name;
	this->m_iIndex = index;

	m_dMinActivation = 1000.0;
	m_dMaxActivation = 0.0;
	m_dMeanActivation = 0.0;
	m_dVarianz = 0.0;
	m_lNoOfActionPerformed = 100L;
	m_dDecisiveness = 0.0;

#if STORIES
	pActRes = NULL;
	this->mod = NULL;
#endif
}

void TAction::ResetCallCounter(){
	this->m_pObject->resetCallCounter();
}
int TAction::GetCallCount()
{ return m_pObject->getCallCount(); }

#endif



TAction::~TAction()
{
}

#if UNITYAGENT==0


/*!

\brief   sets the parameters of a behavior
\date 1999

\param name of the behavior
			  pointer to the object containing the action function
			  pointer to the action function
*/


void TAction::SetAction(string name, int index, TObject* pObj, PCallBackAction pFn)
{
	m_strName = name;
	m_iIndex = index;
	m_pObject = pObj;
	m_pAction = pFn;
}
#endif



/*!

\brief
\return
\date 02.2014

\param The modules activation/executability
\todo recheck whether this function is really doing its job as it should (why not min/max but stddev?)
\details
\sa
*/


void TAction::PerformAction(double activation)
{
	double	dNewMean;
	double	dNewN = (double) (m_lNoOfActionPerformed + 1L);
	double	dDeltaMean;
	double	dStdDev;
	double	dLeft;
	double	dRight;

	// Version with mean and stddev
	// mean activation (incrementally calculated)
	dNewMean = m_dMeanActivation + (activation - m_dMeanActivation) / dNewN;

	// standard deviation of activation (incrementally calculated)
	// Sneu = (n+1)(Mneu - M)2 + (n-1)S / n
	if (m_lNoOfActionPerformed > 0L)
	{
		dDeltaMean = dNewMean - m_dMeanActivation;
		m_dVarianz = dNewN * (dDeltaMean * dDeltaMean) 
										+ ((double)(m_lNoOfActionPerformed - 1L)
										* m_dVarianz / (double) m_lNoOfActionPerformed);
	}
	m_dMeanActivation = dNewMean;
	dStdDev = sqrt(m_dVarianz);
	dLeft = m_dMeanActivation - dStdDev;
	dRight = m_dMeanActivation + dStdDev;
	// mean - stddev is mapped to 0, mean + stddev to 1 
	if (dRight - dLeft < D_ZERO_P)
		m_dDecisiveness = 1.0;
	else if (activation < dLeft)
		m_dDecisiveness = 0.0;
	else if (activation > dRight)
		m_dDecisiveness = 1.0;
	else
		m_dDecisiveness = (activation - dLeft) / (dRight - dLeft);

/*	// Version with min and max
	// store the minimal and maximal activation with which this action is called
	if (activation < m_dMinActivation)
		m_dMinActivation = activation;
	if (activation > m_dMaxActivation)
		m_dMaxActivation = activation;

	// the activation is mapped into a range from 0.0 to 1.0 where
	// 0.0 is minimal activation, 1.0 is maximal activation
	if (m_dMaxActivation - m_dMinActivation < D_ZERO_P)
		// min and max are equal, so take max
		activation = 1.0;
	else
		activation = (activation - m_dMinActivation) / (m_dMaxActivation - m_dMinActivation);

*/
	//ASSERT((m_dDecisiveness > -0.01) && (m_dDecisiveness < 1.01));//a debug assertion I don't need
	

	#ifdef NO_ACTION_PARAMETER
		m_dDecisiveness = PHI;
	#endif


#if UNITYAGENT==0
	if ((m_pObject != NULL) && (m_pAction != NULL))
		(m_pObject->*m_pAction)(activation);
//		(m_pObject->*m_pAction)(m_dDecisiveness);
#else
//		m_pObject->actionCallback(m_dDecisiveness);//see discussion Dorer2004, section 4
		m_pObject->actionCallback(m_dDecisiveness,activation);//we also need activation for drawing purposes
#endif

	m_lNoOfActionPerformed++;
}
