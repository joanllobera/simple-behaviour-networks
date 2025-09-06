using System;
using System.Collections;
using UnityEngine;


namespace timepath4unity
{

    [System.Serializable]
    public class TPProposition : MonoBehaviour // ScriptableObject
    {

        [SerializeField]
        protected TPPerceptionBase p = null;

        [SerializeField]
        protected TPResource res = null;


        [SerializeField]
        protected bool isTrue = true; //!write false if it is negated

        //	[SerializeField]
        //	protected bool isAnd = false; //!write false if it is not preceded by an AND

        //when we deal with effects, we use:
        [Range(0.0f, 1.0f)]
        [SerializeField]
        double expectedProba = -1;

      

        //when we deal with resources, we use:
        [SerializeField]
        protected double rNeeded = 0.0;

        public double ResNeeded
        {
            get { return rNeeded; }
            set
            {
                if (value < 0.0)
                    rNeeded = 0.0;
                else
                    rNeeded = value;
            }
        }
        

        public bool HasRes
        {
            get
            {
                if (getResourceTransform() == null) { return false; }
                return true;
            }
        }

        public bool HasEffect
        {
            get
            {
                return (this.expectedProba != -1);
            }

        }


        public string ResName
        {
            get
            {
                string s = "";

                if (res == null) { return s; }
                else { return res.name; }
            }

        }


        /* //not used
        public string PropName
        {
            get
            {
                string s = "";
                try
                {
                    s = this.p.name;

                }
                catch { };
                return s;

            }
        }*/

        /*
        static Transform getPerceptionsContainerFromHierarchy(TPLogicExpression formula) {
                //we do not store this value, we always find it through the hierarchy. This is to prevent it from pointing to out from the hierarchy (for example, when copied)	
                return formula.transform.parent.parent.parent.GetComponentInChildren<TPPerceptionList>().transform;
			
            }


        static Transform getResourcesContainerFromHierarchy(TPLogicExpression formula) {
                //we do not store this value, we always find it through the hierarchy. This is to prevent it from pointing to out from the hierarchy (for example, when copied)
                return formula.transform.parent.parent.parent.GetComponentInChildren<TPResourceList>().transform;

        }
    */

        //! quite subtle: we confirm that it points to THE RIGHT perception instance in the personality.
        //!  This means, we check it points to the right perception inside the personality, or make it point to it. Otherwise, we create it.
        public TPPerceptionBase getP()
        {
            if (p == null)
                return null;

            TPLogicExpression f = this.transform.parent.GetComponent<TPLogicExpression>();
            if (f == null)
                return null;

            Transform container = f.getPerceptionsContainerFromHierarchy();
            Transform perceptionTransform = null;
            try
            {
                perceptionTransform = container.Find(this.p.name);
            }
            catch { }
            if (perceptionTransform == null)
            {
                GameObject go = new GameObject();
                go.transform.parent = perceptionTransform;
                go.name = this.p.name;
                this.p = go.AddComponent<TPPerceptionBase>();
            }
            else
            {

                try
                {
                    TPPerceptionBase perc = (TPPerceptionBase)perceptionTransform.gameObject.GetComponent<TPPerceptionBase>();
                    this.p = perc;
                }
                catch
                {
                    Debug.LogError("there is an object with name: " + this.p.name + " but it is NOT a perception!!");
                    return null;
                }

            }
            setName();
            return this.p;




        }

        //! to set up the names directly from the inspector (inside a TPPlotSentence)
        public string PerceptionName{
            get { return this.p.name; }
            set { this.p.name = value;
                setName();
            }
            
            }

#if TRAMA
        //! we set the perceptions for tpplotsentences, see TPCallbacktools
        public bool setExternalPerception(TPPerceptionBase p, TPPlotSentence ps)
        {
            this.p = p;
            Transform container = TPPlotSentence.setPerceptionsContainerInHierarchy(ps);
            p.transform.parent = container;
            transform.parent = ps.PreCon.transform;

            return true;
        }


#endif


        public bool setP(string perceptionName, TPLogicExpression formula)
        {
            Transform container = formula.getPerceptionsContainerFromHierarchy();
            Transform t = container.Find(perceptionName);
            if (t != null)
            {
                this.p = t.GetComponent<TPPerceptionBase>();
                if (this.p != null)
                {
                    this.transform.parent = formula.transform;
                    return true;
                }
            }
            setName();
            return false;

        }


        public void setName()
        {
            if (p != null)
            {  //! \todo think if here we should recheck with P() to make sure everything is fine, or to make evident if it is not
                this.name = p.name;
                if (!this.IsTrue)
                    this.name = TPLogicExpression.NOT + this.name;
                if (HasEffect)//it is an effect
                    this.name = this.name + " " + this.expectedProba.ToString("F2");
            }
            else if (this.res != null)
            {
                this.name = this.res.name + " " + this.rNeeded.ToString("F2");
            }
            else
                this.name = TPLogicExpression.EMPTYRULE;
            
        }

        public bool setResource(string rname, TPLogicExpression formula, float amountNeeded)
        {
            Transform container = formula.getResourcesContainerFromHierarchy();
            Transform t = container.Find(rname);
            if (t != null)
            {
                this.res = t.GetComponent<TPResource>();
                if (this.res != null)
                {
                    this.transform.parent = formula.transform;
                    this.ResNeeded = amountNeeded;
                    return true;
                }
            }
            setName();
            return false;



        }


        protected Transform getResourceTransform()
        {
            TPLogicExpression formula = this.transform.parent.GetComponent<TPLogicExpression>();
            if (formula == null)
                return null;
            Transform container = formula.getResourcesContainerFromHierarchy();


            Transform t = null;
            try
            {//to avoid the eventual exception for being null

                t = container.Find(this.res.name);
            }
            catch
            {
            }

            return t;


        }

        public TPResource getOrAddResource()
        {
            TPLogicExpression formula = this.transform.parent.GetComponent<TPLogicExpression>();
            if (formula == null)
                return null;
            Transform container = formula.getResourcesContainerFromHierarchy();


            Transform t = getResourceTransform();

            if (t == null)
            {
                GameObject go = new GameObject();
                go.transform.parent = container;
                this.res = go.AddComponent<TPResource>();
            }
            else
            {

                try
                {
                    TPResource r = (TPResource)t.gameObject.GetComponent<TPResource>();
                    this.res = r;

                }
                catch
                {
                    Debug.LogError("there is an object with name: " + this.p.name + " but it is NOT a perception!!");
                    return null;
                }


            }

            return this.res;


        }


        
        public double Ex
        {
            set
            {
                if (value > 1.0)
                    expectedProba = 1.0;
                else if (value < 0.0)
                    expectedProba = 0.0;
                else
                    expectedProba = value;
                setName();
            }
            get { return expectedProba; }
        }



        public bool IsTrue
        {
            get { return isTrue; }
            set
            {
                isTrue = IsTrue;
                setName();
            }

        }


        public void setPostvalue(double val)
        {
            expectedProba = val;
            setName();
        }


        /*
        public void setAnd(bool b){
            this.isAnd = b;
        }
        public void setAnd(){
            setAnd(true);	
        }
	
        */


        public void setFalse()
        {
            isTrue = false;
            setName();
        }
        public void negate()
        {
            isTrue = !isTrue;
            setName();

        }


        //! returns the string corresponding to the rule (assumes thekeywords defined in networkheader.h  are identical) \todo include bracket processing


        /*
        public string getRule(){
            string s="";
            if(isAnd)
                s = String.Concat(s," and ");
            if(! isTrue)
                s = String.Concat(s," not ");

            s = String.Concat(s, this.p.name);
            s = String.Concat(s," ");
            return s;
        }*/

    }


}