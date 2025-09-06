using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;

using System.Linq;
namespace timepath4unity
{
    [UnityEditor.CustomEditor(typeof(TPAgent))]
    public class TPAgentInspector : Editor
    {



        /*[MenuItem("Assets/Timepath/Turn Selection into Timepath Agent")]
        public static void associateTPAgent()
        {
            GameObject go = Selection.activeGameObject;
            if (go)
            {
                go.AddComponent<TPAgent>();

            }
        }
        */




        public override void OnInspectorGUI()
        {
            TPAgent a = (TPAgent)target;

            //select personality:
            a.MyPersoTemplate = TPEditorTools.selectPersonality(a.MyPersoTemplate);

            /*
            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("Body: ");
            a.MyBody =(GameObject) EditorGUILayout.ObjectField(  a.MyBody, typeof(GameObject), true);
            EditorGUILayout.EndHorizontal();
            */
        }





        //! \todo manage to override the selection of plottest and roletest with something a bit nicer, like the attempt below, hwihc is currently amnesiac.

        /*
        public override void OnInspectorGUI()
        {
            TPAgent a = (TPAgent)target;

            //select personality:
            a.MyPersoTemplate = TPEditorTools.selectPersonality(a.MyPersoTemplate);







            a.participateInAllStoriesAndRoles = EditorGUILayout.Toggle("all Stories and Roles", a.participateInAllStoriesAndRoles);

            if (!a.participateInAllStoriesAndRoles)
            {
                EditorGUILayout.LabelField("Stories:");
                string toAdd = "toAdd";
                string toRemove = "toRemove";
                TPEditorTools.selectStringsFromList(a.MyStoryNames, TPEditorTools.getAllPlotNames(), ref toRemove, ref toAdd);
                if (!toRemove.Equals("toRemove"))
                {
                    a.removeStory(toRemove);
                }
                if (!toAdd.Equals("toAdd"))
                {
                    a.addStory(toAdd);
                }

                if (!a.participateInAllStoriesAndRoles)
                {
                    EditorGUILayout.LabelField("Roles:");
                    List<string> allPossibleRoles = new List<string>();
                    foreach (TPPlot plot in a.MyStories)
                    {
                        allPossibleRoles.AddRange(plot.RoleNames);
                    }

                    toAdd = "toAdd";
                    toRemove = "toRemove";
                    TPEditorTools.selectStringsFromList(a.MyRoleNames, allPossibleRoles.Distinct().ToArray(), ref toRemove, ref toAdd);
                    if (!toRemove.Equals("toRemove"))
                    {
                        a.removeRole(toRemove);
                    }
                    if (!toAdd.Equals("toAdd"))
                    {
                        a.addRole(toAdd);
                    }

                }


                if (GUI.changed)
                {
                    EditorUtility.SetDirty(target);
                }


            }


            //! \todo check this for refresh frequency
            TPAgent.refreshFrequency = EditorGUILayout.FloatField("refresh freq: ", TPAgent.refreshFrequency);


        }


*/
    }

    }