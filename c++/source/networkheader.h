#ifndef __NETWORK_HEADER_H
#define __NETWORK_HEADER_H


//#include "resource.h"
//#include "TBehaviorNetwork.h"

//typedef double (CObject::*PCallBackPerception)( void ); //how will you redefine this???
//typedef void (CObject::*PCallBackAction)( double );

// defined when history of activation is needed (needs lots of space and time)
//#define xACTIVATION_HISTORY
#define MAX_HISTORY 6000

//#define xACTIVATION_DISTRIBUTION
#define NO_OF_SLOTS 10

// border under which perception values will be round to zero
#define FUZZY_ROUND 0.1

//For goal preconditions: if 1, then and is fuzzy and is min, otherwise it is product
#define FUZZY_MIN_AND 1

//For product of activation and executability:
#define FUZZY_MIN_AND_MODULE 0

#if FUZZY_MIN_AND_MODULE==0

#define FUZZY_SQRT_AND 1
#endif



// defined for noise on perception nodes
#define xINPUT_NOISE_ON
// percent of noise to input values (before rounding to non fuzzy if non fuzzy)
#define INPUT_NOISE_VALUE	0.7

// defined for noise on perception (distance, direction)
#define xPERCEPTION_NOISE_ON
// percent of noise to distance and direction
#define PERCEPTION_NOISE_VALUE	0.2

// defined for noise on kick (power, direction)
#define xACTION_NOISE_ON
#define PERCENTUAL_NOISE_ON

// defines for Dorer2
//#define NO_TRANSFER_FUNCTION
#define NO_GOALTRACKING
//#define NO_ACTION_PARAMETER
#define CONCURRENT_ACTIONS

// parameters for the networks
#define NO_OF_CONNECTION_TYPES			6
#define SITUATION_CONNECTION			0
#define GOAL_CONNECTION					1
#define PROTECTED_GOAL_CONNECTION		2
#define SUCCESSOR_CONNECTION			3
#define PREDECESSOR_CONNECTION			4
#define CONFLICTOR_CONNECTION			5

// strings for rules
#define NEWLINE "\r\n";

//skills:
#define STRING_IF "if:"
#define STRING_AND "and"
//#define STRING_THEN "then"
#define STRING_THEN_SKILL "do:"
#define STRING_EFFECT "effect:"
#define STRING_NOT "not"
#define STRING_USING "using:"
#define STRING_END_IF "endskill"

//goals:
//#define STRING_GOAL "goal"
#define STRING_GOAL "when:"
#define STRING_THEN_GOAL "iwant:"
#define STRING_END_GOAL "endgoal"






#define MODULE_SIZE 5.0

#ifndef D_ZERO_P
	#define D_ZERO_P		0.0000001
#endif

#ifndef D_ZERO_N
	#define D_ZERO_N		-0.0000001
#endif


#endif //__NETWORK_HEADER_H