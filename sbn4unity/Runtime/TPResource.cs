using UnityEngine;

/*!

 \brief A TimePath Resource
 \date 02.2014
 \details  The equivalent of TPPerceptionBase used for resources
 
 \sa TPAgent, TPSkill
*/

namespace timepath4unity
{

    [System.Serializable]
    public class TPResource : MonoBehaviour
    {

        [HideInInspector]
        [SerializeField]
        protected int position = -1;

        [HideInInspector]
        [SerializeField]
        protected int mindID;

        [HideInInspector]
        private bool isConnected = false;

        [SerializeField]
        protected double amountAvailable = 1.0;



        public double AmountAvailable
        {
            get { return amountAvailable; }
            set
            {
                if (value < 0.0)
                    amountAvailable = 0.0;
                else
                    amountAvailable = value;
            }
        }



        [SerializeField]
        protected GameObject resourceInstance = null;


        //! the game object corresponding to the resource, when it is in the world
        public GameObject GameObjectInstance
        {
            get
            {
                if (resourceInstance == null)
                    return null;
                else
                    return (GameObject)GameObject.Instantiate(resourceInstance);
            }
            set
            {
                resourceInstance = (GameObject)GameObject.Instantiate(value);
                resourceInstance.SetActive(false);
            }
        }

        /*
        public bool setResourceID(int pos)
        {
            if (this.position == -1)
            {
                position = pos;
                return true;
            }
            else
                return false;
        }


        public void Connect2Net(int ID)
        {
            this.netID = ID;

            TPAgentNetwork.AddResource(netID, this.position, this.name, ResAvailableCallback, this.rMaxAvailable);
            //TPAgentNetwork.AddResource(netID, this.position, this.name, this.rMaxAvailable);
            isConnected = true;
        }
        */


        public bool prepare2Connect(int pos, int ID)
        {
            if (this.position == -1)
               position = pos;
            else
                return false;
        
            this.mindID = ID;
            isConnected = true;
            return true;
        }


        /*
        public void UpdateMaxAvailable()
        {

            if (isConnected)
                //! \todo think what to do when an action consists in leaving a resource. If the action is defined as using the resource, it will provoke a contradiction (see AddResourceUnits in TPPersonality)
                TPAgentNetwork.UpdateResourceAvailable(netID, this.name, rMaxAvailable);
        }
        */

        //! this corresponds to the amount of resource we have (maxAvailable) minus the amount already in use by other competence modules 
        public double ResourceNotUSed
        {
            get
            {
                if (isConnected)
                    return TPAgentNetwork.GetResourceNotUsed(mindID, this.name);//! \todo check the update of the resources is properly done if perceptionID put the code in here.
                else
                    return amountAvailable;

            }


        }





        //! the callback method called by TPAgentNetwork.	
        //[SerializeField]
        //public TPPerceptionBase.TPPerceive perceiveResAvailable;

        
        public static double ResAvailableCallback(int agentID, int resourceID)
        {
            TPResource r = TP.GetAgent(agentID).getConnectedResource(resourceID);
            return r.amountAvailable;

        }




    }


}