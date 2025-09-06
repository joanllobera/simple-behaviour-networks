
#include "NetworkHeader.h"
#include "TResource.h"

#ifdef UNITYAGENT
#include "TCallbackConverter.h"
#endif


/*!
\brief Creates a resource (later on needed for some action) using an object and a method for callback
\return It returns a pointer to the resource created
\date 02.2014
\param  name is the name of the resource,
index is the index of the resource in the factory
pObj is the object containing the callback conversion,
iMaxAvail is the maximum amount of resources available
\sa TResourceFactory::AddResource
*/

TResource::TResource(string name, int index, TObject* pObj, PCallBackPerception pCall, double iMaxAvail) 
		: TWorldState(name, index, pObj, pCall)
{
	m_iMaxAvailable = iMaxAvail;
	ResetAmountAvailable();

#if STORIES
		SpendableResource=false;
#endif
}

#ifdef UNITYAGENT
/*!
\brief Creates a resource (later on needed for some action) using the class TCallbackConverter
\return It returns a pointer to the resource created
\date 02.2014
\param  name is the name of the resource, 
		index is the index of the resource in the factory
		pObj is the object containing the callback conversion, 
		iMaxAvail is the maximum amount of resources available
\sa TResourceFactory::AddResource
*/


TResource::TResource(string name, int index, TCallbackConverter* pObj, double iMaxAvail) 
: TWorldState(name, index, this, (PCallBackPerception) &TResource::GetActivation)
{
	m_iMaxAvailable = iMaxAvail;
	this->CallbackResourceCount = pObj;

	ResetAmountAvailable();
#if STORIES
	SpendableResource = false;
#endif
}

void TResource::ResetCallCounter()
{
	this->CallbackResourceCount->resetCallCounter();
}

#endif




TResource::~TResource()
{
}

/*!

\brief it reduces the activation threshold of the resource (but not below to zero)
\date 02.2014
\param dDeltaTheta The amount the theta parameter of an extended behavior network should be reduced
\detail  it reduces the activation threshold of the resource (but not below to zero)
*/


void TResource::ReduceActivationLevel(double dDeltaTheta)
{
	double	dActivation = GetActivation();

	dActivation -= dDeltaTheta;
	if (dActivation < 0.00001)
		dActivation = 0.00001;
	SetActivation(dActivation);
}



/*!

\brief it resets the activation threshold of the resource
\date 02.2014
\param The amount the theta parameter should be set
\detail it resets the activation threshold of the resource (typically, once a module has been executed)
*/

void TResource::ResetActivationLevel(double dTheta)
{
	SetActivation(dTheta);
}






/*!

\brief it updates the Maximum amount of resource available
\date 08.2014

\detail
it updates the amount of resource available from the callback provided in the constructor.
In the formalism of Dorer 2004, this amounts to change Tau_r from s

*/

void TResource::UpdateMaxAvailable(){
	double newMaxAmount = this->CallbackResourceCount->perceptionCallback();

	double test = this->GetAmountUsed();
	ASSERT(newMaxAmount >= test);
	//wether the competence modules running actions have bound more elements that are actually existing according to the new perception
	this->UpdateMaxAvailable(newMaxAmount);



}

/*!

\brief it updates the Maximum amount of resource available
\date 05.2010
\param The maximum amount of available resource
\detail
it updates the amount of resource available in order
resources are sensitive to updates from situation s.
In the formalism of Dorer 2004, this amounts to change Tau_r from s

\todo remove this function in roder everything is dealt through callbacks
*/


int	TResource::UpdateMaxAvailable(double value){
 //change Tau_r from s


	double temp_amountUsed = this->GetAmountUsed();

	if (value < this->m_iMaxAvailable && value < this->GetAmountUsed()){
		//we are trying to say that we have less amount than the ones we are already using.
		stringstream logstream;

		logstream << "error: you are trying to set the amount of resource " << this->GetName() << " to " << value << "but I am using " << this->GetAmountUsed() << ". I am afraid I can't do this";
		SEND2LOG(logstream.str().c_str());
		return 0;


	}
	m_iMaxAvailable = value;
	double difference = m_iMaxAvailable - value;
	m_iAmountAvailable-=difference;
	return 1;
}



/*!

\brief  decreases the amount of resource available to use 
\date 02.2014

\param the amount of decrease
\details decreases the amount of resource available to use (always staying bigger than 0)
*/



#ifndef STORIES
void TResource::ReduceAmountAvailable(double iReduction)
{
	ASSERT(iReduction <= m_iAmountAvailable); 
	m_iAmountAvailable -= iReduction;
}

#else

void TResource::ReduceAmountAvailable(double iReduction){
#if STORIES
	if(SpendableResource){
#endif
		ASSERT(iReduction <= m_iAmountAvailable); 
		m_iAmountAvailable -= iReduction;
#if STORIES
	}
#endif

}

#endif

/*!

\brief  increases the amount of resource available to use 
\date 02.2014

\param the amount of increase
\details increases the amount of resource available to use (always staying smaller than the maximum amount available)
*/

#ifndef STORIES
void TResource::IncreaseAmountAvailable(double iIncrease)
{
	ASSERT(iIncrease + m_iAmountAvailable <= m_iMaxAvailable); 
	m_iAmountAvailable += iIncrease;
}
#else


void TResource::IncreaseAmountAvailable(double iIncrease)
{
#if STORIES
	if(SpendableResource){
#endif
		//ASSERT(iIncrease + m_iAmountAvailable <= m_iMaxAvailable); 
		
		
		m_iAmountAvailable += iIncrease;
		if (iIncrease + m_iAmountAvailable > m_iMaxAvailable){
			m_iMaxAvailable = iIncrease + m_iAmountAvailable;
			stringstream logstream;
			logstream << "increased the total of resource " << this->GetName() << " availalbe to " << m_iMaxAvailable << " when releasing the resources used in an action.";
			SEND2LOG(logstream.str().c_str());
		}
#if STORIES
	}
#endif

}


#endif



