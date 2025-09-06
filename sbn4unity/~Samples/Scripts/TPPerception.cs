using UnityEngine;
using System.Collections;

using System.Collections.Generic;


/*!

 \brief When using Timepath, each object that can be perceived can have one or several perceptions (TPPerception) associated
 \date August 2014
 \details 
 
 	Each perception can be created through the Perception Inspector, which allows selecting the functions available in TPPerception. 
 	To define a new perception, it is enough to define a function that does not take more than one parameter, and which changes the double "value".

 	Accesory variables can also be defined (see for example the variables near and far, and the function isTagNear).

 \sa  TPPersonality TPPlot
*/
using timepath4unity;


    [System.Serializable]
    public class TPPerception : TPPerceptionBase
    {


        #region VARIABLES USED BY SOME PERCEPTION FUNCTIONS

        float far = 100.0f;
        float near = 0.0f;
        GameObject target = null;
        #endregion



        #region PERCEPTION FUNCTIONS AVAILABLE IN MENU_______________________________________________________________________________________
        public void perceiveLow()
        {

            value = 0.096;
        }

        public void perceiveHigh()
        {
            value = 0.98;
        }


        public void getResourceCount(string name)
        {
            TPResource res = Me.MyPerso.GetResourceByName(name);
            if (res == null)
                value = 0.0;
            else if (res.AmountAvailable > 1.0)
                value = 1.0;
            else if (res.AmountAvailable <= 0)
                value = 0.0;
            else
                value = res.AmountAvailable;

        }

        public void doIHaveAtLeastOneOfResource(string name)
        {
            TPResource res = Me.MyPerso.GetResourceByName(name);
            if (res == null)
                value = 0.0;
            else if (res.AmountAvailable >= 1.0)
                value = 1.0;
            else
                value = 0.0;
        }






        //! \todo allow functions not affecting value to run only at startup
        public void defineNear(float near)
        {
            this.near = near;

        }

        public void defineFar(float far)
        {
            this.far = far;
        }


    /*
        //! it gives a fuzzy value between far and near
        public void isTagNear(string theTag)
        {

            double isNear;
            GameObject NearestObject = FindClosestObjectTagged(MyBody, theTag);

            if (NearestObject == null)
                isNear = 0.0;
            else
            {
                double dist = (NearestObject.transform.position - MyBody.transform.position).magnitude;

                if (dist > far)
                    isNear = 0.0;
                else if (dist < near)
                    isNear = 1.0;
                else
                    isNear = (double)(1.0f - (dist - near) / (far - near));
            }
            value = isNear;
        }

        public void selectNearestObjectWithTag(string theTag)
        {
            target = FindClosestObjectTagged(MyBody, theTag);
        }

    */



        //! \todo NOT TESTED
        //! are there several of this tag?
        public void isTargetNearSiblings()
        {
            if (target != null && target.tag != "")
            {
                GameObject[] ObjectsInRadius = FindObjectTaggedInRadius(target, target.tag, far, near);
                if (ObjectsInRadius.Length > 1)
                    value = 1.0;
                else
                    value = 0.0;
            }
            else
                value = 0.0;
        }


        //! \todo NOT TESTED
        public void isTargetNotNearSiblings()
        {
            isTargetNearSiblings();
            value = 1.0 - value;
        }





        #endregion

        #region OTHER USEFUL FUNCTIONS___________________________________________________________________
        public static GameObject FindClosestObjectTagged(GameObject mybody, string theTag)
        {
            GameObject[] gos;
            gos = GameObject.FindGameObjectsWithTag(theTag);
            GameObject closest = null;
            float distance = Mathf.Infinity;
            Vector3 position = mybody.transform.position;
            // Iterate through them and find the closest one
            foreach (GameObject go in gos)
            {
                float curDist = (go.transform.position - position).magnitude;
                if (curDist < distance)
                {
                    closest = go;
                    distance = curDist;
                }
            }

            if (closest != null && !closest.tag.Equals(theTag))
            {
                Debug.Log("the object selected does not have  the tag " + theTag + "but rather " + closest.tag);
                return null;
            }


            return closest;
        }


      

        public static GameObject[] FindObjectTaggedInRadius(GameObject target, string theTag, float far, float near)
        {
            GameObject[] gos;
            gos = GameObject.FindGameObjectsWithTag(theTag);
            List<GameObject> closeObjects = new List<GameObject>();
            Vector3 position = target.transform.position;

            foreach (GameObject go in gos)
            {
                float curDist = (go.transform.position - position).magnitude;
                if (curDist < far && curDist > near)
                {
                    closeObjects.Add(go);
                }
            }
            return closeObjects.ToArray();
        }


        #endregion


    }

