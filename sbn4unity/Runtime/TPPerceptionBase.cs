using UnityEngine;
using System.Collections;
using System.Collections.Generic;

//for callbacks:
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;



/*!

 \brief The class taking care of the perception callbacks
 \date November 2016
 \details 

    In the editor, the content creator can create perceptions. However, these are separated, and editable in a script. TPPerceptionBase uses internal variables to connect to TPAgent
    and the EBN defined in a C++ dll, while it makes available protected variables for their use within TPPerception
 
 \sa  TPPerception
*/


namespace timepath4unity
{
    [System.Serializable]
    public class TPPerceptionBase : MonoBehaviour
    {

        [HideInInspector]
        [SerializeField]
        protected int perceptionID = -1;

        //	[SerializeField]
        //	public bool customCallback = false;

        //[SerializeField]
        protected double value;

        //! the callback class called by TPAgentNetwork.
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [System.Serializable]
        internal delegate double TPPerceive(int agentID, int perceptionID);


        private int mindID = -1;

        protected TPAgent Me
        {
            get { return TP.GetAgent(mindID); }
        }

        /*
        protected GameObject MyBody
        {
            get { return Me.MyBody; }
        }
        */

        /*

        public int ID
        {
            get { return perceptionID; }


        }*/



        public static bool removePerception(TPPersonality perso, TPPerceptionBase p)
        {
            foreach (TPSkill s in perso.Skills)
            {
                s.Precons.remove(p.name);
                s.Effects.remove(p.name);

            }
            foreach (TPGoal g in perso.Goals)
            {
                g.Rcon.remove(p.name);
                g.Gcon.remove(p.name);
            }
            GameObject.DestroyImmediate(p.gameObject);
            return true;
        }




        //! The callback adjusted manually on the menu
        [SerializeField]
        private List<TPCallback> callbacksSelected = new List<TPCallback>();



        //! the callback method called by TPAgentNetwork.	
        [SerializeField]
        internal TPPerceive perceptionMethod;

        internal TPPerceive PerceptionMethod
        {
            get
            {
                return perceptionMethod;
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




        //! these must be static in order they work well 
        static double defaultCallback(int agentID, int perceptionID)
        {
            TPPerceptionBase p = TP.GetAgent(agentID).getConnectedPerception( perceptionID);

            foreach (TPCallback c in p.callbacksSelected)
            {
                c.Invoke();
            }


            //p.how.Exec(p);	
            return p.getvalue();
        }
    


        public bool setRandomValue()
        {
            this.value = (double)Random.Range(0.0f, 1.0f);
            return true;

        }

  
        internal bool prepare2Connect(int pos, int ID)
        {
            if (this.perceptionID == -1)
                perceptionID = pos;
            else
                return false;
        

            this.mindID = ID;

            perceptionMethod = defaultCallback;


            TPAgent a = TP.GetAgent(mindID);

            //we regenerate the callbacks:
            for (int i = 0; i < this.callbacksSelected.Count; i++)
            {
                TPCallback cb = callbacksSelected[i];
                callbacksSelected[i] = callbacksSelected[i].Reconnect2Mind(this);
            }

            return true; //! by returning a bool it also makes it not  appear in the TPPersonalityWindow

        }

      



        public double getvalue()
        {
            return (value);

        }

        void Update()
        {
            this.name = this.gameObject.name;

        }




    }


}