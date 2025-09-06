using UnityEngine;
using System.Collections;
using UnityEditor;

using timepath4unity;


    [UnityEditor.CustomEditor(typeof(TPAction))]
    public class TPActionInspector : TPActionBaseInspector
    {
        TPAction action;
        //bool showTPCallback = true;
        public override void OnInspectorGUI()
        {

           

            base.OnInspectorGUI();

            //showTPCallback = EditorGUILayout.Foldout(showTPCallback, "Custom Actions");
            //if (showTPCallback)
            //{
                action = (TPAction)target;
                TPCallbackEditorTools.drawTPCallbacks(action);
            //}

        }


    }


