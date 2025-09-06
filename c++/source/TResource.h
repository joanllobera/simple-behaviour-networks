
#pragma once


#include "Timepath_header.h"
#include "TWorldState.h"

/*!

\brief A resource in the network
\date 02.2014
\details  A resource in the network is used to coordinate parallel action selection (in order, for example, that not two actions use the same resource at the same time)
\sa TWorldState, TResourceFactory
*/

class TResource : public TWorldState
{
	public:
		TResource(string name, int index, TObject* pObj, PCallBackPerception pCall, double iMaxAvail);

#ifdef UNITYAGENT
		TResource(string name, int index, TCallbackConverter* pObj, double iMaxAvail);
		virtual void ResetCallCounter();
#endif
		virtual ~TResource();

		void	ReduceActivationLevel(double dDeltaTheta);
		void	ResetActivationLevel(double dTheta);
		virtual void	ReduceAmountAvailable(double iReduction); 
		void	IncreaseAmountAvailable(double iIncrease);
		void	ResetAmountAvailable() { m_iAmountAvailable = m_iMaxAvailable; };
		double	GetAmountAvailable() const { return m_iAmountAvailable; };


		//update the maximum available from the callback already set up.
		void UpdateMaxAvailable();

		//! todo remove this function, in order ALL are dealt through callbacks.
		int	UpdateMaxAvailable(double value);		 
		double	GetAmountUsed(){return (m_iMaxAvailable-m_iAmountAvailable);}; 

	private:
		//! the total amount of resources available globally
		double	m_iMaxAvailable;
		//! the amount of resources available (that are not in use) 
		double	m_iAmountAvailable;  

#if UNITYAGENT
		//! the method to update hte number of resources available
		TCallbackConverter* CallbackResourceCount;
#endif

#if STORIES 
		//! this is false if this is a non spendable resource (non spendable resources are needed to manage well actions within stories)
		bool SpendableResource; 
	public:
		void SetSpendable(bool spendable){SpendableResource=spendable;};
#endif


};

