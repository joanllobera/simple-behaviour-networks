using UnityEngine;
using UnityEditor;
using System.Collections;


using timepath4unity;

[UnityEditor.CustomEditor(typeof(TPGoalList))]
public class TPGoalListInspector : Editor
{
    TPPersonalityBox box;

    public override void OnInspectorGUI()
    {
        TPGoalList listContainer = (TPGoalList)target;

        TPPersonality perso = listContainer.transform.parent.GetComponent<TPPersonality>();



        if (perso == null || box == null)
        {

            box = new TPPersonalityBox(perso);

        }
        EditorUtility.SetDirty(target);

        box.OnInspectorGUIGoals();




    }
}