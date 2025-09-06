#pragma once

#include "Timepath_header.h"
#include "TBehaviorNetwork.h"

class TCompetence;
class TNetworkView;
class TRuleView;

class TDorer2Goal;
class TPercept;
class TResource;

#include "TResourceFactory.h"


/*!

 \brief It represents the knowledge of an agent. It extends TBehaviorNetwork 
 \details  represents the knowledge of an agent containing the three levels of cognition: 
				 cognitively inpenetrable perception and action
				 competence modules
				 goals and goal management

				 It extends TBehaviorNetwork  using TDorer2Goal, which has relevance functions (see Behavioral Networks (Dorer 2000))
				 If the rules found have resources, it uses TResource and it will define an architecture like Extended Behavioral Networks (Dorer 2004).
				 This implies allowing multiple action selection
 
				Behavioral Networks are themselves an implementation of Maes Agent Network Architecture (ANA) extended	 to use multivalued propositions.

 \sa TLlogicNetwork TBehaviorNetwork TDorer2Competence
*/

class TDorer2Network : public TBehaviorNetwork
{
	public:
	
		TDorer2Network();
#if UNITYAGENT==0
		TDorer2Network(char *pRuleFile);
#endif
		virtual ~TDorer2Network();

		
		void			AddResource(string name, TObject* pObj, PCallBackPerception pFn, double iAmount);
#if UNITYAGENT
		bool	PerformCycle(TNetworkParams* params, int iNoOfCycles);
		void			AddResource(string name, TCallbackConverter* pObj, double iAmount);
#endif



		TResource* GetResource(string name){return m_pResourceFactory->GetResource(name);};


		//-----------------------------------------------------------------------------------------------------------------------------

		//access to stuff existing in Dorer2Network:

		

		TCompetence* GetCompetence(string name);

		TResource*	AddGetResource(string name, TObject* pObj, PCallBackPerception pFn, double iAmount);

#if UNITYAGENT
		TResource*	AddGetResource(string name, TCallbackConverter* pObj, double iAmount);
#endif





protected:
	virtual bool	ParseCompetenceRule(TCompetence* pCompetence, string rule);
	void			ConnectCompetence(TCompetence *pNewCompetence);

	virtual TGoal*			CreateNewGoal();


	//! pointers to the actual resources,
	//list<TResource*>	m_pResourceList; 

	//! lists of resources as they are used within the network
	TResourceFactory*	m_pResourceFactory;	

private:
		virtual void	Initialize();

		virtual void	UpdatePerception();

#if UNITYAGENT
	
		void	UpdateResourcesAvailable();
#endif


		virtual TCompetence*	CreateNewCompetence();

		virtual void	InitializeActionSelection();
		virtual double	DoActivationSpreading(); 	
		virtual bool	DoActionSelection();

		virtual void	ReduceThreshold();

		void				SetUseFuzzyValues(bool bVal);
		bool				GetUseFuzzyValues() const;

	private:
		void	ResetResourceUsage();
};

