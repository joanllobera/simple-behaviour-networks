using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/*!

 \brief A TimePath Goal
 \date 02.2014
 \details  A simple container of the elements defining a TPGoal
 
 \sa TPAgent, TPSkill
*/
namespace timepath4unity
{

    [System.Serializable]
    public class TPGoal : MonoBehaviour
    {

        private static string GCON = "iwant: ";
        private static string RCON = "when: ";
        private static string ENDGOAL = " endgoal ";
        private static string NOGOAL = "(no goal condition)";
        //new string name;

        [SerializeField]
        protected int position = -1;

        //[SerializeField]
        //protected TPLogicExpression rcon = null; // = ScriptableObject.CreateInstance<TPLogicExpression>();

        //[SerializeField]
        //protected TPLogicExpression gcon = null; //ScriptableObject.CreateInstance<TPLogicExpression>();





        //to grant the importance and the relevance values keep being a fuzzy value:
        [SerializeField]
        private float i = 1.0f;


        public TPLogicExpression Rcon
        {
            get
            {
                TPLogicExpression rcon = TPLogicExpression.getLogicExpression(TPConstant.relevanceConditions, this.transform);




                return rcon;
            }
        }


        public TPLogicExpression Gcon
        {
            get
            {
                TPLogicExpression gcon = TPLogicExpression.getLogicExpression(TPConstant.goalConditions, this.transform);

                return gcon;
            }
        }


        public float I
        {
            get
            {
                return i;
            }

            set
            {
                i = Mathf.Clamp(value, 0.0f, 1.0f);
            }
        }


        //this will not work at runtime, perceptionID do not know why
        /*
        public float updateRelevance(){
                            float r = (float)rcon.getTruthValue ();
                            return r;
				
        }*/




        public bool setGoalID(int pos)
        {
            if (this.position == -1)
            {
                position = pos;
                return true;
            }
            else
                return false;
        }





        //!removes propositions referring to perceptions that do not exist
        /*public void cleanRules(){
            rcon.cleanRules ();
            gcon.cleanRules ();
        }*/

        public string getRule()
        {

            //string rules = "\n goal " + Rcon.getRule() + " \n then " + Gcon.getRule() + this.I + " endgoal \n \n";

            string rules = RCON +"\n  "+ Rcon.getRule() + GCON + "\n  " + Gcon.getRule() + "\n  " + this.I + ENDGOAL;

            return rules;
        }

        public string getGoalName()
        {
            string rules = "";
            if (this.Rcon.Count > 0)
            {
                rules = rules + RCON + Rcon.getRule();
            }

            if (this.Gcon.Count > 0)
            {
                rules = rules + GCON;
                rules = rules + Gcon.getRule();
            }
            else
            {
                return NOGOAL;
            }


            return rules;
        }


    }


}