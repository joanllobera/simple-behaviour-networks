
#pragma once

#include "Timepath_header.h"

#include "TConnection.h"

class TProposition;
class TEnergySpreader;
class TNetworkParams;


/*!

 \brief class to treat Dorer connections between states of the world and Preconditions. Modifies TConnection
 \date 2010
\todo check if all the connection classes cannot be made more compact, in one single class 
 \sa  TConnection
*/


class TDorer2SituationLink : public TConnection
{
	public:
		TDorer2SituationLink(TNetworkParams* params, TProposition* src, TProposition *dest);

		
		virtual double		GetActivation();
		virtual void		SetMaxActivation();
};


/*!

 \brief class to treat Dorer connections between Effects and Goals. Modifies TConnection
 \date 2010
 
 \sa  TConnection
*/
class TDorer2GoalLink : public TConnection
{
	public:
		TDorer2GoalLink(TNetworkParams* params, TProposition* src, TProposition *dest);

		virtual double		GetActivation();
		virtual void		SetMaxActivation();
};


/*!

 \brief to treat Dorer connections between Effects and Negated Goals. Modifies TConnection
 \date 2010
 
 \sa  TConnection
*/
class TDorer2ProtectedGoalLink : public TConnection
{
	public:
		TDorer2ProtectedGoalLink(TNetworkParams* params, TProposition* src, TProposition *dest);

		virtual double		GetActivation();
		virtual void		SetMaxActivation();

};


/*!

 \brief to treat Dorer connections among successive modules. Modifies TConnection
 \date 2010
 
 \sa  TConnection
*/
class TDorer2SuccessorLink : public TConnection
{
	public:
		TDorer2SuccessorLink(TNetworkParams* params, TProposition* src, TProposition *dest);

		virtual double		GetActivation();
		virtual void		SetMaxActivation();
		double	GetGoalActivation(int iGoalIndex);
};


/*!

 \brief class to treat Dorer connections among successive and conflicting modules. Modifies TConnection
 \date 2010
 
 \sa  TConnection
*/
class TDorer2ConflictorLink : public TConnection
{
	public:
		TDorer2ConflictorLink(TNetworkParams* params, TProposition* src, TProposition *dest);

		virtual double		GetActivation();
		virtual void		SetMaxActivation();
		double	GetGoalActivation(int iGoalIndex);
};


/*!

 \brief class to treat Dorer connections between the usage of resources and world states of resources. Modifies TConnection
 \date 2010
 
 \sa  TConnection
*/
class TResourceLink : public TConnection
{
	public:
		TResourceLink(TNetworkParams* params, TProposition* src, TProposition *dest);

};

