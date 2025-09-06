


using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using System;
using System.Threading;



//for callbacks:
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


namespace timepath4unity
{ 

/*!

 \brief When using Timepath, each virtual character is a TPAgent.
 \date 07.2014
 \details When using Timepath, each virtual character is a TPAgent.
 
 Each TPAgent has one personality, and can be involved in one or several story plots assuming one or several ROLES
 
 Each TPAgent uses a separate instance of the EBN library (see TPAgentNetwork), for which runs on a separate thread 
 to create thread example: http://msdn.microsoft.com/fr-fr/library/7a2f3ay4%28v=vs.90%29.aspx

in order to allow perceptions which change based on collisions, the agent will store the tags of the objects it collides with

 \todo add the roles and the story plots
 
 \sa TPAgentNetwork TPPersonality TPPlot
*/

//[RequireComponent (typeof (Animator))]
[System.Serializable]
public class TPAgent : MonoBehaviour {
	
	#region Member variables
	private string agentID; //a completely unique ID
	private int netID = -1; //a number given by the dll, starting at 0


//    [SerializeField]
//    private GameObject myBodyInstance;



        //for personalities:
        [SerializeField]
	private  TPPersonality myPersonalityTemplate;
	private TPPersonality myPersonalityInstance; //each agent has a different instance of the same personality

#if TRAMA
        //for plots:
        [SerializeField]
    TPPlot[] MyStories;

    [SerializeField]
    public
    TPRole[] MyRoles;
    private TPPlot[] myStoriesInstance;
#endif

        //! \todo  test stories with external actions, and decide whether each TPPlotSentence really needs a TPPerception


        //perceptions, actions and resources both for the personality and the stories
        private List<TPPerceptionBase> myConnectedPerceptions = new List<TPPerceptionBase>();
    private List<TPActionBase> myConnectedActions = new List<TPActionBase>(); 
    private List<TPResource> myConnectedResources = new List<TPResource>();




#if TRAMA


	[SerializeField]
	public bool participateInAllStoriesAndRoles = false;

        public string[] MyStoryNames{get{ 

			List<string> storyNames = new List<string>();
			/*foreach( TPPlot plot in myStories){
            {
                storyNames.Add(plot.name);
            }*/
            for (int i = 0; i < MyStories.Length; i++)
            {
                storyNames.Add(MyStories[i].name);
            }
            
			return storyNames.ToArray();
		}
	}
  


#endif

        //! \todo recheck TIME MANAGMETN functions:
        [SerializeField]
	private static float refreshFrequency = 50.0f;
	[HideInInspector]
	private float last_time = 0.0f;


#endregion


        #region Property variables

    internal int NetID{get{return netID;}}
	public TPPersonality MyPerso{get{ return myPersonalityInstance;}}
	public TPPersonality MyPersoTemplate{get{ return myPersonalityTemplate; } set{ myPersonalityTemplate = value;}}

    //        public GameObject MyBody { get { return myBodyInstance; } set { myBodyInstance = value; } }
        public GameObject MyBody;
        #endregion




        internal TPPerceptionBase getConnectedPerception(int perceptionID)
    {
        if (myConnectedPerceptions.Count > perceptionID)
            return myConnectedPerceptions[perceptionID];
        else
            return null;

    }
    internal TPActionBase getConnectedAction(int actionID)
    {
        if (myConnectedActions.Count > actionID)
            return myConnectedActions[actionID];
        else
        return null;
        
    }
    internal TPResource getConnectedResource(int resourceID)
    {
        if (myConnectedResources.Count > resourceID)
            return myConnectedResources[resourceID];
        else
            return null;
    }



        /*
    //! select the plots in which the agent can have an explicit role
    TPPlot[] findRelevantStories()
    {
        List<TPPlot> myPlots = new List<TPPlot>();
        //TPPlot[] plots = (TPPlot[])UnityEngine.Resources.FindObjectsOfTypeAll(typeof(TPPlot));

       // TPRole[] roles = (TPRole[])UnityEngine.Resources.FindObjectsOfTypeAll(typeof(TPRole));

        //foreach (TPPlot plot in plots)
        foreach (TPPlot plot in MyStories)
        {
            foreach (TPRole role in MyRoles)
            {
                if (plot.hasRole(role))
                    myPlots.Add(plot);
            }
        }
        return myPlots.Distinct().ToArray();
    }
    */

	void Start () {

		Application.runInBackground = true;
			
		TPAgent[] agents=this.gameObject.GetComponents<TPAgent>();
		if(agents.Length>1){
			Debug.LogError(this.name+ "has " + agents.Length + "TPAgent scripts, and only 1 is possible");	
		}
		
		
		if(myPersonalityTemplate){			
			CreateNet();

            Connect2Net();

		}
		else
			Debug.LogError("I cannot create a Behavior Network for " + this.name + "  because it has no personality");



            /* if (!myBodyInstance)
            if (!MyBody)

            {
                Debug.LogError(" " + this.name + "  does not seem to have a body");


            }
            */

            //mindThread = new Thread(mind.update);

            // if(MyStories)



        }

	void Update () {
		if(MyPerso){
			this.MyPerso.transform.position = this.transform.position;


			if(  (Time.time - this.last_time ) > 1.0f/TPAgent.refreshFrequency ){
               /* foreach (TPResource r in MyPerso.Resources){
                    r.UpdateMaxAvailable();
               }*/
				TPAgentNetwork.UpdateNet(netID);
				this.last_time = Time.time;
			}
		}


	}

	void OnDestroy(){
		TPAgentNetwork.ClearNets();
	}




	protected bool Connect2Net()
	{
		
		//3.a we add all the perceptions, actions and resources defined in the personality
		
		TPPerceptionBase[]  perc = myPersonalityInstance.Perceptions.Distinct().ToArray();
		myConnectedPerceptions.Clear();
		for (int perceptionID = 0; perceptionID < perc.Length; perceptionID++)
		{
			TPPerceptionBase p = perc[perceptionID];
			if(p)
			{
    				p.prepare2Connect(perceptionID, netID);
                    TPAgentNetwork.AddPerception(netID, perceptionID, p.name, p.PerceptionMethod);    //we connect 2 mind:
                    myConnectedPerceptions.Add(p);
			}
		}


		TPSkill[] skills   = myPersonalityInstance.Skills.Distinct().ToArray();//to prevent errors, we make sure there are no repeated elements
		myConnectedActions.Clear();
		for (int actionID = 0; actionID < skills.Length; actionID++)
		{
			TPSkill s = skills[actionID];
			if (s != null)
			{
    				s.Action.prepare2Connect(actionID, netID);
                    TPAgentNetwork.AddAction(netID, actionID, s.name, s.Action.ActionMethod);
                    myConnectedActions.Add(s.Action);
			}
		}

		TPResource[] resources = myPersonalityInstance.Resources.Distinct().ToArray();//to prevent errors, we make sure there are no repeated elements
		myConnectedResources.Clear();

		for (int resID = 0; resID < resources.Length; resID++)
		{
			TPResource r = resources[resID];
			if (r) { 
				r.prepare2Connect(resID, netID);
                    TPAgentNetwork.AddResource(netID, resID, r.name, TPResource.ResAvailableCallback, r.AmountAvailable);

                    myConnectedResources.Add(r);
			}
		}

#if TRAMA

            for (int i = 0; i < myStoriesInstance.Length; i++)
        {

            foreach (TPPlotSentence phrase in myStoriesInstance[i].Plot)
            {
                if (phrase)
                {
                    //3.a we add all the perceptions and actions defined
                    int perceptionID = -1;
                    
                    //JL2016
                    //TPPerceptionBase p = phrase.GetComponent<TPPerception>();
                    TPPerceptionBase p = phrase.GetComponent<TPPerceptionBase>();

                    if (p)
                    {
                        perceptionID = myConnectedPerceptions.Count;
                        p.prepare2Connect(perceptionID, mindID);
                        myConnectedPerceptions.Add(p);
                    }

                    int actionID = -1;
                        //JL2016

                        //     TPActionBase a = phrase.GetComponent<TPAction>();
                        TPActionBase a = phrase.GetComponent<TPActionBase>();
                        if (a)
                    {
                        actionID = myConnectedActions.Count;
                        a.prepare2Connect(actionID, mindID);
                        myConnectedActions.Add(a);
                    }

                    string roleNAME = phrase.getFirstRole();
                    if (roleNAME.Equals(""))//! this will only work well with sentences having 1 role
                    {
                        TPAgentNetwork.AddActionPerception(mindID, perceptionID, actionID, phrase.name, p.PerceptionMethod, a.ActionMethod);
                    }
                    else
                    {
                        TPAgentNetwork.AddActionPerceptionRole(mindID, perceptionID, actionID, phrase.name, p.PerceptionMethod, a.ActionMethod, roleNAME);
                    }

                }

            }//each phrase
        }//each plot

#endif  

	    //4.a we parse the rules in goals and skills
	    string rules = "";
	    foreach (TPGoal g in myPersonalityInstance.Goals)
	    {
		    if (g)
			    rules = rules + g.getRule();
	    }

	    foreach (TPSkill s in myPersonalityInstance.Skills)
	    {
		    if (s)
			    rules = rules + s.getRule();
	    }




#if TRAMA
            //4.b we parse the rules in the plots:
            for (int i = 0; i < myStoriesInstance.Length; i++)
        {

                Debug.Log("i am parsing the story rule (" +i+"/"+myStoriesInstance.Length + "): " + myStoriesInstance[i].getRule());
            rules += rules + myStoriesInstance[i].getRule();
        }
#endif

            rules = rules.Replace("\n", " ");
            //5. we connect the perceptions and actions in the mind through rules
            TPAgentNetwork.ConnectNet(netID, rules);


		return true;
 
	}




        private void CreateNet(){
		

		
		//1 we initialize the method used for the logs
   		TPAgentNetwork.logPointer lPtr = TPAgentNetwork.logPlugin;
		IntPtr fPtrLog = Marshal.GetFunctionPointerForDelegate(lPtr);
		TPAgentNetwork.InitLog (fPtrLog); 



		//2.a we create the mind 
        agentID = TPPersonalityTools.GetUniqueID();
		netID = TPAgentNetwork.CreateNet(agentID);
	


        //2.b we make a copy of the personality to "put it in the mind"
		
		this.myPersonalityInstance = TPPersonality.Instantiate (myPersonalityTemplate) as TPPersonality;
		this.myPersonalityInstance.name = this.myPersonalityTemplate.name + " in " + this.name;
		this.myPersonalityInstance.transform.parent = this.transform;

        //in addition, we make sure that there are no spaces in the names of the skills, perceptions or resource  (we don't care about goals because we don't use goal names)
        TPPersonality.removeSpacesInNames(this.myPersonalityInstance);



#if TRAMA
            //2.c we also instantiate the stories where the agent is involved:

            //we make sure there are no empty assets:

            List<TPPlot> temp = new List<TPPlot>(MyStories);
            temp.RemoveAll(item => item == null);
            myStoriesInstance = new TPPlot[temp.Count];

        for (int i = 0; i < myStoriesInstance.Length; i++)
        {
            myStoriesInstance[i] = TPPlot.Instantiate(temp[i]) as TPPlot;
            myStoriesInstance[i].name = temp[i].name + " in " + this.name;
            myStoriesInstance[i].transform.parent = this.transform;
        }

#endif


            //2.d Finally, we add it to the agents list
            TP.AddAgent(this);



		
        

	}

    //! \todo stuff for collision detection (to be replaced, if possible, by something more elegant interfacing the actions with the collision detectors)
  
    /*
    private List<GameObject> recentCollisions = new List<GameObject>();


    //! gets the gameObject that recently collided, and removes it from the list.
    public GameObject getObjectCollidedWithTag(string theTag){
        GameObject go = recentCollisions.Find (p => p.tag.Equals (theTag));
        recentCollisions.Remove (go);
        return(go); //! \todo try to find a way to go around this. it looks ugly
        }

    void OnTriggerEnter(Collider collider)
    {
        if (recentCollisions.Find (go => go.Equals (collider.gameObject))) {

        } else {
            recentCollisions.Add(collider.gameObject);
        }
    }
    
    */

    //!todo  put back myStories in a list, with MyStories being a property, and with TPAgentInspector enabling a dropdown menu

    /*
    [SerializeField]
    TPAgentStories myStories = new TPAgentStories();
     
     [SerializeField]
	TPAgentRoles myRoles = new TPAgentRoles(); 
      
	public TPPlot[] MyStories{get{ 
			//return myStories.ToArray();
		}
	}
    

	public TPRole[] MyRoles{ get {
                       myRoles.Remove(null);
					   return myRoles.ToArray ();
				} 
	}
    
      
      
     
    public string[] MyRoleNames{get{ 

			myRoles.Remove(null);
			List<string> roleNames = new List<string>();
			foreach( TPRole role in myRoles){
				roleNames.Add(role.name);
			}
			return roleNames.ToArray();
		}
	}

	public bool removeStory(string storyName){
		int perceptionID =myStories.RemoveWhere(s=>s.name.Equals(storyName));
		return perceptionID > 0;
		}

	public bool addStory(string storyName){
		TPPlot[] plotVector = (TPPlot[]) Resources.FindObjectsOfTypeAll(typeof(TPPlot));
		List<TPPlot> plots = new List<TPPlot> (plotVector);
		TPPlot plot = plots.Find (p => p.name.Equals (storyName));
		if (plot) {
						myStories.Add (plot);
						return true;
				} else {
						return false;
				}


	}


	public bool removeRole(string roleName){
		int perceptionID =myRoles.RemoveWhere(r => r.name.Equals(roleName));
		return perceptionID > 0;
	}
	
	public bool addRole(string roleName){

		//since it is a hashSet, we do not need to wonder about repetitions.
		return myRoles.Add(TPRole.getRole (roleName));
		
	}
    */




}

}//namespace timepath