
using UnityEngine;
using System.Collections;
using System.Collections.Generic;



/*!

 \brief The basic item containing the rules
 \date 02.2014
 \details The basic item containing one rule, pointing to a perception with ands, as well as nots. 

	To deal with expected effects, it can also contain a certain amount of expected effect (see property Ex)


 \todo think if it is necessary to add "or" support,  also bracket support (also implies work in  the .dll)

 \sa TPAgent, TPSkill
*/

namespace timepath4unity
{

    [System.Serializable]
    public class TPLogicExpression : MonoBehaviour
    {

        private static string AND = " and ";
        public static string NOT = " not ";

        private static string EFFECT = "effect: ";
        private static string USING = "using: ";

        public static string EMPTYRULE = " (empty rule) ";


        private TPProposition[] Props
        {
            get
            {
                return this.transform.GetComponentsInChildren<TPProposition>();
            }
        }


        //public int Count{get{return props.Count;}}
        public int Count { get { return Props.Length; } }


        public TPPerceptionBase[] Perceptions
        {

            get
            {
                List<TPPerceptionBase> perceptionsList = new List<TPPerceptionBase>();
                foreach (TPProposition prop in Props)
                {
                    TPPerceptionBase perc = prop.getP();
                    if (perc != null)
                        perceptionsList.Add(perc);
                }
                return perceptionsList.ToArray();
            }
        }

        public TPResource[] Resources
        {

            get
            {
                List<TPResource> resourceList = new List<TPResource>();
                foreach (TPProposition prop in Props)
                {
                    resourceList.Add(prop.getOrAddResource());
                }
                return resourceList.ToArray();
            }
        }

        public bool UsesResources
        {
            get {
                bool b = true;
                foreach (TPProposition p in Props) {
                    b = b & p.HasRes;
                }
                return b;
            }
        }


        /*
        public TPProposition getProposition(TPPerceptionBase p){
            TPProposition prop = props.Find(pr => pr.P().Equals(p) ); 
            return prop;
            }
        */
        public TPProposition getProposition(TPPerceptionBase p)
        {
            foreach (TPProposition prop in Props)
            {
                if (prop.getP().name.Equals(p.name))//! \todo check if it works directly from comparing TPPerceptionBase, and not the names. 
                    return prop;
            }
            return null;

        }

        public TPProposition getProposition(TPResource r)
        {
            foreach (TPProposition prop in Props)
            {
                if (prop.getOrAddResource().name.Equals(r.name))//! \todo check if it works directly from comparing TPPerceptionBase, and not the names. 
                    return prop;
            }
            return null;

        }


        static public TPLogicExpression getLogicExpression(string s, Transform expressionContainer)
        {
            TPLogicExpression precons = null;
            Transform preconsT = expressionContainer.Find(s);
            if (preconsT != null)
                precons = preconsT.GetComponent<TPLogicExpression>();

            if (precons == null)
            {
                GameObject go = new GameObject();
                precons = go.AddComponent<TPLogicExpression>();
                go.transform.parent = expressionContainer;
                go.name = s;
            }
            return precons;
        }

        //! adds a condition related to a perception, either negated or not (false will mean it is negated)
        protected TPProposition add2Personality(string pname, bool isTrue)
        {
            GameObject go = new GameObject();
            TPProposition prop = go.AddComponent<TPProposition>();
            prop.setP(pname, this);
            if (!isTrue)
                prop.setFalse();
            prop.setName();

            return prop;
        }

        public TPProposition add2Personality(string pname)
        {
            return this.add2Personality(pname, true);
        }
        
        public TPProposition addEffect(string pname, float expect)
        {
            TPProposition prop = this.add2Personality(pname, true);
            prop.Ex = expect;
            prop.setName();
            return prop;
        }

        public TPProposition addResource(string rname, float amount)
        {
            GameObject go = new GameObject();
            TPProposition prop = go.AddComponent<TPProposition>();
            prop.setResource(rname, this, amount);
            prop.setName();
            return prop;

        }


        /*
        public void negate(TPPerceptionBase p){
            TPProposition prop = props.Find(pr => pr.P.Equals(p) );
            if(prop != null)
                prop.IsTrue = ! prop.IsTrue;

            }*/
        public void negate(string pname)
        {
            this.transform.Find(pname).GetComponent<TPProposition>().negate();
        }


        //! we remove a  proposition referring to a perception with name. Notice this does not remove the corresponding perception.
        //! it also works for resources
        public bool remove(string pname)
        {

            /*
            TPProposition p=this.transform.FindChild ().GetComponent<TPProposition>();//FALSE
            if(p != null){
                GameObject.DestroyImmediate(p.gameObject);
                return true;
            }
            return false;
            */
            TPProposition[] props = this.transform.GetComponentsInChildren<TPProposition>();
            foreach (TPProposition p in props)
            {
                if (p.getP().name.Equals(pname))
                {
                    GameObject.DestroyImmediate(p.gameObject);
                    return true;
                }
            }

            foreach (TPProposition p in props)
            {
                TPResource res = p.getOrAddResource();
                if (res != null && res.name.Equals(pname))
                {
                    GameObject.DestroyImmediate(p.gameObject);
                    return true;
                }
            }



            return false;


        }

        public bool remove(TPProposition prop)
        {

            /*
            TPProposition p=this.transform.FindChild ().GetComponent<TPProposition>();//FALSE
            if(p != null){
                GameObject.DestroyImmediate(p.gameObject);
                return true;
            }
            return false;
            */
            if (prop.transform.parent == this.transform)
            {
                GameObject.DestroyImmediate(prop.gameObject);
                return true;
            }
            else
            {

                Debug.LogError("trouble destroying proposition " + this.name);
                return false;
            }


        }


        //surprisingly, this function is never used (formulas evaluated in external dll)

        /*
        public double getTruthValue(){

		
            double t = 1.0;
            double value = 0.0;
            foreach (TPProposition p in props) {
                value =  p.P.getvalue();

                if(! p.IsTrue)
                    value = 1.0 -value;
                if (value < t) //this is good only if within the TramaNet.dll there is fuzzy_min defined
				
                    t = value;
            }
            if (t < 0.0)
                t = 0.0;
            else if (t > 1.0)
                t = 1.0;

            return t;
            }
        */

        /*public void cleanRules(){
                    this.props.RemoveAll (prop => prop.P == null);
            }
        */

        /*
        public void cleanRules(){
            TPProposition[] props = this.transform.GetComponents<TPProposition> ();
            foreach (TPProposition prop in props)
                GameObject.DestroyImmediate(prop.gameObject);
        }*/


            //! getPerceptionsContainer for personalities
        public Transform getPerceptionsContainerFromHierarchy()
        {
            //! \todo check if this redundant with  TPPerceptionList.GetPerceptionContainer(perso);


            //we do not store this value, we always find it through the hierarchy. This is to prevent it from pointing to out from the hierarchy (for example, when copied)	
            Transform persoTransform = this.transform.parent.parent.parent;

            Transform perceptionContainer = persoTransform.GetComponentInChildren<TPPerceptionList>().transform;
            return perceptionContainer;
        }

        



        public Transform getResourcesContainerFromHierarchy()
        {
            //we do not store this value, we always find it through the hierarchy. This is to prevent it from pointing to out from the hierarchy (for example, when copied)	
            Transform persoTransform = this.transform.parent.parent.parent;

            Transform perceptionContainer = persoTransform.GetComponentInChildren<TPResourceList>().transform;
            return perceptionContainer;
        }



        public string getRule()
        {
            string rules = "";
            bool firstBit = true;
            foreach (TPProposition r in this.Props)
            {
                if (r != null)
                {
                    TPPerceptionBase p = r.getP();
                    if (p != null)
                    {

                        if (firstBit)
                        {
                            firstBit = false;
                        }
                        else
                            rules = rules + AND;

                        if (!r.IsTrue)
                            rules = rules + NOT;

                        rules = rules + p.name + " ";
                    }
                }
          }
            
            
            return rules;

        }
        



        public string getEffectsRule()
        {
            string rules ="\n"+ EFFECT + "\n   ";
            bool firstBit = true;
            foreach (TPProposition r in this.Props)
            {

                if (r != null)
                {
                    TPPerceptionBase p = r.getP();
                    if (p != null)
                    {

                        if (firstBit)
                        {
                            firstBit = false;
                        }
                        else
                            rules = rules + AND;

                        if (!r.IsTrue)
                            rules = rules + NOT;

                        rules = rules + p.name + " " + r.Ex.ToString("F2");
                    }
                }
            }
            return rules;

        }






        public string getResourcesRule()
        {
            string rules = "\n" + USING + "\n   ";
            bool firstBit = true;
            foreach (TPProposition rule in this.Props)
            {
                TPResource r = rule.getOrAddResource();
                if (r != null)
                {

                    if (firstBit)
                    {
                        firstBit = false;
                    }
                    else
                    {
                        rules = rules + AND;

                    }

                    rules = rules + r.name + " " + rule.ResNeeded.ToString("F2");
                }
            }
            return rules;


        }



        /*
        public void Clear(){
            props.Clear ();
        }*/
    }


}