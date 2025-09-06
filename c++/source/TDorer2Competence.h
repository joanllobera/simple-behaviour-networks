#pragma once


#include "Timepath_header.h"
#include "TCompetence.h"

class TGoal;
class TConnection;
class TNetworkParams;
class TDorer2BaseGoalTrack;
class TScreenParamsNetwork;
class TResourceFactory;
class TResourceProposition;


/*!

 \brief represents a competence module, an energy spreader of level 2. It extends TCompetence with the use of a relevance function and the managment of resources.
 \details  represents the knowledge of an agent containing the three levels of cognition: 
				 cognitively inpenetrable perception and action
				 competence modules
				 goals and goal management

				 If the rules found have resources, it can define an architecture of Behavioral Networks (Dorer 2000)
				 or Extended Behavioral Networks (Dorer 2004), this is, having resources and allowing multiple action selection
 
				Behavioral Networks are themselves an implementation of Maes Agent Network Architecture (ANA) extended
				 to use multivalued propositions.

 \sa   TCompetence TDorer2Network
*/


class TDorer2Competence : public TCompetence  
{
	public:
		TDorer2Competence(TNetworkParams* params, string name, int iNoOfRules, int iNoOfGoals);
		virtual ~TDorer2Competence();

		
		 void	CalculateActivation();
		 void	CalculateExecutability();
		virtual double	GetActivationAndExecutability();
		virtual bool	CheckForExecution(double dThreshold);
		virtual double	Transfer(double dVal);
		virtual double	SetToNewActivation();

		virtual void	ConnectCompetence(list<TCompetence*> pCompetenceList, list<TGoal*> pGoalList);
		virtual void	AddPropositionLink(TProposition* source, TProposition* dest);
		virtual void	AddGoalLink(TProposition* source, TProposition* prop);
		virtual void	AddProtectedGoalLink(TProposition* source, TProposition* prop);
		virtual void	AddSuccessor(TProposition* source, TProposition* dest);
		//virtual void	AddPredecessor(TProposition* source, TProposition* dest);
		virtual void	AddConflictor(TProposition* source, TProposition* dest);
		virtual void	AddResourceLink(TProposition* source, TProposition* dest);

		bool		ParseRule(string rule, TAct* pAct, TPercept* pPercept, TResourceFactory* pResourceFactory);	
		double	GetTransferedActivation(int iGoalIndex);
		void		UpdateResourceUsage();
		double	GetSumTaup() const { return m_dSumTaup; };


	protected:


		void		CalculateExternActivation();

		void		CalculateSpreadingActivation();


		TDorer2BaseGoalTrack*	m_pGoalTrack;
		list<TResourceProposition*>	m_pResources;

		list<TConnection*>			m_pResourceLinks;
		double						m_dSumTaup;
};

