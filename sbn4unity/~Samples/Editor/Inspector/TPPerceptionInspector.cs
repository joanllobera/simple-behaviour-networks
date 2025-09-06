using UnityEngine;
using System.Collections;
using UnityEditor;
using timepath4unity;

[UnityEditor.CustomEditor(typeof(TPPerception))]
    public class TPPerceptionInspector : Editor
    {

      
        public override void OnInspectorGUI()
        {
            TPPerception p = (TPPerception)target;


            TPCallbackEditorTools.drawTPCallbacks(p);
 


        }

    }

