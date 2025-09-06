using UnityEngine;
using UnityEditor;
using System.Collections;


using timepath4unity;

[UnityEditor.CustomEditor(typeof(TPSkillList))]
public class TPSkillListInspector : Editor
{
    TPPersonalityBox box;

    public override void OnInspectorGUI()
    {
        TPSkillList listContainer = (TPSkillList) target;

        TPPersonality perso = listContainer.transform.parent.GetComponent<TPPersonality>();

   

        if (perso == null || box == null )
        {

            box = new TPPersonalityBox(perso);

        }
        EditorUtility.SetDirty(target);

        box.OnInspectorGUISkills();




    }
}
