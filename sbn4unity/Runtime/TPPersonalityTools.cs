using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using System;


/*!
\brief
TPPersonalityTools is a container class containing static methods that are useful to build personalities

\todo REMOVE THIS CLASS

\details 

*/
namespace timepath4unity
{

    public static class TPPersonalityTools
    {



        /*
        public static void generateBasicPersonality(TPPersonality ebn){
	
		
            TPPersonalityTools.generatePerceptions(ebn);
	
            //SKILL 1:
		
            //if not food near then ApproachFood with effect isFoodNear 0.9	
            TPSkill temp = ebn.AddSkillByName("ApproachFood");

            temp.Precons.add("isFoodNear", false);
            temp.Effects.addEffect("isFoodNear",0.9f);
		
            //SKILL 2:
            //if always then_doing  avoidEnnemy has_effect not ennemyNear 1.0
            TPSkill temp2 = ebn.AddSkillByName("avoidEnnemy");
            temp2.Precons.add("ennemyNear",true);
            temp2.Effects.addEffect("ennemyNear",1.0f);
            temp2.Effects.negate("ennemyNear");	
		
            //2 GOALS:
		
            // When (always) I want to not beNearEnnemy 1.0
            TPGoal g1 = ebn.AddGoal();
            g1.Rcon.add("perceiveHigh",true);
            g1.Gcon.add("ennemyNear", false); 
            g1.I = 1.0f;
		
            // When (always) I want to isFoodNear 0.6
            TPGoal g2 = ebn.AddGoal();
            g2.Rcon.add("perceiveHigh", true);
            g2.Gcon.add ("ennemyNear", false); 
            g2.Gcon.add("isFoodNear",true);
            g2.I = 0.6f;
        }
	
	
	
        public static void generatePerceptions(TPPersonality ebn){
		
            ebn.AddPerceptionByName("isFoodNear");
            ebn.AddPerceptionByName("perceiveLow");
            ebn.AddPerceptionByName("perceiveHigh");
            ebn.AddPerceptionByName("ennemyNear");
		
		
            ebn.AddResourceByName("Legs");
            ebn.AddResourceByName("Ammo");

        }	
        */





        //from : http://answers.unity3d.com/questions/246116/how-can-perceptionID-generate-a-guid-or-a-uuid-from-within-un.html
        public static string GetUniqueID()
        {
            string key = "ID";

            var random = new System.Random();
            DateTime epochStart = new System.DateTime(1970, 1, 1, 8, 0, 0, System.DateTimeKind.Utc);
            double timestamp = (System.DateTime.UtcNow - epochStart).TotalSeconds;

            string uniqueID = Application.systemLanguage //Language
                //+"-"+GetPlatform() //Device
            + "-" + Application.platform.ToString() //Device
            + "-" + String.Format("{0:X}", Convert.ToInt32(timestamp)) //Time
            + "-" + String.Format("{0:X}", Convert.ToInt32(Time.time * 1000000)) //Time in game
            + "-" + String.Format("{0:X}", random.Next(1000000000)); //random number


            if (PlayerPrefs.HasKey(key))
            {
                uniqueID = PlayerPrefs.GetString(key);
            }
            else
            {
                PlayerPrefs.SetString(key, uniqueID);
                PlayerPrefs.Save();
            }

            return uniqueID;
        }




    }

}