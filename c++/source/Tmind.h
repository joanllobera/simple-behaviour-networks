#include "Timepath_header.h";
#include "TLlogicNetwork.h";
#include "TDorer2Network.h";

#pragma once

/*!

\brief A class containing the different elements forming the mind of an agent

\date 02.2014

\todo
\details  A class containing the different elements composing the mind of an agent.
		This class is only used if #UNITYAGENT is defined.
		This class also assumes that #TLlogicNetwork is available and that #STORIES is defined

\sa
*/




class TMind
{
	TLlogicNetwork* theNet;
	
	//! an integer saying where it is placed in the vector containing them 
	int mindID;

	//! a unique string given at the constructor
	string uniqueID;

public:
	TLlogicNetwork* getNet(){ return theNet; };

	TMind(string uniqueID);
	~TMind();

	void seTMindID(int val){ mindID = val; };


};

