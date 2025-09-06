using UnityEngine;
using System.Collections;
using UnityEditor;
namespace timepath4unity
{
   // [UnityEditor.CustomEditor(typeof(TPPersonality))]
    public class TPPersonalityInspectorBase : Editor
    {
        public static GameObject timepathprefab = null;

     
        public TPPersonality perso = null;

      

        [MenuItem("Assets/Create/Timepath Personality")]
        public static void createNewPersonality()
        {
            if (!timepathprefab)
            {
                string name = "TimepathPrefab";
                timepathprefab = Resources.Load(name, typeof(GameObject)) as GameObject;
                if (!timepathprefab)
                {
                    Debug.Log("The prefab " + name + " does not seem to be in the Timepath Resources folder");
                    return;
                }


            }

            TPPersonality[] personalities = FindObjectsOfType(typeof(TPPersonality)) as TPPersonality[];
            int numpersona = personalities.Length + 1;


            GameObject spawnedPrefab = GameObject.Instantiate(timepathprefab) as GameObject;
            spawnedPrefab.name = "personality type" + numpersona.ToString();
            spawnedPrefab.AddComponent<TPPersonality>();
            Selection.activeTransform = spawnedPrefab.transform;

        }

      

    


    }

}