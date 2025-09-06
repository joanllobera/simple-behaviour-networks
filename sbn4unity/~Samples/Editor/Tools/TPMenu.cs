using UnityEditor;
using UnityEngine;
using System.Collections;

using timepath4unity;

public class TPMenu : Editor {

    public static GameObject timepathprefab = null;


    [MenuItem("Assets/Create/Timepath Agent")]
    public static void createNewAgent()
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

        TPAgent[] agents = FindObjectsOfType(typeof(TPAgent)) as TPAgent[];
        int numpersona = agents.Length + 1;


        GameObject spawnedPrefab = GameObject.Instantiate(timepathprefab) as GameObject;
        spawnedPrefab.name = "agent" + numpersona.ToString();
        spawnedPrefab.AddComponent<TPMentalBag>();
        spawnedPrefab.AddComponent<TPAgent>();
        Selection.activeTransform = spawnedPrefab.transform;

    }




}
