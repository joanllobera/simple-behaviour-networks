
#include "Timepath_header.h"



#pragma once



#include "TAct.h"
class TPercept;
class TAction;
class TCompetence;
class TNetworkParams;
class TParseString;

class TEnergySpreader;
class TGoal;

#if UNITYAGENT
class TCallbackConverter;
#endif


/*!

 \brief represents the knowledge of an agent containing the three levels
				 of cognition:
 \date 06.2010
 \details  It represents the knowledge of an agent containing the three levels
				 of cognition: 
				 cognitively inpenetrable perception and action
				 competence modules
				 goals and goal management
			 It works with simple goals of class TGoal and simple competence modules of class TCompetence.
			 The goals have no relevance conditions, and the Competence Modules have no resources.

 \sa TDorer2Network TLlogicNetwork
*/
class TBehaviorNetwork : public TObject
{
	public:
		TBehaviorNetwork();
		TBehaviorNetwork(char *pRuleFile);
		virtual ~TBehaviorNetwork();

#if USEBEHAVIORNETWORKSWITHOUTRESOURCES
		bool	PerformCycle(TNetworkParams* params, int iNoOfCycles);	
#endif
		char*	m_RulesFileName;

		//! Adds a new perception to the network 
		void	AddPerception(string name, TObject* pObj, PCallBackPerception pFn);

		//! Adds an action  to the network 
		void	AddAction(string name, TObject* pObj, PCallBackAction pFn);

#if UNITYAGENT==0
		virtual void	ResetNetwork();
#else
		virtual void	ResetNetwork(string rules);

		//! Adds a new perception to the network 
		void	AddPerception(string name, TCallbackConverter* pObj);

		//! Adds an action  to the network 
		void	AddAction(string name, TCallbackConverter* pObj);

#endif
		
		TNetworkParams* GetNetworkParams();



	protected:
		virtual void	Initialize();
		int				GetNoOfRules() const;

		TAction* GetAction(string name){ return m_pAct->GetAction(name); };

		virtual void	UpdatePerception();
		virtual bool	ParseRules(string rules);
		


		virtual TCompetence*	CreateNewCompetence();
		virtual TGoal*	CreateNewGoal();
#if USEBEHAVIORNETWORKSWITHOUTRESOURCES
		//virtual void			CalculateSpreading(){};
#endif
	

		void			DeleteNetwork();
		virtual void	ConnectCompetence(TCompetence* newCompetence); //! IMPORTANT: it should stay virtual (it changes with the kind of network used

		bool			CheckGoalReached();


		TAct*				m_pAct;				// action factory
		TPercept*			m_pPercept;			// perception factory
		list<TGoal*>		m_pGoalList;	
		list<TCompetence*>	m_pCompetenceList;
		TNetworkParams*		m_pNetworkParams;

		double			m_dThreshold;
private:
		virtual bool	ParseCompetenceRule(TCompetence* pCompetence, string rule);
#if USEBEHAVIORNETWORKSWITHOUTRESOURCES
		virtual double	DoActivationSpreading();
#endif
		virtual void	InitializeActionSelection() {};
		virtual bool	DoActionSelection();
		virtual void	ReduceThreshold();
		void			SetThreshold(double val);
		void			InsertSorted(list<TCompetence*> thelist, TCompetence* module);
	
};

