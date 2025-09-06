
/*! 


\mainpage TP4Unity

\section sec_intro Introduction

TP4Unity (perceptionID.e., Timepath 4 Unity) is a set of customized Unity editors built on top of a C++ Library


It is a library useful to develop decisional agents, this is, agents that can decide what action to do in real time. 
It does this in a distributed way, encapsulating skills within modules according to simple rules. 
The decision is taken in a distributed way, this is, by spreading the update of the perceptions among the network of modules,
then the decision taken consist in choosing which are the modules that should execute their behavior.
Such an arquitecture is named Extended Behavior Networks (EBN). An accurate description of how this is done can be found in Dorer (2004).

Its main function is not only to facilitate the creation of artificial intelligence (AI) for virtual characters,
and use these characters to develop Immersive Stories such as described in Llobera et al. (2013).
The library also implements an extension of EBN in order the rules can also take the form of stories.
This is done considering stories as sequences of causally related events in a complex environment. 


\section start Getting Started

Each character using this library must have a TPAgent associated to it.

Each TPAgent has a personality and a set of possible Roles to adopt within one or several stories.





\subsection howto How to use the library



\section paths Folder organization
\todo rewrite this section.

so far, the Time Path scripts introduced are in:

Assets/Timepath

TPAgent (a particular personality, stories and roles)



./Assets/Timepath/Editor

contains all the GUI code. Concretely:
 
Assets/Timepath/Editor/uSequencer

TPEvent, a class used to draw TP events in the usequencer events timeline

Assets/Timepath/Editor/Inspector

TPAgentInspector, a custom outline of how a TPAgent looks within a character





\section tech Technical Details

The implementation of the decisionals rules is done in pure c++, and it also uses some classes of the Standard Template Library.
Therefore, there is no system dependency and the same code could be reused in different operating systems.

\section situation State Of Development

At the present The implementation of the Extended Behavioral Networks is complete.
The extension to process stories is working. The integration within Unity is in progress

\todo complete the integration of unity and timepath

\todo retest exhaustively the copying and moving of skills between personalities. The construction of the trees still triggers bugs (and it seems to create perceptions in top of hierarchy)

\todo finish teh adaptation of the Skill inspector,
\todo adapt the personality inspector and the agent inspector for the realtime execution situation. remove the personalitytemplate from the inspector in the execution.
\todo in the list action methods there is still a getAction method.
\todo the display of the activations of goals and skills is still wrong.
\todo change the labels of goals and skills, they should list the goals and resources and explicitly outline the rule they involve. It should possible to sort them as wished.
\todo check what changes in the decision-making when one goal is COMPLETED (it should change something, and I think it does not). possible change in timepathcore
\todo resource amount in Skill should be EDITABLE

\todo: change DrawSkills in TPPersonalityBoxBase in order that: the number shown is activation  (we are showing the decisiveness already in the slider), and that when an action is selected IT BLINKS


\author <A HREF="http://joan.llobera.name"> Joan Llobera </A>
\date Autumn 2015



*/





