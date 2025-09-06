using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
	

namespace timepath4unity
{
#if TRAMA

    /*!

     \brief This class implements the Base of the main story editor. 
     \date July 2014


    \details
    This class implements the main story editor. It is the Custom Inspector of the class TPPlot. 
    The relation between TPPlotEditor and TPSentenceEditor is similar to TPCurvesAndRules and TPInternalCurve




     \sa  TPCurvesAndRules, TPSkillBox, TPGoalBox, TPProposition
    */



    public class TPPlotInspectorBase : Editor
    {

        protected  TPPlot plot = null;

        public static GameObject TPPlotPrefab = null;

        //static Vector2 psize;//see onEnable
        //static float ysep;

        bool showSentences = true;
        //bool showTimeline = false;
        bool showRoles = false;


        //List<TPSentenceBox> SentenceBoxes = new List<TPSentenceBox>();
        public static GUIStyle plotsty;
        public static GUIStyle plotstyLight;

        //when visualiszing the plot sentences, there is possible a scroll area:
        //Vector2 scrollPos = new Vector2(0, 0);

        [
    ("Assets/Create/Timepath Story")]
        public static void createNewStory()
        {
            if (!TPPlotPrefab)
            {
                string name = "TimepathPrefab";
                TPPlotPrefab = Resources.Load(name, typeof(GameObject)) as GameObject;
                if (!TPPlotPrefab)
                {
                    Debug.Log("The prefab " + name + " does not seem to be in the Timepath Resources folder");
                    return;
                }


            }
            TPPlot[] plots = FindObjectsOfType(typeof(TPPlot)) as TPPlot[];
            int numplots = plots.Length + 1;

            GameObject spawnedPrefab = GameObject.Instantiate(TPPlotPrefab) as GameObject;
            spawnedPrefab.AddComponent<TPPlot>();
            spawnedPrefab.name = "Story" + numplots.ToString();
            Selection.activeTransform = spawnedPrefab.transform;

        }

        private void OnEnable()
        {

            //psize = TPEditorTools.psize;
            //ysep = TPEditorTools.sepy;

            GUISkin TPSkin = Resources.Load("TPSkin", typeof(GUISkin)) as GUISkin;

            plotsty = TPSkin.GetStyle("PlotSentenceStyle");
            plotstyLight = TPSkin.GetStyle("PlotSentenceStyleLight");
        }

        /*
        public void Update ()
        {	
            Repaint (); //if this is not present, it gets stuck on the last selecteed
        }*/


//for oninspectorGUI;
        protected void drawPlotSentences()
        {


            

            EditorGUILayout.BeginVertical();
            GUILayout.Space(5);

            showSentences = EditorGUILayout.Foldout(showSentences, "show script");

            if (showSentences)
            {
                plot.Plot.RemoveAll(t => t==null);
                foreach (TPPlotSentence s in plot.Plot) {
                //for (int i = 0; i < plot.Plot.Count; i++)
                //{
                    //TPPlotSentence s = plot.Plot[i];
                    //s.updateChangesInSentence();

                    s.getRoles();
                    
                    TPEditorTools.drawListItem(s);
                }
                //plot.Plot.Remove(null);
                EditorGUILayout.EndVertical();
            }


        }


        //! Adds the components related to a sentence if they do not exist, and it returns the roles present
        public void SentenceSetup(TPPlotSentence sentence)
        {
            string[] roles = sentence.getRoles();


            if (roles.Length <= 1)
            {

                //we create the resources needed to process such sentence:

                /*
                GameObject go = new GameObject();
                go.transform.parent = sentence.transform.parent;
                go.name = TPConstant.plotAction;
                go.AddComponent<TPActionBase>();
                */
                

            }   //!  \todo DEVELOP the case of multiple roles


            /* else if (roles.Count != singleRoles.Count) {//if there are more than 1, we create a nested structure

                    foreach(TPPlotSentence l in this.singleRoles){
                        singleRoles.Remove(l);
                        GameObject.DestroyImmediate(l);
                    }
                    foreach (string r in roles) {
                            GameObject go = new GameObject ();

                            TPPlotSentence s = go.AddComponent<TPPlotSentence> ();
                            s.name = TPPlotSentence.returnOneRoleSentence (s.name, r);
                            s.a = go.AddComponent<TPActionBase> ();
                            s.p = go.AddComponent<TPPerceptionBase> ();
                            go.transform.parent = this.transform;
                            this.singleRoles.Add (s);

                    }
            }*/
            else
            {
                //! \todo dynamic transitions going from single to multiple roles, and viceversa, should better preserve the p and the a given for each role.
                Debug.Log("Plot Sentence " + this.name + " seems to involve more than one role. This functionality is currently not supported");
            }
        }



        /*

        protected void drawTimeline(){

        EditorGUILayout.Space();

        showTimeline = EditorGUILayout.Foldout(showTimeline, "show timeline");
        if (showTimeline)
        {

            EditorGUILayout.BeginVertical();
            this.scrollPos = EditorGUILayout.BeginScrollView(this.scrollPos);

            bool need2RebuildBoxes = false;
            if (SentenceBoxes.Count != plot.Plot.Count)
            {
                need2RebuildBoxes = true;
            }
            else
            {
                bool someDragging = false;
                foreach (TPSentenceBox pr in SentenceBoxes)
                {
                    pr.updateTo(Event.current.mousePosition.y);


                    //this drawing can be encapsulated:
                    if (pr.Dragging)
                    {
                        EditorGUI.LabelField(pr.destiny, pr.s.name, TPPlotInspectorBase.plotstyLight);
                    }
                    else
                    {
                        //! \todo make this work better, with a zone to drag, and another to edit the field
                        //s.name = EditorGUI.TextField(pr.origin, s.name,  TPPlotEditor.plotsty);
                        EditorGUI.LabelField(pr.origin, pr.s.name, TPPlotInspectorBase.plotsty);
                    }
//                        pr.TminTmax.drawLine();



                    if (pr.Dragging)
                    {
                        someDragging = true;
                        //we are dragging but we will very soon stop
                        if (Event.current.type == EventType.MouseUp)
                        {
                            need2RebuildBoxes = rebuildOrder(plot.Plot, SentenceBoxes, pr);

                            break;
                        }
                    }

                    pr.Drag(pr.origin);
                }
                if (someDragging)
                    Repaint();
            }

            if (need2RebuildBoxes)
            {

                TPPlotInspectorBase.rebuildTimeLine(plot.Plot, SentenceBoxes);
                plot.rebuildTransformsHierarchy();
            }


            if (GUI.changed)
            {
                EditorUtility.SetDirty(plot);
            }
            EditorGUILayout.EndScrollView();
            EditorGUILayout.EndVertical();
        }
        }
        */



        protected void drawRoles(){

            EditorGUILayout.Space();

            EditorGUILayout.BeginVertical();
            showRoles = EditorGUILayout.Foldout(showRoles, "story roles:");

            if (showRoles)
            {
                foreach (TPRole r in plot.getPlotRolesFromGlobalList())
                {
                    if (r)
                    {
                        EditorGUILayout.BeginHorizontal();
                        GUILayout.Label(r.name);
                       // r.Perso = TPEditorTools.selectPersonality(r.Perso);
                        EditorGUILayout.EndHorizontal();
                    }
                }
            }

            EditorGUILayout.EndVertical();
            EditorGUILayout.Space();

        }
        /*
        private static void rebuildTimeLine(List<TPPlotSentence> PlotSentences, List<TPSentenceBox> SentenceBoxes)
        {
            float increment = 0;
            float ypos = 0;
            SentenceBoxes.Clear();
            foreach (TPPlotSentence s in PlotSentences)
            {
                TPSentenceBox pr = TPSentenceBox.CreateInstance<TPSentenceBox>();

                increment += (s.Tmin + s.Tmax) / s.Duration * (Screen.width / 10);
                pr.init(s, new Rect(increment, ypos, increment + Screen.width / 2, psize.y));
                SentenceBoxes.Add(pr);
                ypos += psize.y + ysep;

            }
        }
        */

        /*
        private static bool rebuildOrder(List<TPPlotSentence> PlotSentences, List<TPSentenceBox> PlotBoxes, TPSentenceBox movedBox)
        {

            int i = -1;
            if (movedBox.destiny.y < PlotBoxes[0].origin.y)
                i = 0;//we moved to the first position
            else
                i = PlotBoxes.FindLastIndex(box => box.origin.yMin < movedBox.destiny.yMin);

            if (i >= 0 && i < PlotSentences.Count)
            {
                PlotSentences.Remove(movedBox.s);
                PlotSentences.Remove(null);
                PlotSentences.Insert(i, movedBox.s);
                //! \todo redo hierarchy of parents

                return true;
            }
            else
            {
                return false;
            }

        }
        */


    }

#endif
}