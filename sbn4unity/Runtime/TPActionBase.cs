using UnityEngine;
using System.Collections;
using System.Collections.Generic;

//for callbacks:
using System;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;






/*!

 \brief The class taking care of the action callbacks
 \date November 2016
 \details 

  \sa TPAction, TPPerceptionBase

*/
namespace timepath4unity
{
    [System.Serializable]
    public class TPActionBase : MonoBehaviour
    {

        //! \todo check if these cannot be made private (so they are not visible in TPAction API)
        //protected int netID = -1;
        protected int mindID = -1;

        protected TPAgent Me {
            get { return TP.GetAgent(mindID); }
        }

        private bool wasExecuted = false;


        //! WasExecuted is only used to paint whether it was executed
        public bool WasExecuted{
            get { return wasExecuted; }
            set { wasExecuted = value; }
        }



       
        protected GameObject MyBody
        {
            get { return Me.MyBody; }
        }
        

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [SerializeField]
        internal delegate void TPAct(int agentID, int actionID, double decisiveness, double activation);

        [SerializeField]
        private int position = -1;

        //[SerializeField]
        //public bool useHardcodedCallback = false;

        [SerializeField]
        private TPAct actionMethod;

        internal TPAct ActionMethod
        {
            get
            {
                return actionMethod;
            }
        }



        //[SerializeField]
        //public List<TPMecanimParam> mecanimParamsSelected = new List<TPMecanimParam>();

        //[SerializeField]
        //public AnimationClip animClip = null;

        //	public Delegate how;

        [SerializeField]
        private List<TPCallback> callbacksSelected = new List<TPCallback>();





        //the activation level can be read, but only changed internally:
        //[Range(0.0f, 1000.0f)]
        //[SerializeField]
        private double decisiveness = 0.0;

        private double activation = 0.0;


        public double Decisiveness
        {
            get
            {
                return decisiveness;
            }
        }

        public double Activation
        {
            get
            {
                return activation;
            }
        }


        public TPCallback[] CallbacksSelected
        {
            get
            {
                return callbacksSelected.ToArray();
            }
        }

        public void RemoveCallback(TPCallback a)
        {
            callbacksSelected.Remove(a);
            callbacksSelected.Remove(null);
        }

        public void AddCallback(TPCallback a)
        {
            callbacksSelected.Add(a);
        }


        /*
        public void setRandomActivation()
        {

            decisiveness = (double)Mathf.Abs(UnityEngine.Random.Range(0.0f, 1000.0f));
        }
        */



        //! The callback action. This must be static in order to work well 
        static void defaultCallback(int agentID, int actionID, double decisiveness, double activation)
        {
            TPAgent agent = TP.GetAgent(agentID);

            TPActionBase a = agent.getConnectedAction(actionID);







            //Debug.Log ("I am executing action " + a.name + "in agent: " + agentID + "agent is: " + agent.name);
            a.decisiveness = decisiveness;
            a.activation = activation;

            //Option1: we deal with the mecanim Parameters:
            //! \todo find out how this can go back to default state.

            /*
            foreach (TPMecanimParam mP in a.mecanimParamsSelected)
            {
                if (mP.type.Equals(TPMecanimParam.boolParam))
                {
                    agent.GetComponent<Animator>().SetBool(mP.name, mP.b);
                }
                else if (mP.type.Equals(TPMecanimParam.intParam))
                {
                    agent.GetComponent<Animator>().SetInteger(mP.name, mP.perceptionID);
                }
                else if (mP.type.Equals(TPMecanimParam.floatParam))
                {
                    agent.GetComponent<Animator>().SetFloat(mP.name, mP.f);
                }
            }
            */
           // Debug.Log("i am executing action: " + a.wasExecuted + "DEcis: " + a.Decisiveness + "Activati: " + a.Activation);
            a.wasExecuted = true;
           // Debug.Log("i am executing action: " + a.wasExecuted );




            //Option 3: we execute all the delegates.
            //a.how.Exec(a);	


            //Option 3b: we execute all the delegates in TPCallback,
            foreach (TPCallback c in a.callbacksSelected)
            {
                c.Invoke();
            }

         

            //	Debug.Log("My name is:" + agent.name + "and I have executed action: " +  a.name);

        }


        internal bool prepare2Connect(int pos, int mindID)
        {
            if (this.position == -1)
               position = pos;
            else
                return false;
       
            this.mindID = mindID;

            //we regenerate the callbacks:
            for (int i = 0; i < this.callbacksSelected.Count; i++)
            {
                //TPCallback cb = callbacksSelected[i];
                callbacksSelected[i] = callbacksSelected[i].Reconnect2Mind(this);
            }

            this.actionMethod = defaultCallback;


            return true;
        }



     

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {
           // this.name = this.gameObject.name;

        }

    }


}