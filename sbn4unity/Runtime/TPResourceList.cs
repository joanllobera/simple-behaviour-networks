using UnityEngine;
using System.Collections;


namespace timepath4unity
{

    public class TPResourceList : MonoBehaviour
    {

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {

        }

        public static Transform GetResourceContainer(TPPersonality perso)
        {
            Component test = perso.transform.GetComponentInChildren<TPResourceList>();
            Transform t = null;
            if (test != null)
                t = test.transform;
            
            else
            {
                t = (new GameObject()).transform;
                t.gameObject.name = TPConstant.resourcesContainer;
                t.gameObject.AddComponent<TPResourceList>();
                t.parent = perso.transform;
            }
            return t;


        }

    }

}