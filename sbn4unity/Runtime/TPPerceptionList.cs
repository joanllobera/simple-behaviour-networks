using UnityEngine;
using System.Collections;
namespace timepath4unity
{
    public class TPPerceptionList : MonoBehaviour
    {

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {

        }

        public static Transform GetPerceptionContainer(TPPersonality perso)
        {
            Component test = perso.transform.GetComponentInChildren<TPPerceptionList>();
            Transform t = null;
            if (test != null) { 
                t = test.transform;
            }

            else
            {
                t = (new GameObject()).transform;
                t.gameObject.name = TPConstant.perceptionsContainer;
                t.gameObject.AddComponent<TPPerceptionList>();
                t.parent = perso.transform;
            }

            return t;
        }





    }

}
