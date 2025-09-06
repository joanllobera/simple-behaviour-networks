using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;

namespace timepath4unity
{
#if TRAMA


    //[UnityEditor.CustomEditor(typeof(TPPlotSentence))]
    public class TPSentenceInspectorBase : Editor
    {
        
    protected static TPProposition[] preDrawLogicExpression(TPPlotSentence s)
        {
            s.getRule();
            TPProposition[] props = s.getExternalPropositions();
            return props;

        }

   protected static void drawProposition(TPProposition prop)
        {
            int currentIndex = 1;
            if (prop.IsTrue)
                currentIndex = 0;

            string[] labels = new string[2];
            labels[0] = " - ";
            labels[1] = "NOT";
            int selectedIndex = currentIndex;

            selectedIndex = EditorGUILayout.Popup(selectedIndex, labels, GUILayout.Width(40));

            if (selectedIndex != currentIndex)
            {
                prop.negate();
                selectedIndex = currentIndex;
            }
            prop.PerceptionName = GUILayout.TextArea(prop.PerceptionName, GUILayout.MinWidth(80));
        }
        



        protected void drawName(TPPlotSentence s)
        {
            GUILayout.BeginHorizontal();
            s.name = GUILayout.TextArea(s.name);
            EditorGUILayout.EndHorizontal();



        }



        protected void drawRoles(TPPlotSentence s)
        {


            if (s != null)
            {
                string[] roles = s.getRoles();
                if (roles.Length > 0)
                {
                    GUILayout.Label("Role: " + roles[0]);
                }
            }


            /*		if(s.IsMultiRole){
                        GUILayout.Label ("Roles: ");
                        TPPlotSentence[] kids=s.gameObject.transform.GetComponentsInChildren<TPPlotSentence>();
                        foreach(TPPlotSentence ks in kids){
                            GUILayout.BeginHorizontal();
                            GUILayout.Label (ks.name);
                            if (GUILayout.Button ("+E", GUILayout.Width (30))) {								
                                Selection.activeObject = ks.gameObject;
                            }
                            GUILayout.EndHorizontal();
                        }
                    }
            */
        }





    }

#endif
}