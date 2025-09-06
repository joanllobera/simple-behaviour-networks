
/*! \file *********************************************************************************

This file contains stuff you want to be included in general, like general defines, log file capabilities, etc.

*******************************************************************************************/
#pragma once


#if _WIN32
#define EXPORT_API extern "C" __declspec(dllexport)
#else
#define EXPORT_API
#endif

#define _CRT_SECURE_NO_WARNINGS


#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <vector> //just for m_pLinks , and for the different minds in unityAgent
#include <math.h>


using namespace std;

#define TRUE true
#define FALSE false




#include "TObject.h" 




/*!******************************************************************************************

@defgroup GENERALITIES General Macros 

@{


\page General  General Macro definitions


\section Introduction
  These macros change very general aspects of the project

There are 3 possible ways to connect an Agent with the decision network:

If INTERNALAGENT is defined, the dll will have functions to call directly the agent
defined in the dll (see also TAgent and Timepath_export_header.h). It can be threaded, or not.

If EXTERNALNET is defined, an external agent can be created, and the
dll will have the functions defined in Timepath_externalnet.h, which are the main ones needed to define
an extended behavior network with stories added to it.
The tricky part is that we are using pointers to class methods
(see definition of PCallbackPerception and PCallbackAction)

If UNITYAGENT is defined, the external agent will be a unity agent defined in C#. The callback functions
will be of 2 kinds: pointers to functions, which in C# will be delegates, and the same that with EXTERNALNET
 \sa see definition of PCallbackPerception and PCallbackAction

 \todo review in detail the mechanism for activation spreading. Check every detail and confirm it is precise, correct, and as fast as possible


*******************************************************************************************/


#define USEBEHAVIORNETWORKSWITHOUTRESOURCES 0

#define EXPORTTESTFUNCTIONS 0


#if USEBEHAVIORNETWORKSWITHOUTRESOURCES


#define HORIZON_SPREADING_CYCLES 100
#endif

//! #INTERNALAGENT If INTERNALAGENT is defined, the dll will have functions to call directly the agent defined in the dll(see TAgent.cpp and Timepath_export_header).It can be threaded, or not.
//#define INTERNALAGENT

//! #EXTERNALAGENT is a net ready to be called by an external agent, by default by an external C++ agent. 
//#define EXTERNALAGENT 

//! #UNITYAGENT if #UNITYAGENT is defined as 1 the agent is declared in C# within Unity. This implies the callbacks are C function pointers (pointing to C# functions), and not C++ function pointers \sa TCallbackConverter
#define UNITYAGENT  1

#if UNITYAGENT
typedef void(*logCallback)(char*);


//! for the functions declared in Unity and C# we use:
typedef double(*UnityPerception)(int, int);


typedef void(*UnityAction)(int,int,double,double);//agentID,mindID,decisiveness, activation

#endif



typedef double(TObject::*PCallBackPerception)(void);
typedef void(TObject::*PCallBackAction)(double);






#define SEND2LOG( char ) send2logfile(char );

void send2logfile(const char *cs);


//the files where to log to and read rules from:

#define DEFAULTRULEFILENAME "Timepathdefaultrulefile.txt"
//#define LOGFILENAME "Timepathlogfile.txt"
#define LOGFILENAME "TimepathNetMessages.txt"

#define VERBOSE 0 //a bit more information within the parsing and executing procedures
//VERBOSE 2 will reflect the values of the callback perceptions




#define CONSOLE_APP //to show logfiles through the console app



#define HORIZON_COMPLETE_CYCLES 10 
//how many spreading cycles is appropriate? how many modules far away can be the goal?


#define MAX_SPREADING_CYCLES 10
//If we look in the relation between THETA and THRESHOLD_REDUCTION is 10%. Therefore 10 seems fine, since 10x10% = 100%
// THETA_SIGMOID and its threshold hold the same relation.



#define MAXPERCEPTIONS 128 //the maximum of perceptions listed in the agent


#ifdef INTERNALAGENT
//#define THREADED //to run the update of the network in a separate thread
#endif




/*****************************************************************

		T H E     N E T W O R K


******************************************************************/


//! NDEBUG disables all the assertions
//#define NDEBUG //this 
#include <assert.h>
#define ASSERT(int) assert(int);

//! \def DORERGOALSHAVEEFFECTS Do we want goals to have effects??  If some day goals have effects, this should be defined (check TDorer2Goal). By default, it should be commented. Note it is not clear how effects can influence the relevance of goals
//#define DORERGOALSHAVEEFFECTS 
#define WARNINGIFNOTDIRECTENDGOAL 0


/** @} end the group generalities */



/*! ****************************************************************
		T H E     S T O R I E S

@defgroup STORIES Defines for Stories

@{
******************************************************************/

//! DEBUGWITHTIMESTEPS to have time increments of 0.5 seconds at each iteration. For debugging

//#define DEBUGWITHTIMESTEPS 

//! \def REASONABLE_CONDITION_SATISFACTION 0.7 ,used when debugging out from the behavior network

//#define REASONABLE_CONDITION_SATISFACTION 0.7 


//! STORIES An extension to Extended Behavior Networks allowing to process stories
#define STORIES 0

#if STORIES



/*!

\brief   defining ONLYONCE will make  a story action be triggered only once.
\return
\date 02.2014

\param
\details

defining ONLYONCE will make  a story action be triggered only once.
If it is not defined, the function updatePos will reset the value and
the action will be triggered with an activation value while in the right interval.

The first is useful if the actions are executed only ones, the second if it is necessary to refresh regularly their execution

\sa   TActionResource::update, and TActionResource::isPAst
*/
#define ONLYONCE 



//added to the strings that are in networkheader.h
#define STRING_START_STORY "Llogic:" 
#define STRING_DEFAULT_CONTEXT "K"


#define STRING_POINT "\." //this is how to define a point
#define STRING_COLON "\:" 



#define STRING_THEN_PLOT "then" //not used, should check if it works well with the one defined in networkheader
#define STRING_NONE_OF_PREVIOUS "notp"
#define STRING_ATP "ATP"
#define STRING_ESTIMATOR "EST"
#define STRING_ROLE "ROLE"

//!the default minimum time between 2 actions of the story
#define DEFAULT_MINDELAY 4.0  

#define DEFAULT_MAXDELAY 6.0
#define DEFAULT_MEANDELAY 5.0

//! the expected use of actionresource context before the time to trigger the action arrives. Any value > than 1
#define MAX_TAU_U 1.0 




#define STORYEND "THE_END"

/** @}  end the group stories*/


#endif //#if STORIES
