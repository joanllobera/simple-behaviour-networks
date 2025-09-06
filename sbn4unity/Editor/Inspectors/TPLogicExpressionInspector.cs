using UnityEngine;
using System.Collections;
using UnityEditor;
using timepath4unity;


[UnityEditor.CustomEditor(typeof(TPLogicExpression))]
public class TPLogicExpressionInspector : Editor
{

    public override void OnInspectorGUI()
    {
        TPLogicExpression le = (TPLogicExpression)target;


        string rule = le.getRule();
        if (le.UsesResources)//it relates to using resources, not a precondition
        {
            rule = le.getResourcesRule();
        }

        if (rule == "")
            rule = TPLogicExpression.EMPTYRULE;
        EditorGUILayout.BeginHorizontal();
        GUILayout.Label(rule);
        EditorGUILayout.EndHorizontal();
        }
}