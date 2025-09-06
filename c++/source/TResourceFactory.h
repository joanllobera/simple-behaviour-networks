
#pragma once


#include "Timepath_header.h"
#include "TPercept.h"

class TResource;

/*!

\brief The resource manager
\date 02.2014

\details The resource manager, which is mainly an extension of TPercept
\sa TPercept
*/

class TResourceFactory : public TPercept  
{
	public:
		TResourceFactory();
		virtual ~TResourceFactory();


		TResource*	AddResource(string name, TObject* pObj, PCallBackPerception pCallback, double iAmount);
#ifdef UNITYAGENT
		TResource*	AddResource(string name, TCallbackConverter* pObj, double iAmount);
		void ResetCallbackCounters();
#endif

		void			ReduceAllThresholds(double dDeltaTheta);
		void			ResetAllResourceUsages();


		void UpdateResourcesAvailable();
		
		TResource* GetResource(string name);

};
