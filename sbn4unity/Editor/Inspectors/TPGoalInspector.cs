using UnityEngine;
using System.Collections;
using UnityEditor;

namespace timepath4unity
{

    [UnityEditor.CustomEditor(typeof(TPGoal))]
    public class TPGoalInspector : Editor
    {


        public override void OnInspectorGUI()
        {
            //draw the list of all perceptions involved in the goal as popup lists, and also include the possibility of adding new ones.
            TPGoal g = (TPGoal)target;
            //GUILayout.Label(g.getRule());
           GUILayout.Label(g.getGoalName());
            g.I = EditorGUILayout.FloatField("goal importance:", g.I);
            
            //base.OnInspectorGUI();
        }

    }

}