

#include "TLlogicCompetence.h"
#if STORIES

#include "TNetworkParams.h"

#include "TDorer2Connection.h"
#include "TDorer2Competence.h"

#include "TResourceProposition.h"
#include "TResource.h"
#include "TResourceFactory.h"

#include "TActionResource.h"

#include "TDorer2Goal.h"

#endif


TLlogicCompetence::TLlogicCompetence(TNetworkParams* params, string name, int index, int iNoOfGoals)
		:TDorer2Competence(params, name, index, iNoOfGoals)
{
#if STORIES	
	UpdateTriggerTime=FALSE;
#endif
}



TLlogicCompetence::~TLlogicCompetence(void)
{
}


#if STORIES


/*!

\brief It adds an effect and connects it
\date 02.2014

\todo track changes concerning TDorer2Goal and TGoal
*/

void TLlogicCompetence::AddEffect(TProposition* pProp, list<TCompetence*> pCompetenceList, list<TGoal*> pGoalList){
	
list<TProposition*>::iterator pos;

	bool isequal=FALSE; //first, it ensures the effect hasn't been added already
	for(pos=m_pEffects.begin();pos!=m_pEffects.end();++pos){
		if((*pos)==pProp) 
			isequal=TRUE;
	}
	if(!isequal){
			m_pEffects.push_back(pProp);

		// connections due to the effect (as in TCompetence::connect)
	
			bool bInhibition = FALSE;
			bool bExcitation = FALSE;
	
			TProposition*		pSource;
			TProposition*		pDest;
			TCompetence*		pCompetence;
			TGoal*				pGoal;

			list<TProposition*>				pPreconditions;
			list<TProposition*>				pEffects;


			list<TCompetence*>::iterator	pos2;	//competence modules
			list<TProposition*>::iterator	pos3;  //effects
			list<TGoal*>::iterator	pos4;  
			

			pDest=pProp;

			for(pos2=pCompetenceList.begin(); pos2 != pCompetenceList.end(); ++pos2)
			{
				pCompetence = (TCompetence*) (*pos2);
				pPreconditions = pCompetence->GetPreconditions();

				// go through the preconditions of the module

				for(pos3=pPreconditions.begin(); pos3 != pPreconditions.end(); ++pos3)
				{
					pSource = (TProposition*) (*pos3);
				
					// is it a successor
					if (pSource->IsIdentic(pDest))
					{
						AddSuccessor(pSource, pDest);
						pCompetence->AddPredecessor(pDest, pSource);
						bExcitation = TRUE;
					}
					else if (pSource->IsInvers(pDest))
					{
						AddConflictor(pSource, pDest);
						bInhibition = TRUE;
					}
				}
			} //end for pos2


			// add goals, protected goals
			for(pos4=pGoalList.begin(); pos4 != pGoalList.end(); ++pos4)
			{
				pGoal = (TGoal*) (*pos4);;
				pSource = pGoal->GetCondition();
				if (pSource->IsIdentic(pDest))
				{
					AddGoalLink(pSource, pDest);
					bExcitation = TRUE;
				}
				else if (pSource->IsInvers(pDest))
				{
					AddProtectedGoalLink(pSource, pDest);
					bInhibition = TRUE;
				}
			}	//end for pos4

			
	} //end if
}



/*!

\brief This method adds a precondition and connects it */
void TLlogicCompetence::AddPrecondition(TProposition* pProp, list<TCompetence*> pCompetenceList){

	// connections due to the precondition added
	list<TProposition*>::iterator pos;

	bool isequal=FALSE; //first, it ensures the effect hasn't been added already
	for(pos=this->m_pPreconditions.begin();pos!=m_pPreconditions.end();++pos){
		if((*pos)==pProp) //! \todo check this is right
			isequal=TRUE;
	}
	if(!isequal){
			m_pPreconditions.push_back(pProp);


			bool bInhibition = FALSE;
			bool bExcitation = FALSE;
	
			TProposition*		pSource;
			TProposition*		pDest;
			TCompetence*		pCompetence;

			list<TProposition*>				pPreconditions;
			list<TProposition*>				pEffects;


			list<TCompetence*>::iterator	pos2;	//competence modules
			list<TProposition*>::iterator	pos3;  //effects


		 pDest = pProp;

		// add as energy source
		AddPropositionLink(pDest->GetWorldState()->GetProposition(), pDest);

	// look for predecessor and conflictor links
		for(pos2=pCompetenceList.begin(); pos2 != pCompetenceList.end(); ++pos2)
		{
			pCompetence = (TCompetence*) (*pos2);
			pEffects = pCompetence->GetEffects();

		// go through the effects of the module
			for(pos3=pEffects.begin(); pos3 != pEffects.end(); ++pos3)
			{
				pSource = (TProposition*) (*pos3);
				if (pSource->IsIdentic(pDest))
				{
					// is it a predecessor
					pCompetence->AddSuccessor(pDest, pSource);
					//! \todo recheck what this means
				}
				else if (pSource->IsInvers(pDest))
				{
					// the other module is a conflictor of the new one
					pCompetence->AddConflictor(pDest, pSource);
					//! \todo !!! this is not absolutely correct,  because it might have been added for this  proposition before
				}
			}
		}
	}
}




/*!

\brief This method adds a resource and connects it */



void TLlogicCompetence::AddResource(TResourceProposition* res){

	m_pResources.push_back(res);

	TResourceProposition*	pProp;
	pProp = res;
	TWorldState* TWS=pProp->GetWorldState();
	TProposition* TP=TWS->GetProposition();
	AddResourceLink(TP, pProp);

}


/*!

 \brief it executes the action and updates its possibility value if necessary
 \return (void) 
 \date 06.2010
\details 
It executes the action and updates its possibility value if necessary. This function has been redefined to ensure 
that once the action is executed to also update the trigger times of the actionResources forming the story and this is realised by the system

 \param  the level of activation assosiated with the action

 \sa TCompetence::ExecuteModule

 \todo now the action will be triggered only once. A modification should be introduced in order there is the option to trigger during the whole duration of the action.
*/

void TLlogicCompetence::ExecuteModule(double activation)
{

	bool reallydoit=TRUE;

	TActionResource* act=GetAction()->getActRes();
	if(act != NULL)
		reallydoit= act->Check4Execution(); //should it really be triggered or be considered to have been done by someone else?
	if(reallydoit){ 
			// execute action
		m_pAction->PerformAction(activation); 
		m_bExecuted = TRUE;
	}
}



/*!

 \brief  It updates the amount of a particular resource that is expected to be needed to execute the action. defined by a string
 \return TRUE if the resource was found
 \date 06.2010


 \param  u the amount expected to be used and s the name of the resource involved


*/


bool TLlogicCompetence::UpdateResourceNeeded(double u,string s){
	bool b=FALSE;
	list<TResourceProposition*>::iterator pos;
	
	for(pos=m_pResources.begin();pos!=m_pResources.end();++pos){
		string name =((TEnergySpreader*) ((TProposition*) (*pos))->GetWorldState())->GetName();
		if(s.compare(name)== 0){
			(*pos)->SetAmountUsed(u);
			b= TRUE;
			pos=m_pResources.end();
			pos--;
		}
	}
	return b;
}



/*!

 \brief  It returns the amount of a particular resource expected to be needed by the module to execute the action.
 \return the amount of a particular needed
 \date 06.2010

 \param   s the name of the resource involved


*/

double TLlogicCompetence::GetResourceNeeded(string s){

	list<TResourceProposition*>::iterator pos;
	
	double amount;
	for(pos=m_pResources.begin();pos!=m_pResources.end();++pos){
		string name =((TEnergySpreader*) ((TProposition*) (*pos))->GetWorldState())->GetName();
		if(s.compare(name)== 0){
			amount=(*pos)->GetAmountNeeded();
			return amount;
		}
	}
	return -1; //in case it is not found...
}

#endif