
#include "NetworkHeader.h"

#include "TWorldState.h"
#include "TResource.h"
#include "TResourceFactory.h"

TResourceFactory::TResourceFactory()
{

}

TResourceFactory::~TResourceFactory()
{
	// the factory objects are deleted by the base class
}

/*!
\brief Adds a resource needed for some action using an object and a method to do the callback
\return It returns a pointer to the resource created
\date 02.2014
\param  name is the name of the resource, pObj is the object containing the callback conversion, iAmount is the amount of resource expected to be used
\sa TPercept::AddPerception
*/

TResource* TResourceFactory::AddResource(string name, TObject* pObj
											 , PCallBackPerception pCallback, double iAmount)
{
	TResource* pState;


	pState = new TResource(name, m_pFactoryObjects.size(), pObj, pCallback, iAmount);


	m_pFactoryObjects.push_back((TWorldState*) pState);
	return pState;
}

#ifdef UNITYAGENT
/*!
\brief Adds a resource needed for some action using the class TCallbackConverter 
\return It returns a pointer to the resource created 
\date 02.2014
\param  name is the name of the resource, pObj is the object containing the callback conversion, iAmount is the amount of resource expected to be used
\sa TPercept::AddPerception, TResource constructor
*/



TResource* TResourceFactory::AddResource(string name, TCallbackConverter* pObj, double iAmount)
{
	TResource* pState;

	pState = new TResource(name, m_pFactoryObjects.size(), pObj, iAmount);

	m_pFactoryObjects.push_back((TWorldState*)pState);
	return pState;
}

void TResourceFactory::UpdateResourcesAvailable()
{

	list<TWorldState*>::iterator pos;

	for (pos = m_pFactoryObjects.begin(); pos != m_pFactoryObjects.end(); pos++){
		if ((*pos)->IsUsed()){
			((TResource*) (*pos))->UpdateMaxAvailable();
		}
	}

}


void TResourceFactory::ResetCallbackCounters(){

	list<TWorldState*>::iterator pos;
	for (pos = m_pFactoryObjects.begin(); pos != m_pFactoryObjects.end(); pos++){
		(*pos)->ResetCallCounter();
	}
}


#endif



/*!

\brief Finds the resource within the list of resources
\return the resource sought
\date 02.2014

\param name is the name of the resource
\details Finds the resource within the list of resources
\sa TPercept::GetPerception
*/

TResource* TResourceFactory::GetResource(string name)
{	
		
	TResource* pRes=(TResource*) TPercept::GetPerception(name);

	return pRes;

}









void TResourceFactory::ReduceAllThresholds(double dDeltaTheta)
{
	list<TWorldState*>::iterator pos;
	for(pos = m_pFactoryObjects.begin(); pos != m_pFactoryObjects.end();++pos){
		((TResource*) (*pos))->ReduceActivationLevel(dDeltaTheta);
	}
}



void TResourceFactory::ResetAllResourceUsages()
{
	list<TWorldState*>::iterator pos;
	for(pos = m_pFactoryObjects.begin(); pos != m_pFactoryObjects.end();++pos){

		((TResource *) (*pos))->ResetAmountAvailable();
	}

}
