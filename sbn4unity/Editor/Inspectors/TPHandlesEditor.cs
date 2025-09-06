using UnityEngine;
using System.Collections;
using UnityEditor;
namespace timepath4unity
{
    internal class TPHandlesEditor : Editor
    {

        //This exploits a Untiy Bug or undocumented feature.
        //declaring this method in ANY extension of editor makes it apply to ALL gameObjects. We therefore choose to put it in this special class	


//! \todo find a new way to show the gizmos we need


     //   [DrawGizmo(GizmoType.Selected | GizmoType.NotInSelectionHierarchy)]
        static void DrawGameObjectName(Transform transform, GizmoType gizmoType)
        {

            TPAgent a = transform.gameObject.GetComponent<TPAgent>();

            if (a)
                drawTPAgentLabel(a);
            else



                try
                { //some object do not have tags
                    string tag = transform.gameObject.tag;

                    if (tag.Equals("can_group"))
                        Handles.Label(transform.position + Vector3.up * 1.0f, " CAN_GROUP  ");

                }
                catch
                {//if it has no tag the tag is actually Untagged, but for some reason here it throws a special unity exception

                }


        }





        public static void drawTPAgentLabel(TPAgent a)
        {


            string text = "TPAgent: " + a.name;

            if (a.MyPerso != null)
                text = text + "  \n Perso: " + a.MyPerso.name;
            else if (a.MyPersoTemplate != null)
                text = text + "  \n Perso template: " + a.MyPersoTemplate.name;
#if TRAMA
            if (a.MyRoles.Length > 0)
            {
                text = text + " \n  Roles: ";
                foreach (TPRole role in a.MyRoles) { 
                    if(role != null)
                        text = text + role.name;
                }
            }
#endif
            Handles.Label(a.transform.position + Vector3.up * 2.0f, text, TPEditorTools.Hsty);


            //	Handles.Label(a.transform.position + Vector3.up*1.0f, a.name + " BBBB  " );

        }


    }


}