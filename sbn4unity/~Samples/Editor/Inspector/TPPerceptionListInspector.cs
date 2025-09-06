using UnityEngine;
using UnityEditor;

using System.Collections;

using timepath4unity;

[UnityEditor.CustomEditor(typeof(TPPerceptionList))]
public class TPPerceptionListInspector : Editor {


    public override void OnInspectorGUI()
    {
        TPPerceptionList listContainer = (TPPerceptionList) target;
        TPPerception[] list = listContainer.transform.GetComponentsInChildren<TPPerception>();
        foreach (TPPerception p in list) {
//            EditorGUILayout.BeginHorizontal();
            p.name = EditorGUILayout.TextField( p.name, GUILayout.ExpandWidth(true));
            EditorGUILayout.BeginHorizontal();
            TPCallbackEditorTools.drawTPCallbacks(p);
            if (GUILayout.Button("-", GUILayout.Width(20)))
            {
                GameObject.DestroyImmediate(p.gameObject);
            }
            EditorGUILayout.EndHorizontal();
            EditorGUILayout.Space();
        }




    }
}
