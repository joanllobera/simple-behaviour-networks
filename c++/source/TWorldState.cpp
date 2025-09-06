

#include "NetworkHeader.h"
#include "TProposition.h"
#include "TCallbackConverter.h"
#include "TWorldState.h"

/*!

\brief The constructor of the world state representation
\return
\date 02.2014

\param	name contains the name of the world state
		index says what index it has in TPercept
		pObj points to the object for the callback
		pCall points to the method used to update the world state
\todo
\details The constructor of the world state representation
		 It is important to notice that by default the variable
		 m_bFuzzy is set to true, and m_dFuyyzRound is set to FUZZY_ROUND
		 This means that a perception lower than FUZZY_ROUND will be round to 0.0

\sa
*/
TWorldState::TWorldState(string name, int index, TObject* pObj
								 , PCallBackPerception pCall)
				: TEnergySpreader(name, index)
{
	m_pProposition = new TProposition(this, this, 0.0, FALSE);
	Initialize();

#if UNITYAGENT==0
	m_pObject = pObj;
	m_pTruthFunction = pCall;
#else
	TCallbackConverter* pObjCall = new TCallbackConverter();
	pObjCall->setInternalPerceptionCallback(pObj, pCall);
#endif
	m_bFuzzy = TRUE;
	m_dFuzzyRound = FUZZY_ROUND;
}


#ifdef UNITYAGENT

/*!

\brief The constructor of the world state representation
\return
\date 02.2014

\param	name contains the name of the world state
index says what index it has in TPercept
pObj points to the  callback object


\details The constructor of the world state representation
It is important to notice that by default the variable
m_bFuzzy is set to true, and m_dFuyyzRound is set to FUZZY_ROUND (defined in networkheader.h)
This means that a perception lower than FUZZY_ROUND will be round to 0.0

Do not commit the mistake of using this function for TResource. This would come down to say the amount of a resource available is its activation level
\sa
*/



TWorldState::TWorldState(string name, int index, TCallbackConverter* pObj)
	: TEnergySpreader(name, index)
{
	m_pProposition = new TProposition(this, this, 0.0, FALSE);
	Initialize();
	m_pObject = pObj;
	m_bFuzzy = TRUE;
	m_dFuzzyRound = FUZZY_ROUND;
}

#endif



TWorldState::~TWorldState()
{
	delete m_pProposition;
	m_pObject = nullptr;
}



/*!
\brief It puts  the activation level at 0.0 and sets the World State to consider it is not being used
\date 02.2014
\details It puts  the activation level at 0.0 and sets the World State to consider it is not being used
*/
void TWorldState::Initialize()
{
	 
	m_bUsed = FALSE;
	SetActivation(0.0);
}

double TWorldState::GetTruthValue()
{
	double	value = -1.1;

	
#if UNITYAGENT==0
	if ((m_pObject != NULL) && (m_pTruthFunction != NULL))
		value = (m_pObject->*m_pTruthFunction)();
	m_pProposition->SetValue(value);
#else
	//we only update the perceptions once per updateMind. If we have already done the callback once, we only send the value returned by that callback
	if (m_pObject->getCallCount() == 0){
		value = m_pObject->perceptionCallback();
		m_pProposition->SetValue(value);
	}
#endif

	return m_pProposition->GetValue();

}

#ifdef UNITYAGENT
void TWorldState::ResetCallCounter()
{
	m_pObject->resetCallCounter();
}


#endif



/*!
\brief Calculates the truth value  associated with that world state
\return  The truth value
\date 02.2014

\param 
\todo
\details Calculates the truth value  associated with that world state. 
		If m_bFuzzy is true, this function will also reduce to 0.0 the truth level if it is under a certain threshold (m_dFuzzyRound)

\sa
*/
double TWorldState::GetMainActivation() const
{
	double	value = TEnergySpreader::GetMainActivation();

#ifdef INPUT_NOISE_ON
	// percent error
	value += value * (1.0 - (double) rand() / ((double) RAND_MAX / 2.0)) * INPUT_NOISE_VALUE;
	if (value < 0.0)
		value = 0.0;
	if (value > 1.0)
		value = 1.0;
#endif

	if (m_bFuzzy)
		return value;
	else if (value < m_dFuzzyRound)
		return 0.0;
	else
		return 1.0;
}