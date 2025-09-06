using UnityEngine;
using System.Collections;

using timepath4unity;

    public static class TPCallbackTools
    {


       


        //! a method to add a perception to a personality
        public static void AddPerceptionByName(TPPersonality perso, string n)
        {

            Transform perContainer = TPPerceptionList.GetPerceptionContainer(perso);


            GameObject go = new GameObject();
            TPPerception pIn = go.AddComponent<TPPerception>();
            pIn.transform.parent = perContainer;
            pIn.name = n;
            pIn.setRandomValue();


        }

#if TRAMA
    //! a method to add a perception to a Plot Sentence
    public static bool setPerception(TPProposition prop, TPPlotSentence ps)
    {
        Transform container = TPPlotSentence.setPerceptionsContainerInHierarchy(ps);
        Transform t = container.FindChild(prop.name);
        if (t != null)
        {
            prop.setExternalPerception( t.GetComponent<TPPerception>(),ps);
        }
        else
        {
            GameObject go = new GameObject();
            prop.setExternalPerception( go.AddComponent<TPPerception>(), ps);
            prop.PerceptionName = "Perception" + ps.PreCon.Count;
        }

        prop.setName();
        return true;
    }

#endif


    //! a method to add a perception from a personality

    static void AddAction2Skill(TPSkill s)
        {

            GameObject go = new GameObject();
        //TPAction a = go.AddComponent<TPAction>();
        go.AddComponent<TPAction>();
        go.transform.parent = s.transform;
            go.name = TPConstant.actionName;
        }

   

      //! a method to add a skill to a personality
        public static TPSkill AddSkillByName(TPPersonality perso, string name)
        {

            Transform skillContainer = TPSkillList.GetSkillContainer(perso);

            GameObject goSkill = new GameObject();
            TPSkill temp = goSkill.AddComponent<TPSkill>();
            goSkill.transform.parent = skillContainer;
            goSkill.name = name;

            TPCallbackTools.AddAction2Skill(temp);
            return temp;
        }

        

    
    
    }


