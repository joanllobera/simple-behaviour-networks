using UnityEngine;
using System.Collections;
using UnityEditor;

using timepath4unity; 
[UnityEditor.CustomEditor(typeof(TPPersonality))]
public class TPPersonalityInspector : TPPersonalityInspectorBase
{

    public TPPersonalityBox box;

    public override void OnInspectorGUI()
    {
        TPPersonality temp = (TPPersonality)target;

        

        //        if(PrefabUtility.GetPrefabParent(target) == null && PrefabUtility.GetPrefabObject(target) != null)
        if( ! Application.isPlaying &&  //it is not playing
            ! PrefabUtility.GetPrefabType(target).Equals(PrefabType.None)  && //it is a prefab
             ! PrefabUtility.GetPrefabType(target).Equals(PrefabType.PrefabInstance) ) //it is not a prefab instance
        {
            EditorGUILayout.LabelField("Please instantiate the prefab to see it");
        }
        else { 
            if (perso == null || box == null || temp.Equals(perso) == false)
            {
                perso = temp;
                box = new TPPersonalityBox(perso);

            }
            EditorUtility.SetDirty(target);

            box.OnInspectorGUI();
        }

    }
}

