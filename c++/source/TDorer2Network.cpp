

#include "NetworkHeader.h"
#include "TParseString.h"

#include "TNetworkParams.h"

#include "TProposition.h"
#include "TCompetence.h"
#include "TConnection.h"
#include "TResource.h"
#include "TResourceFactory.h"
#include "TAction.h"
#include "TDorer2Competence.h"
#include "TDorer2Goal.h"

#include "TDorer2Network.h"


TDorer2Network::TDorer2Network()
					: TBehaviorNetwork() 
{
	m_pResourceFactory = new TResourceFactory();
	m_pNetworkParams= new TNetworkParams();
	Initialize();
}


#if UNITYAGENT==0
/*!

 \brief It sets a new Network and points where to check 
 \param pRuleFile array of characters with the name of rule file that will be parsed later on when executing TDorer2Network::ResetNetwork

 \date 03.2010
\sa   TDorer2Network::ResetNetwork

*/

TDorer2Network::TDorer2Network(char *pRuleFile): TBehaviorNetwork(pRuleFile) 
{
	
	m_pResourceFactory = new TResourceFactory();
	m_pNetworkParams= new TNetworkParams();
	Initialize();
}
#endif


TDorer2Network::~TDorer2Network()
{
//	m_pResourceList.clear();
}

void TDorer2Network::Initialize()
{
	m_pNetworkParams->SetGamma(GAMMA_SIGMOID);
	m_pNetworkParams->SetPhi(PHI_SIGMOID);
	m_pNetworkParams->SetDelta(DELTA_SIGMOID);
	m_pNetworkParams->SetBeta(BETA);
	m_pNetworkParams->SetTheta(THETA_SIGMOID);
	m_pNetworkParams->SetThetaReduction(TH_REDUCTION_SIGMOID);
	m_pNetworkParams->SetSigma(0.55);
	m_pNetworkParams->SetGain(5.0);

	TBehaviorNetwork::Initialize();
}


/*!

\brief It creates a new competence of kind TDorer2Competence
\return the new (empty) competence module created
\date 02.2014

\param
\todo
\details
\sa
*/


TCompetence* TDorer2Network::CreateNewCompetence()
{
	return new TDorer2Competence(m_pNetworkParams, "", GetNoOfRules(),(int) m_pGoalList.size());
}


/*!

\brief It creates a new (empty) goal within the network
\return the goal created
\date 02.2014

\param
\todo
\details
\sa
*/

TGoal* TDorer2Network::CreateNewGoal()
{
	TDorer2Goal*	newGoal = new TDorer2Goal();
	return newGoal;
} 


#if UNITYAGENT
/*!

\brief updates the amount of perceptions available
\return
\date 05.2010

\param
\todo
\details  This function updated the amount of resources available (using the callback functions )

This function will trigger an assertion if  the competence modules running actions 
have bound more elements that are actually existing according to the new perception

\sa
*/
void TDorer2Network::UpdateResourcesAvailable()
{



	//we consider no resources are being used:
	this->m_pResourceFactory->ResetAllResourceUsages();
	
	//we consider no action was just executed (otherwise, the module might "give back" the resource when initializing the aciton selection mechanism)
	list<TCompetence*>::iterator pos;
	for (pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end(); ++pos)
	{
		(*pos)->SetExecuted(false);
	}

	//now it is a good time to check whether the amount of resources available changed "in the world". This will check the apropriate callbacks:
	this->m_pResourceFactory->UpdateResourcesAvailable();


}
#endif



/*!

\brief this method updates the perceptions
\return
\date 02.2014

\param
\todo
\details this method updates the perceptions, the available resource and the relevance of the goals
\sa
*/

void TDorer2Network::UpdatePerception()
{ 

		TBehaviorNetwork::UpdatePerception();


	// update all goal relevances
	list<TGoal*>::iterator pos;
	

	m_pGoalList.size();
	for(pos= m_pGoalList.begin();  pos != m_pGoalList.end(); ++pos){

		((TDorer2Goal*) (*pos))->CalculateSituationRelevance();

	}
}



/*!

 \brief adds a resource to the resource factory
 \param name string with the name of the resource
			  pObj pointer to the object that contains the callback function
			  pFn pointer to the callback function that calculates the current amount of resource unit available
			  iAmount amount of resource units needed
 \date 03.2010
 \details adds a resource to the resource factory
  
*/

void TDorer2Network::AddResource(string name, TObject* pObj, PCallBackPerception pFn, double iAmount)
{
	TResource* pResource = m_pResourceFactory->AddResource(name, pObj, pFn, iAmount);
	pResource->SetActivation(m_pNetworkParams->GetTheta());
	pResource->SetMaxActivation(m_pNetworkParams->GetTheta());


#if STORIES
	pResource->SetSpendable(true);
#endif

	//this->m_pResourceList.push_back(pResource);

}

#if UNITYAGENT
/*!

\brief adds a resource to the resource factory
\param name string with the name of the resource
pObj pointer to the object that contains the callback function
iAmount amount of resource units needed
\date 03.2010
\details adds a resource to the resource factory

*/

void TDorer2Network::AddResource(string name, TCallbackConverter* pObj, double iAmount)
{
	TResource* pResource = m_pResourceFactory->AddResource(name, pObj, iAmount);
	pResource->SetActivation(m_pNetworkParams->GetTheta());
	pResource->SetMaxActivation(m_pNetworkParams->GetTheta());


#if STORIES
	pResource->SetSpendable(true);
#endif

	//this->m_pResourceList.push_back(pResource);

}

#endif

/*!

\brief Finds a competence Module by its name
\return the competence module
\date 05.2010

\param  the name of the competence module


*/

TCompetence* TDorer2Network::GetCompetence(string name){
	for (list<TCompetence*>::iterator pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end(); ++pos){
		string temp = (*pos)->GetName();
		if (temp.compare(name) == 0)
			return (*pos);
	}
	return NULL;
}






/*!

\brief A modification of AddResource to get the pointer of the resource created
\return the resource added
\date 05.2010

\param  the name of the resource, the object pObj and the Method pFn used to call it, and the amount iAmount of units initially available


\details

\todo clarify why this function is different from AddResource, and why it does a setTrue to spendable, it is probably related to WHO USES IT (make it easier to grasp)

\sa TLlogicNetwork::AddResource, TResourceFactory::AddResource

*/


TResource* TDorer2Network::AddGetResource(string name, TObject* pObj, PCallBackPerception pFn, double iAmount){

	TResource* pResource = m_pResourceFactory->AddResource(name, pObj, pFn, iAmount);

	pResource->SetActivation(m_pNetworkParams->GetTheta());
	pResource->SetMaxActivation(m_pNetworkParams->GetTheta());

	//this->m_pResourceList.push_back(pResource);


#if STORIES
	pResource->SetSpendable(true);
#endif
	return pResource;

}

#if UNITYAGENT
/*!

\brief A modification of AddResource to get the pointer of the resource created
\return the resource added
\date 02.2014

\param  the name of the resource,
the object pObj handling the callback
and the amount iAmount of units initially available

\details

\sa TLlogicNetwork::AddResource, TResourceFactory::AddResource

*/


TResource* TDorer2Network::AddGetResource(string name, TCallbackConverter* pObj, double iAmount){

	TResource* pResource = m_pResourceFactory->AddResource(name, pObj, iAmount);

	pResource->SetActivation(m_pNetworkParams->GetTheta());
	pResource->SetMaxActivation(m_pNetworkParams->GetTheta());

	//this->m_pResourceList.push_back(pResource);

#if STORIES
	pResource->SetSpendable(true);
#endif
	return pResource;
}

#endif





/*!

\brief pointer to the competence module, that contains the rule to parse
\return
\date 02.2014

\param 
\details calls the parse function of each module. This is needed because ParseRule interface
				 differs for each kind of Network.

*/


bool TDorer2Network::ParseCompetenceRule(TCompetence* pCompetence, string rule)
{
	return ((TDorer2Competence*) pCompetence)->ParseRule(rule, m_pAct, m_pPercept, m_pResourceFactory);
}



/*!

\brief
\return
\date 02.2014

\param pointer to the competence module, that should contain the rule
			  string that contains the rule to parse
\todo
\details We redefine this method to ensure resources are connected to the competence modules (which is not the case in a BehaviorNetwork)
\sa
*/



void TDorer2Network::ConnectCompetence(TCompetence* pNewCompetence){

	((TDorer2Competence*) pNewCompetence)->ConnectCompetence(m_pCompetenceList, m_pGoalList); //redefined to connect also to resources!!
}

/*!

\brief It releases resource binding of all modules
\date 02.2014

*/

void TDorer2Network::InitializeActionSelection()
{

	//in a unity agent, in 1 update loop a unityagent can execute an action once.
	//at the beginning of the update loop, the usages are already reset. 
	//therefore, this function is not needed.
#if UNITYAGENT==0

	TDorer2Competence*	module;

	// reset the resource usage for modules no longer active
	list<TCompetence*>::iterator pos;
	for(pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end(); ++pos)
	{

		module = (TDorer2Competence*) (*pos);
		module->UpdateResourceUsage();  //this releases the units previously being used by the module.
	}
#endif

	//ResetResourceUsage(); 	//this updates the free units after the previous update of the max units available. OVERRIDES THE PREVIOUS, correct only if all actions are done.
}

/*!

\brief  It does the basic action selection
\return true if an action has been selected
\date 02.2014
\details  This function does 2 things: 
first : calculates the activation of each module
second: checks each module for execution



*/



bool TDorer2Network::DoActionSelection()
{
	TDorer2Competence*	module;
	TDorer2Competence*	pExecutedModule = NULL;
	int						iNoOfExecutableModules = 0;
	int						iNoOfExecutedModules = 0;


	//releases all the resources previously used
	InitializeActionSelection();

	// check each module for execution
	list<TCompetence*>::iterator pos;
	for(pos=m_pCompetenceList.begin();pos != m_pCompetenceList.end(); ++pos)
	{
		module = (TDorer2Competence*) (*pos);
		
		// check module for execution
		if (module->CheckForExecution(m_dThreshold))
		{
			// the module has been executed
			iNoOfExecutedModules++;
			pExecutedModule = module;
		

			#ifndef CONCURRENT_ACTIONS
				// stop after the first executed action
				pos = NULL;
			#endif
		}

		// for statistics
		if (module->GetExecutability() > 0.01)
			iNoOfExecutableModules++;
	}


	// statistics

#ifdef STORE_STATISTICS
	m_dAvgNoOfExecutableModules = (m_dAvgNoOfExecutableModules * m_lNoOfActions 
											+ iNoOfExecutableModules) / (double)(m_lNoOfActions + 1L);
	m_lNoOfActions++;
	
	// !!! this does not work anymore with multiple actions
	if ((pExecutedModule != NULL) && (!m_bBlocksworld))
	{
		// calculate the number of action changes
		if (m_pLastAction != pExecutedModule->GetAction())
			m_lNoOfActionChanges++;
		m_pLastAction = pExecutedModule->GetAction();

		if (m_lNoOfActions < MAX_SPREADING_CYCLES)
			m_aiActionExecuted[m_lNoOfActions] = m_pLastAction->GetIndex();
	}
#endif
	if (iNoOfExecutedModules > 0)
		return TRUE;
	else
		return FALSE;
}

/*!

\brief It reduces the threshold of all resources
\date 02.2014

*/


void TDorer2Network::ReduceThreshold()
{
	m_pResourceFactory->ReduceAllThresholds(m_pNetworkParams->GetThetaReduction());
}



/*!

\brief releases the resources used by this module, if it has been executed
\return
\date 02.2014


\sa m_pResourceFactory->ResetAllResourceUsages()
*/


void TDorer2Network::ResetResourceUsage()
{
	m_pResourceFactory->ResetAllResourceUsages();
}





/*!

\brief It calculates one cycle of activation spreading
\return sum of activation in the network
\date 02.2014

\param
\todo
\details  It calculates the new input to the network and the spreading of activation
				 within the network. Only one cycle of activation spreading is executed.
				 It is assumed, that there is real perception available
				 or provided with some logged or fake perception
\sa
*/


double TDorer2Network::DoActivationSpreading()
{
	list<TCompetence*>::iterator pos;
	
	TDorer2Competence*	module;
	double			activity;

	// first calculate all executabilities 
	for(pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end();++pos)
	{
		//module = dynamic_cast<TDorer2Competence*>(*pos);
		module = (TDorer2Competence*) (*pos);

		module->CalculateExecutability();
	}


	// calculate the activation of competences from goals and perceptions
	for(pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end();++pos)
	{
		//module = dynamic_cast<TDorer2Competence*>(*pos);
		module = (TDorer2Competence*)(*pos);

		module->CalculateActivation();
	}


	// calculate and set are separated, so that new activations depend all on the 
	// current activations of modules and not of their new energy
	activity = 0.0;

	for(pos = m_pCompetenceList.begin(); pos != m_pCompetenceList.end();++pos){
		activity += ((TDorer2Competence*) (*pos))->SetToNewActivation();
	}

	return activity;
}



/*!

 \brief It does iNoOfCycles complete perception / decision cycle
 \date 05.2010
 \details It does iNoOfCycles complete perception / decision cycle
  
  It should be noted there is a limit in the number of actions checked to ensure
  that the process ends if there is a patological network defined
  (for example, one without a null action and where it is impossible to select an action)

  \sa TBehaviorNetwork::PerformCycle(), and note there are subtle but important diferences between both

*/

#if UNITYAGENT
bool TDorer2Network::PerformCycle(TNetworkParams* params, int iNoOfCycles)
{
	int	i = 0;
	int j = 0;
	bool	bSelectedAction = FALSE;


	// set the network parameters to the new ones
	m_pNetworkParams->SetNetworkParams(params);


	//we reset the counters. this is because we only want the callbacks to be executed once per call.
	//therefore, in the case the module is executed once in each update, we will consider it CANNOT be reexecuted.

	this->m_pPercept->ResetCallbackCounters();
	this->m_pAct->ResetCallbackCounters();
	this->m_pResourceFactory->ResetCallbackCounters();
	

	


	// update the perceptions and goal relevances
	UpdatePerception();


	//reset all the thresholds of the resources
	UpdateResourcesAvailable();


	do
	{
		
		
		// repeat until an action has been selected
		bSelectedAction = FALSE;

		
		do
		{
			
			ReduceThreshold(); //OK
			

			//In Dorer 2004: steps 1 and 2 (in first list)
			DoActivationSpreading(); //one cycle

			bSelectedAction = DoActionSelection(); 		
			
			
			i++;
		} while (!bSelectedAction && (i <MAX_SPREADING_CYCLES) );
		 

		// reset theta

		
		m_dThreshold = m_pNetworkParams->GetTheta();

		j++;

		// we do not check if we reached the goal because the action does not update instantaneously.
		//bReachedGoal = CheckGoalReached();
	//} while (!bReachedGoal && ( j < iNoOfCycles));
	} while (j < iNoOfCycles);
	return true;
}

#endif
