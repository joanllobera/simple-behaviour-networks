using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/*!

 \brief A TimePath story plot
 \date 02.2014
 \todo make plots work
 
*/

namespace timepath4unity
{

#if TRAMA

    public class TPPlot : MonoBehaviour
    {

        [SerializeField]
        List<TPPlotSentence> plot = new List<TPPlotSentence>();

        [SerializeField]
        List<TPRole> roles = new List<TPRole>(); //! \todo since this class will probably be exposed, it might be better, if possible, to store strings


        public List<TPPlotSentence> Plot
        {
            get
            {
                return plot;
            }
        }


        //! Role getter. 
        public TPRole[] getPlotRolesFromGlobalList()
        {

            //! \todo it rechecks we have all the roles right. Not efficient, but secure. maybe there is another way?
            this.roles = new List<TPRole>(TPRole.getRolesInPlot(this));
            return roles.ToArray();

        }



        public string[] RoleNames
        {
            get
            {
                List<string> roleNames = new List<string>();
                foreach (TPRole r in this.roles)
                    roleNames.Add(r.name);
                return roleNames.ToArray();
            }
        }


        /*
        public  string[] getRoles(){
            List<string> roleNames = new List<string>();
            foreach(TPRole r in roles)
                roleNames.Add(r.name);

            return roleNames.ToArray();
	
        }*/


        public bool hasRole(TPRole r)
        {
            return roles.Contains(r);

        }







        /*

        public void rebuildTransformsHierarchy()
        {
            if (this.Plot.Count > 0)
            {
                this.Plot[0].transform.parent = this.transform;
                Transform lastParent = this.Plot[0].transform;
                Transform currentTransform = null;
                for (int i = 1; i < this.Plot.Count; i++)
                {
                    currentTransform = this.Plot[i].transform;
                    currentTransform.parent = lastParent;
                    if (!this.Plot[i].isExternal())
                        lastParent = currentTransform;
                }
            }
        }

        */



        //THIS IS NOT USED!!
        void AddPlotSentenceInPersonality(TPPlotSentence sentence, int mindID)
        {

            TPAgent agent = TP.GetAgent(mindID);
            if (sentence.PreCon)
            {
                Debug.LogError("TP: we need to implement external story actions");
            }  //! \todo add support for external actions,

            sentence.name = sentence.getNameWithoutSpaces();
            //JL2016
            //TPAction a = sentence.GetComponent<TPAction>();
            TPActionBase a = sentence.GetComponent<TPActionBase>();

            /*
            if (a)
            {
                Transform skillContainer = TPSkillList.GetSkillContainer(this);

                GameObject goSkill = new GameObject();
                TPSkill temp = goSkill.AddComponent<TPSkill>();
                goSkill.transform.parent = skillContainer;
                goSkill.name = name;


                GameObject go = new GameObject();
                go.AddComponent<TPAction>();
                go.transform.parent = temp.transform;
                go.name = TPConstant.actionName;

            }


            TPPerception p = sentence.GetComponent<TPPerception>();

            if (p)
            {

                Transform perContainer = TPPerceptionList.GetPerceptionContainer(this);


                GameObject go = new GameObject();
                TPPerception pIn = go.AddComponent<TPPerception>();
                pIn.transform.parent = perContainer;
                pIn.name = sentence.name;
                

            }*/


            /*
            if (this.roleName == "")
                TPAgentNetwork.AddActionPerception(ID, perceptionID, actionID, this.name, p.PerceptionMethod, a.ActionMethod);
            else
                TPAgentNetwork.AddActionPerceptionRole(ID, perceptionID, actionID, this.name, this.p.PerceptionMethod, this.a.ActionMethod, this.roleName);

            TPAgentNetwork.SetMinDelay(ID, this.name, this.tmin);
            TPAgentNetwork.SetMaxDelay(ID, this.name, this.tmax);
            */

            //! \todo this might repeat perceptions in the same agent, if these were already used by a different skill or story. check if it is really a problem
            //! \todo work out how to deal with the case where one sentence has multiple roles



        }




        public string getRule()
        {

            //! \todo add support for multiple plots, starting with support for labels HERE, and in Tramanet.dll
          
            string label = this.name.Remove(' ');

            string s = "  Llogic: \n   " +  label + ": ";
            
            foreach (TPPlotSentence sent in plot)
            {
                if(sent)
                    s = s + sent.getRule();
            }
            return s;
        }
        

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {

        }
    }

#endif

}