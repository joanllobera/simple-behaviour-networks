
#pragma once


#include "NetworkHeader.h"
#include "Timepath_header.h"

#include "TEnergySpreader.h"


class TPercept;
class TAct;
class TAction;
class TProposition;
class TGoal;
class TConnection;
class TNetworkParams;
class TParseString;
class TWorldState;



/*!

 \brief represents a competence module, an energy spreader of level 2. 
 \date 01.98
 \details  represents the knowledge of an agent containing the three levels of cognition: 
				 cognitively inpenetrable perception and action
				 competence modules
				 goals and goal management

 \sa TDorer2Competence
*/

class TCompetence : public TEnergySpreader  
{
	public:
		TCompetence(TNetworkParams* params, string name, int iNoOfRules);
		virtual ~TCompetence();

		// functions for different algorithms 		
		virtual void	CalculateActivation() {};
		virtual void	CalculateExecutability() {};
		virtual void	CalculateSpreading() {};
		virtual double	GetActivationAndExecutability() { return 0.0; };
		virtual double	GetTransferedActivation() { return m_dTransferedActivation; };

		virtual void	ExecuteModule(double activity);
		virtual bool	CheckForExecution(double dThreshold);
		virtual double	Transfer(double dVal) { return dVal; };
		virtual double	SetToNewActivation();

		void ConnectCompetence(list<TCompetence*> pCompetenceList, list<TGoal*> pGoalList);
	
		virtual void	AddPropositionLink(TProposition* source, TProposition* dest) {};
		virtual void	AddGoalLink(TProposition* source, TProposition* prop) {};
		virtual void	AddProtectedGoalLink(TProposition* source, TProposition* prop) {}; 
		virtual void	AddSuccessor(TProposition* source, TProposition* dest) {};
		virtual void	AddPredecessor(TProposition* source, TProposition* dest) {};
		virtual void	AddConflictor(TProposition* source, TProposition* dest) {};

		// public methods
		bool		ParseRule(string rule, TAct* pAct, TPercept* pPercept);
		void		SetNetworkParams(TNetworkParams* params) { m_pNetworkParams = params; };
		void		ResetActivation() { SetActivation(0.0); };
		

		string	ToString();
		

		bool Instantiate(list<string> objects, list<string> variables, list<string> conditions
									, string action, list<string> effects, TPercept* pPercept);


		// attribute methods

		list<TProposition*>	GetPreconditions() { return m_pPreconditions; };
		list<TProposition*>	GetEffects() { return m_pEffects; };
		list<TConnection*> * GetConflictors() { return ( (list<TConnection*> *) m_pLinks[CONFLICTOR_CONNECTION]); };
	
		double	GetExecutability() { return m_dExecutability; };
		double 	GetExternActivation() { return m_dExternActivation; };

		string	GetResults();
		string	GetResultsInitial() const;
		TAction* GetAction() { return m_pAction; };
		void		SetExecuted(bool exec = TRUE) { m_bExecuted = exec; };


	protected:
		void		AddEnergySource(TConnection* connection, TProposition* srcProp, int type);

		TWorldState*	GetState(string prop, TPercept* pPercept);

		//! executability of the module
		double			m_dExecutability;		
		
		//! activation from situation and goals and previous
		double			m_dExternActivation;	
		
		//! activation after the spreading
		double			m_dSpreadingActivation;	
		int				m_iNoOfSources;

		bool				m_bExecuted;

		vector<list<TConnection*> *>	m_pLinks; 

		//! indices and importances of the preconditions
		list<TProposition*>		m_pPreconditions;

		//! indices and importances of the effects
		list<TProposition*> m_pEffects;

		//! The action that it will trigger
		TAction*			m_pAction;				

		// for activation function
		double			m_dTransferedActivation;
		double			m_dSigmaCompetence;
		double			m_dGainCompetence;

		//! The parameters of the network
		TNetworkParams*	m_pNetworkParams;


	private:
		bool		DecodeNegated(string &str);
		bool		DecodeProposition(TParseString &prop, list<string> objs, list<string> vars);

};

