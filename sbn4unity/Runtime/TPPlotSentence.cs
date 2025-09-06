using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/*!

 \brief A Sentence within a TimePath story plot
 \date 07.2014

\details

\todo it needs to cover 3 cases:
	1.a condition to realize
	2.an action to perform
	3.perceive if the action or condition has been realized (by the one assuming the role)

 
*/
namespace timepath4unity
{
#if TRAMA

    [System.Serializable]
    public class TPPlotSentence : MonoBehaviour
    {

        [HideInInspector]
        [SerializeField]
        TPLogicExpression preCon = null; //for external sentences, like IF x AND not y THEN Z

        public bool IsExternal{ get { return preCon == null ? false : true; } }


        


        //! external conditions for stories
        public TPLogicExpression setExternalCondition()
        {
            //we create the logic expression:
            GameObject go = new GameObject();
            TPLogicExpression le = go.AddComponent<TPLogicExpression>();
            le.name = TPConstant.externalActionFormula + name;
            le.transform.parent = transform.parent;

            //we assign it:
            preCon = le;

            //we set up the perceptions container:
            setPerceptionsContainerInHierarchy(this);
            

            return le;
        }


        //! an odd function needed to preserve reflection related to TPPerception in order 
        public Transform getPerceptionTransformFromProposition(TPProposition prop)
        {
            try
            {
                Transform t = setPerceptionsContainerInHierarchy(this).transform.FindChild(prop.PerceptionName);
                return t;
            }
            catch { }


            return null;

        }


        //! getPerceptionsContainer for story sentences
        public static Transform setPerceptionsContainerInHierarchy(TPPlotSentence s)
        {
            TPPerceptionList pl = null;
            try
            {
                pl = s.transform.parent.GetComponentInChildren<TPPerceptionList>();
            }
            catch { }

            if (pl == null)
            {
                GameObject go = new GameObject();
                pl = go.AddComponent<TPPerceptionList>();
                pl.transform.parent = s.transform.parent;
                pl.name = TPConstant.perceptionsContainer;
            }
            return pl.transform;
        }
        


        public string getExternalRule()
        {
            string s = "";
            if (preCon != null)
                s = preCon.getRule();
            return s;

        }







        public TPProposition[] getExternalPropositions()
        {
            TPProposition[] props = null;

            TPLogicExpression le = null;
            try
            {
                le = transform.parent.GetComponentInChildren<TPLogicExpression>();
                props = le.GetComponentsInChildren<TPProposition>();
            }
            catch { }
            return props;
        }




        public void destroyExternalCondition()
        {
            Transform t = transform.parent.FindChild(TPConstant.externalActionFormula + name);
            if (t != null)
            {
                DestroyImmediate(t.gameObject);
            }

            TPPerceptionList pl = transform.parent.GetComponentInChildren<TPPerceptionList>();
            if(pl != null)
            {
                DestroyImmediate(pl.gameObject);

            }
        }




        public bool userCanPerform = false;


        [SerializeField]
//        [Range(0.0f,Mathf.Infinity)]
        double tmin = 3.0;
 //       [Range(0.0f, Mathf.Infinity)]
        [SerializeField]
        double tmax = 5.0;

//        [SerializeField]
//        float duration = 9.0f;

        public TPLogicExpression PreCon
        {

            get
            {
                return preCon;
            }

        }


        public float Tmin
        {
            get
            {
                return (float)tmin;
            }
            set {

                tmin = (double) value;
                if (tmin < 0)
                    tmin = 0.0;
            }
        }

        public float Tmax
        {
            get
            {
                return (float)tmax;
            }
            		set {
                        tmax = (double) value;
                        if (tmax < 0)
                            tmax = 0.0;
            
                    }
        }

        /*
        public float Duration
        {
            get
            {
                return duration;
            }
            	set {
                    duration = value;
                    if (tmax < 0)
                        tmax = 0.0;
            
                }
        }
        */


        //! will return "" if no role, otherwise, hte first in the sentence.
        public string getFirstRole()
        {
            string[] rs = this.getRoles();
            return rs[0];
        }


        /*
        //values between 0 and 1
        [SerializeField]
        [Range(0.0f,1.0f)]
        float requireRole = 0.0f; //to consider you are the one performing the action, do you need to be already that role
        
        [SerializeField]
        bool becomeRole = false;//the fact of performing the action, does it make you become that role

        //if it involves several other roles:
        
        List<TPPlotSentence> singleRoles = new List<TPPlotSentence> ();

        public bool IsMultiRole {
            get {
                return (singleRoles.Count ==0);
            }
        }

        public bool IsPartOfMultiRole{
            get{
                if(this.transform.parent && this.transform.parent.GetComponent<TPPlotSentence>() != null)
                    return true;
                else
                    return false;
                }


        }
        */

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {

        }


        //! Returns the roles in sentence. Concretely, all the words written in capital letters. It accepts other symbols for roles(numbers,etc.) as far as it starts with a capital letter </returns>
         static string[] getRoleNamesInSentence(string s)
        {

            string[] sentence2 = s.Split(" ".ToCharArray());

            List<string> roleNames = new List<string>();

            foreach (string input in sentence2)
            {

                if (input.ToUpper() == input && input.Length > 0 && char.IsLetter(input[0]))
                {// string is all upper


                    /*Regex.IsMatch(input, @"^[a-zA-Z]+$");
                    Only letters and numbers:
					
                        Regex.IsMatch(input, @"^[a-zA-Z0-9]+$");
                    Only letters, numbers and underscore:
					
                        Regex.IsMatch(input, @"^[a-zA-Z0-9_]+$");
                    */


                    roleNames.Add(input);
                }
            }
            return roleNames.ToArray();
        }

        
        static string returnOneRoleSentence(string s, string role)
        {
            int i = s.IndexOf(role);
            if (i < 0)
                return s;

            string s2 = s.ToLower();
            s2.Remove(i, role.Length);
            s2.Insert(i, role);
            return s2;
        }

        public string[] getRoles()
        {
            string[] roles = getRoleNamesInSentence(this.name);

                       
            return roles;


        }
        
        public string getNameWithoutSpaces()
        {
            string temp = this.name.Replace('.', ' ');
            temp = temp.Replace(' ', '_');
            return temp;
        }


        public string getRule()
        {
            string s = getNameWithoutSpaces();
            s += ".  ";

            return s;
        }





    }

#endif
}