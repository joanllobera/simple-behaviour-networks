using UnityEngine;
using System.Collections;
//using System.Collections.Generic;


//for callbacks
/*using System;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
*/


/*!

 \brief A TimePath Skill
 \date 02.2014
 \details  A simple container of the elements defining a TPSkill: an action, some preconditions, effects and resources
 
 \sa TPAgent, TPGoal
*/
namespace timepath4unity
{
    [System.Serializable]
    public class TPSkill : MonoBehaviour
    {
        private static string PRECON = " if: ";//cannot be the same than RCON in goal, otherwise there is ambiguity for the parser
        private static string THEN = "do: ";
        private static string ENDSKILL = "endskill ";

        //private static string EFFECT = "effect: "; //defined in TPLogicExpression
        //private static string USING = "using: ";


        public static bool removeSkill(TPSkill s)
        {
            if (s == null)
            {
                return false;
            }
            else
            {
                GameObject.DestroyImmediate(s.gameObject);
                return true;
            }

        }

       
        public TPLogicExpression Resources
        {
            get
            {
                TPLogicExpression resources = TPLogicExpression.getLogicExpression(TPConstant.resourcesInSkill, this.transform);

                return resources;
            }
        }

        public TPActionBase Action
        {
            get
            {
                TPActionBase action = null;
                Transform actionT = this.transform.Find(TPConstant.actionName);
                if (actionT != null)
                    action = actionT.GetComponent<TPActionBase>();

                //! \todo find a way to add this object in case the use messes with it (example: add a gameobject with a dummy component which is changed later on, in the inspector??)
                    if (action == null || actionT == null)
                {
                    Debug.LogError("the Skill " + this.name + " has no Action associated to it. All Skills must have an Action associated to it.");
                    //  action = TPCallbackTools.AddAction2Skill(this);
                }
                return action;

            }
        }

        public TPLogicExpression Precons
        {
            get
            {

                TPLogicExpression precons = TPLogicExpression.getLogicExpression(TPConstant.preConditions, this.transform);

                return precons;
            }
        }

        public TPLogicExpression Effects
        {
            get
            {
                TPLogicExpression effects = TPLogicExpression.getLogicExpression(TPConstant.Effects, this.transform);

                return effects;
            }
        }

        public string getRule()
        {//! \todo replace these hardcoded strings by things defined in some static string
            string rules = PRECON + "\n   " +  Precons.getRule() + "\n" + THEN + "\n   ";

            if (this.Action == null || this.Action.name == "")
            {
                Debug.Log("Skill " + this.name + "has no action defined. I am skipping this Skill");
                return "";
            }

            rules = rules + this.name +  Effects.getEffectsRule();

            rules = rules + Resources.getResourcesRule();

            rules = rules + "\n" + ENDSKILL;

            return rules;

        }

       

        public void OnDestroy()
        {
         
        }


    }

}
	