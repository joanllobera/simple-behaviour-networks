using UnityEngine;
using System.Collections;
namespace timepath4unity
{
    public class TPSkillList : MonoBehaviour
    {

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {

        }

        public static Transform GetSkillContainer(TPPersonality perso)
        {
            Component test = perso.transform.GetComponentInChildren<TPSkillList>();
            Transform t = null;
            if (test != null)
                t = test.transform;
          
         
            else
            {
                t = (new GameObject()).transform;
                t.gameObject.name = TPConstant.skillsContainer;
                t.gameObject.AddComponent<TPSkillList>();
                t.parent = perso.transform;
            }
            return t;


        }



    }


}