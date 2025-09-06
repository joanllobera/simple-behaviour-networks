using UnityEngine;
using System.Collections;
using System.Collections.Generic;




using timepath4unity;

/*!
\brief
TPAction implements a Timepath action.
It will be performed by a TPPersonality. 
It can also be part of a TPPlotSentence within a story plot (TPPlot).

 Important:  
do NOT declare class variables, since each Action defined will be executed separately (similar to static methods). 
 It might work for methods called within one same Action, but it is dangerous way to proceed, no garantees.
  
To deal with objects of the scene, it is usefull to declare a mental bag with the elements needed
To get the agent, use:  TPAgent me = TP.GetAgent(mindID);
 
 and eventually the body, either ADAPT or mecanim:  
 Body b = me.GetComponent<Body>();

    or the mental bag:
  TPMentalBag bag = me.GetComponent<TPMentalBag>();
 
 
\todo initialize me and my body at startup 
 
 \todo think if each function can/should return a success bool, as a precondition for the following functions to execute
  
*/
public  class TPAction : TPActionBase
    {

    /*
  protected TPMentalBag MentalBag
  {
      get { return Me.GetComponent<TPMentalBag>(); }
  }


*/
  #region DO NOT USE CLASS VARIABLES IN TPAction


  #endregion

  #region ACTIONS


    /*
  public void approachNearestObjectWithTag(string tag)
      {
          GameObject go = TPPerception.FindClosestObjectTagged(Me.MyBody, tag);


          if (go)
          {
              TPAgent me = TP.GetAgent(mindID);
              Body b = me.MyBody.GetComponent<Body>();
              b.NavGoTo(go.transform.position);
          }

  }
    */


   
    public void approachObjectCalled(string name)
        {

            GameObject go = GameObject.Find(name);
        /*
        if (go)
            {
                Body b = Me.MyBody.GetComponent<Body>();
                b.NavGoTo(go.transform.position);

            }

        */
        }

    /*
        
        public void approachObject(GameObject go )
        {

            if (go)
            {
                TPAgent me = TP.GetAgent(mindID);
                Body b = me.MyBody.GetComponent<Body>();
                b.NavGoTo(go.transform.position);

            }


        }




        public void leaveM()
            {
                TPAgent me = TP.GetAgent(mindID);
                Body b = me.MyBody.GetComponent<Body>();

                TPMentalBag bag = me.GetComponent<TPMentalBag>();

                if (bag.M)
                {
                    TPResource res = me.MyPerso.GetResourceByName("captured_meteorite");
                    res.AmountAvailable = 0.0f;

                    bag.M.GetComponent<Rigidbody>().isKinematic = false;

                    bag.M.GetComponent<Rigidbody>().useGravity = true;

                    //and we put it back with all the meteorites in the hierarchy
                    GameObject temp = GameObject.Find("Meteorites");
                    if (temp)
                    {
                        bag.M.transform.parent = temp.transform;
                    }
                    bag.M = null;




                }
            }


        public void takeMhome()
        {


    //        Body b = Me.MyBody.GetComponent<Body>();
            approachNearestObjectWithTag(MentalBag.destinationTag);

              RaycastHit hitInfo;
                Vector3 dir = transform.TransformDirection(Vector3.down);
                //define the region of arrival with your tag. make sure the baking is done AFTER the regions are frozen, and that they all are rendered.
                    if (Physics.Raycast(transform.position , dir, out hitInfo, 10))
                    {

                        if (hitInfo.collider.tag == MentalBag.destinationTag)
                        {

      //                      leaveM();

                        }

                    }
        }

      */

    /*
        public void selectM()
        {
        
            TPAgent me = TP.GetAgent(mindID);
            Body b = me.MyBody.GetComponent<Body>();
            TPMentalBag bag = me.GetComponent<TPMentalBag>();

        if (bag.M ==null){
                GameObject go = TPPerception.FindClosestObjectTagged(mindID, "pickme");
                if(go != null){
                    bag.M = go.GetComponent<Meteorite>();
                    bag.M.tag = "selected";
                }
            }
            else if (bag.M.tag != "selected") //because it fell on another region, for example
            {
                leaveM();
            }


        }
     


    public void aproachM(float minDist)
        {

          //  Debug.Log("approaching M" + minDist);
            Body b = Me.MyBody.GetComponent<Body>();
            TPMentalBag bag = Me.GetComponent<TPMentalBag>();

            if (bag.M)
            { 
                //we approach
                if ((transform.position - bag.M.transform.position).sqrMagnitude > minDist) //we are more far appart than 1 meter
                {
                    b.NavGoTo(bag.M.transform.position);

                }else { //we are closer than 1 meter
                    b.NavStop();
                }
            }
        }
    */


    /*
    public void lookM(float lookDist)
        {
            Body b = Me.MyBody.GetComponent<Body>();
            TPMentalBag bag = me.GetComponent<TPMentalBag>();

            if (bag.M)
            {

                if ((transform.position - bag.M.transform.position).sqrMagnitude < lookDist)
                    //we reach

                    b.HeadLookAt(bag.M.transform.position);

            }
        }

        public void reachM(float reachDist){
            TPAgent me = TP.GetAgent(mindID);
            Body b = me.MyBody.GetComponent<Body>();
            TPMentalBag bag = me.GetComponent<TPMentalBag>();

            if (bag.M)
            {
                if ((transform.position - bag.M.transform.position).sqrMagnitude < reachDist)
                    b.ReachFor(bag.M.transform.position);
            }
        }


        public void takeControlOverM(float reachDist){
            TPAgent me = TP.GetAgent(mindID);
            Body b = me.MyBody.GetComponent<Body>();
            TPMentalBag bag = me.GetComponent<TPMentalBag>();


            if (bag.M)
            {
                if ((transform.position - bag.M.transform.position).sqrMagnitude < reachDist)
                {

                    bag.M.transform.parent = b.transform;
                    bag.M.tag = "picked";
                 
                    bag.M.GetComponent<Rigidbody>().isKinematic = false; //it follows the hand.
                    bag.M.GetComponent<Rigidbody>().useGravity = false;

                    bag.M.transform.position += new Vector3(0.0f, 2.0f, 0.0f);
                    
                    b.ReachStop();
                    b.HeadLookStop();
                    
                    //me.MyPerso.GetResourceByName("captured_meteorite").AmountAvailable = 1.0f;

                } 
            }
        }


    public void playAnimation(string animname) {
        TPAgent me = TP.GetAgent(mindID);
        Body b = me.MyBody.GetComponent<Body>();
        b.AnimPlay(animname);



    }
    
    */


    #endregion

}

