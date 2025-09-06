using UnityEngine;
using System.Collections;

namespace timepath4unity
{

    public class TPGoalList : MonoBehaviour
    {

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {

        }

        public static Transform GetGoalContainer(TPPersonality perso)
        {
            Component test = perso.transform.GetComponentInChildren<TPGoalList>();
            Transform t = null;
            if (test != null)
                t = test.transform;
           
            else
            {
                t = (new GameObject()).transform;
                t.gameObject.name = TPConstant.goalsContainer;
                t.gameObject.AddComponent<TPGoalList>();
                t.parent = perso.transform;
            }
            return t;


        }


    }


}