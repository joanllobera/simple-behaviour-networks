// Timepath_main.cpp : contains the main functions of my project


#include "Timepath_header.h" //contains global defines needed for the different classes, and library includes
#include <time.h>



/*! 


\mainpage Timepath

\section sec_intro Introduction

Timepath is an implementation and extension of Extended Behavioral Networks (EBN).

It is a library useful to develop decisional agents, this is, agents that can decide what action to do in real time. 
It does this in a distributed way, encapsulating skills within modules according to simple rules.
The decision is taken in a distributed way, this is, by spreading the update of the perceptions among the network of modules,
then the decision taken consist in choosing which are the modules that should execute their behavior.
An accurate description of how this is done can be found in Dorer 2004.

The library also implements an extension of EBN in order the rules can also take the form of stories.
This is done considering stories as sequences of causally related events in a complex environment. 


\section start Getting Started

As an example of how to use this library, there is a class TAgent defined, which has several actions. 
What the actions do is send their name to the log file, as well as to a string output.
A good way to get started could be to define rule files and see what is the behavior of TAgent to these inputs.
Another is to define a new agent with new actions, perceptions and resources, and then define the rules and/or stories.

To do this, there are 2 things useful to consider:




\subsection howto How to use the library

The main class of the library is TLlogicNetwork or, if the extension to process stories is not wanted, the class TDorer2Network.
Both classes inherit from TBehaviorNetwork, from which some methods are used.
The way to use it is the following:

1) 
Create a new network with the name of the file where the rules are stored. (see the constructor of class 	TDorer2Network or  TLlogicNetwork)

This will generate a new network. such as:
		TLlogicNetwork  theNet= new TLlogicNetwork(name_of_the_rules_file)
2) 
Add the Actions (see TBehaviorNetwork::AddAction )
This will give instructions such as:

	theNet.AddAction(...

3) 
Add the Perceptions (see TBehaviorNetwork::AddPerception )
This will give instructions such as:

	theNet.AddPerception(...



4)
If wanted, also add the Resources (see TDorer2Network::AddResource )

This will give instructions such as:

	theNet.AddResource(...


5) To parse the rules and connect everything, the network has to be reset (see TBehaviorNetwork::ResetNetwork() )
This will give an instruction such as:

	theNet.ResetNetwork();

\subsection sintax Sintax of the rules
The sintax to use for the rule file is the following:



goal p3 
then p2 0.7 
endgoal 


if not p1 
 then a3  
effect p2 0.8 
using box 2.0
endif 





If stories are wanted, the input given then takes the form:


goal p3 
then p2 0.7 
endgoal 


if not p1 
 then a3  
effect p2 0.8 
using box 2.0
endif 




Llogic:  

S1:

a1. a2. 

if a31 then a32 
if a33 then a34 
if notp then a36 

a4. a5. a6.


\section tech Technical Details

The implementation is done in pure c++, and it also uses some classes of the Standard Template Library.
Therefore, there is no system dependency and the same code could be reused in different operating systems.

\section situation State Of Development

At the present The implementation of the Extended Behavioral Networks is complete.

The extension to process stories is working.

\author <A HREF="http://joan.llobera.name"> Joan Llobera </A>
\date 2014



*/




/*******************************************************************************************************************

			4   T H E     L O G     F I L E


*******************************************************************************************************************/



#if UNITYAGENT==0
string gLogFileName=LOGFILENAME;
ofstream logFile;

int initlog(char *cs){ //the entry point
	//logFile initialisation.
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	logFile.open(gLogFileName.c_str());
	//	logFile.open(gLogFileName.c_str(),ios_base::app);
	logFile << "This is the log file result of running the Timepath library @: " << asctime(timeinfo);

#ifdef CONSOLE_APP
	string s = "This is the log file result of running the Timepath library @: ";
	s = s + asctime(timeinfo);
	printf((char*)s.c_str());
#endif

	ifstream theRuleFile;
	theRuleFile.open(cs);
	if (theRuleFile.is_open())
	{
		logFile << " I am using the rule file: " << cs << "\n";
		flush(logFile);
#ifdef CONSOLE_APP
		string s = " I am using the rule file: ";
		s = s + cs;
		s += "\n";
		printf((char*)s.c_str());
#endif


	}
	else{
		logFile << "sorry, I couldn't find the rule file: " << cs << "\n";
#ifdef CONSOLE_APP
		string s = "sorry, I couldn't find the rule file: ";
		s = s + cs;
		s += "\n";
		printf((char*)s.c_str());
#endif
		return 0;
	}
	return 1;
}

void closelog(){

	logFile.close();
	logFile.open(gLogFileName.c_str(), ios_base::app);
#ifdef CONSOLE_APP
	if (logFile.fail()){ cout << "ERROR in the logfile" << endl; }
	else{ cout << "Log file saved" << endl; }
#endif


	logFile.close();
}
#else

logCallback pLogUnity;

void setUnityLogCallback(logCallback pFn){

	pLogUnity = pFn;

}


#endif


void send2logfile(const char *cs){
	
#ifdef CONSOLE_APP
	printf(cs);
#endif

#if UNITYAGENT==0
	logFile << cs;
#else
	string logString =  cs;//this is necessary, if we pass directly the *cs Unity crashes
	if (pLogUnity != nullptr){
		pLogUnity((char*)logString.c_str());
	}
#endif


}





#ifdef EXTERNALAGENT
//---------------------------------If the agent is defined externally, this header should be the one added:
#include "Timepath_externalagent.h"
#endif



#ifdef INTERNALAGENT

#include "Timepath_internalagent.h" //here are defined the main exported functions that allow using the library

#endif 


#ifdef UNITYAGENT

#include "Timepath_unityagent.h" 

#endif 








