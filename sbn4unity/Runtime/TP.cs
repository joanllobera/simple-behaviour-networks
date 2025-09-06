using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using System;


/*!

 \brief A singleton used to manage the list of agents 
 \date 02.2014
 \details  A singleton used to manage the list of agents. The position in the list will be netID


	\todo make sure this is the only class that needs to be used to define any perception or action
 \sa TPAgent TPAgentNetwork TPPersonality
*/
namespace timepath4unity
{
    public class TP
    {

        public const string Version = "f1.0";


        //built at runtime.
        private List<TPAgent> agents = new List<TPAgent>();

        private static TP instance;
        private TP() { }

        //! a strange singleton: this is protected instead of being public. The instance is created when we add the first agent to it. We made it private to make sure it is only accessed through other TP methods (usually static)
        protected static TP Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new TP();
                }
                return instance;
            }
        }

        public static void AddAgent(TPAgent a)
        {
            TP.Instance.agents.Insert(a.NetID, a);
        }

        public static TPAgent GetAgent(int mindID)
        {

            TPAgent a = TP.Instance.agents[mindID];
            return a;
        }





        //! we find all the agents that have this personality, and check they are all instantiated. otherwise, we return an empty list	
        public static List<TPAgent> getAgentsWithPersonality(TPPersonality ebn)
        {

            List<TPAgent> agentsWithPersonality = new List<TPAgent>();
            foreach (TPAgent a in TP.Instance.agents)
            {
                if (a.MyPersoTemplate.Equals(ebn))
                {
                    if (a.MyPerso)
                    {
                        agentsWithPersonality.Add(a);
                    }
                    else
                    {
                        agentsWithPersonality.Clear();
                        break;
                    }
                }
            }
            return agentsWithPersonality;
        }

    }



}