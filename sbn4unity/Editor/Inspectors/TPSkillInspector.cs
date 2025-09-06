using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;

using timepath4unity;

[UnityEditor.CustomEditor(typeof(TPSkill))]
public class TPSkillInspector : Editor {

	public override void OnInspectorGUI(){
        TPSkill s = (TPSkill)target;

        GUILayout.Label(s.getRule());

        //! \todo draw one skill, first deal with selecting one single action.

    }
}
