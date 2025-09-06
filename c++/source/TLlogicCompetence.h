#pragma once

#include "Timepath_header.h"



#include "TDorer2Competence.h"
#include "TActionResource.h"

class TDorer2Goal;

/*!

 \brief represents a competence module, an energy spreader of level 2. It extends TDorer2Competence
 \date 06.2010
 \details  The functionalities added to the behavior network are just the ability to add on-the-fly conditions and effects to the module, connecting them with the rest of the network.
 This is a feature very useful when defining stories that involve actions already used within previous rules.

 \sa TDorer2Competence
*/
class TLlogicCompetence :public TDorer2Competence 
{

public:
	//methods redefined:
	TLlogicCompetence(TNetworkParams* params, string name, int index, int iNoOfGoals);
	~TLlogicCompetence(void);
#if STORIES

	//functions used when parsing or updating stories:

	//! Adds an effect to the competence module and connects it within the network
	void AddEffect(TProposition* pProp, list<TCompetence*> pCompetenceList, list<TGoal*> pGoalList);
	

	// ! Adds a precondition to the competence module and connects it within the network
	void AddPrecondition(TProposition* pProp, list<TCompetence*> pCompetenceList);


	//! Adds a resource to the competence module and connects it within the network
	void AddResource(TResourceProposition* res);

	
	virtual void ExecuteModule(double activation); 

	//new methods:

	bool UpdateResourceNeeded(double u,string s);
	double GetResourceNeeded(string name);

	void setUpdateTriggerTime(bool b){UpdateTriggerTime=b;};

protected:



	bool UpdateTriggerTime;
#endif


};

