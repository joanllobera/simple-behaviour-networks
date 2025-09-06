


using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using System;

using System.Linq;//to have distinct


/*!
\brief
TPPersonality contains perceptions, goals, skills and resources

 * The crucial function is Connect2Net. This will create, before runtime, a copy of the personality for each agent involved, and will connect the callbacks apropriately. 

\details 

*/

namespace timepath4unity
{

    [System.Serializable]
    public class TPPersonality : MonoBehaviour
    {
     


        #region Member variables
        //A TPPersonality is defined by:	

        //TPActions can controll parameters of an animController:
        //[SerializeField]
        //RuntimeAnimatorController animController = new RuntimeAnimatorController();



        #endregion

        #region Member properties

        //public Animator Anim{ get { return anim; }set{anim = Anim;}}
        //public RuntimeAnimatorController AnimControl { get { return animController; } set { animController = value; } }


        public TPPerceptionBase[] Perceptions
        {
            get
            {
                Transform container = TPPerceptionList.GetPerceptionContainer(this);

                TPPerceptionBase[] perceptions = container.GetComponentsInChildren<TPPerceptionBase>();
                return perceptions;
            }
        }



        public TPSkill[] Skills
        {
            get
            {
                Transform container = TPSkillList.GetSkillContainer(this);
                TPSkill[] skills = container.GetComponentsInChildren<TPSkill>();
                return skills;
            }
        }

        public TPGoal[] Goals
        {
            get
            {
                Transform container = TPGoalList.GetGoalContainer(this);
                TPGoal[] goals = container.GetComponentsInChildren<TPGoal>();
                return goals;
            }
        }


        public TPResource[] Resources
        {
            get
            {
                Transform container = TPResourceList.GetResourceContainer(this);
                TPResource[] resources = container.GetComponentsInChildren<TPResource>();
                return resources;
            }
        }



        #endregion



        #region functions used in TPPersonalityWindow:

        static public void removeSpacesInNames(TPPersonality pers)
        {
            foreach (TPPerceptionBase p in pers.Perceptions)
                p.name = p.name.Replace(" ", "_");
            foreach (TPResource r in pers.Resources)
                r.name = r.name.Replace(" ", "_");
            foreach (TPSkill s in pers.Skills)
            {
                s.name = s.name.Replace(" ", "_");
                //s.Action.name = s.Action.name.Replace (" ", "_");
            }
        }




        //! a method to add a resource to a personality
        public TPResource AddResourceByName(string n)
        {
            Transform resContainer = TPResourceList.GetResourceContainer(this);
            GameObject go = new GameObject();
            TPResource rIn = go.AddComponent<TPResource>();
            rIn.transform.parent = resContainer;
            rIn.name = n;
            return rIn; 
        }





        public bool removeResource(TPResource r)
        {

            foreach (TPSkill s in Skills)
                s.Resources.remove(r.name);
            GameObject.Destroy(r.gameObject);
            return true;
        }

        public TPResource GetResourceByName(string n)
        {
            Transform resContainer = TPResourceList.GetResourceContainer(this);
            Transform t = resContainer.Find(n);
            if (t != null)
                return t.GetComponent<TPResource>();
            else
                return null;

        }





        public void AddResourceUnits(string name, float amount)
        {
            TPResource res = this.GetResourceByName(name);
            res.AmountAvailable += amount;

        }
        public GameObject GetResourceUnits(string name, float amount)
        {
            TPResource res = this.GetResourceByName(name);
            res.AmountAvailable -= amount;

            if (res != null)
                return res.GameObjectInstance;
            else
                return null;

        }



        //! a method to add a goal to a personality 
        public TPGoal AddGoal()
        {


            Transform goalContainer = TPGoalList.GetGoalContainer(this);
            GameObject go = new GameObject();
            TPGoal gIn = go.AddComponent<TPGoal>();
            gIn.transform.parent = goalContainer;
            //gIn.setGoalID(goals.Count);
            //this.goals.Add(gIn);
            return gIn;
        }

        public bool removeGoal(TPGoal g)
        {
            if (g == null)
                return false;
            else
            {
                DestroyImmediate(g.gameObject);
                return true;
            }

        }




        /*
        //! a method to add a perception to a personality
        static void AddPerceptionByName(TPPersonality perso, string n)
        {

            Transform perContainer = TPPerceptionList.GetPerceptionContainer(perso);


            GameObject go = new GameObject();
            TPPerception pIn = go.AddComponent<TPPerception>();
            pIn.transform.parent = perContainer;
            pIn.name = n;
            pIn.setRandomValue();


        }*/










      




        

       
        /*
    

        public string Connect2Net(int netID)
        {




            //3.a we add all the perceptions, actions and resources defined (including the ones for the plots, if necessary)

            myConnectedPerceptions = this.Perceptions;
            myConnectedPerceptions = myConnectedPerceptions.Distinct().ToArray();//to prevent errors, we make sure there are no repeated elements
            int i = 0;
            foreach (TPPerceptionBase p in myConnectedPerceptions)
            {
                if (p)
                {
                    p.setPerceptionID(i);
                    p.Connect2Net(netID);
                    i++;
                }
            }



            myConnectedSkills = this.Skills;
            myConnectedSkills = myConnectedSkills.Distinct().ToArray();//to prevent errors, we make sure there are no repeated elements
            for (int perceptionID = 0; perceptionID < myConnectedSkills.Length; perceptionID++)
            {
                TPSkill s = myConnectedSkills[perceptionID];
                if (s != null)
                {
                    s.Action.setActionID(perceptionID);
                    s.Connect2Net(netID);
                }
            }

            myConnectedResources = this.Resources;
            myConnectedResources = myConnectedResources.Distinct().ToArray();//to prevent errors, we make sure there are no repeated elements

            for (int k = 0; k < myConnectedResources.Length; k++)
            {
                TPResource r = myConnectedResources[k];
                if (r)
                    r.setResourceID(k);
                r.Connect2Net(netID);
            }

           

       

            //4. we parse the rules in goals and skills
            string rules = "";
            foreach (TPGoal g in this.Goals)
            {
                if (g)
                    rules = rules + g.getRule();
            }

            foreach (TPSkill s in this.Skills)
            {
                if (s)
                    rules = rules + s.getRule();
            }
            return rules; 



        }
        */

        #endregion



        public void Reset()
        {


        }


        public void Awake()
        {


        }


        void Start()
        {

        }
        void Update()
        {
        }

        void OnDestroy()
        {
        }

    }

}