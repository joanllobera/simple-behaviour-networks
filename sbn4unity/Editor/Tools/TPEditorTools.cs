using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;

namespace timepath4unity
{
public static class TPEditorTools
    {
        public static string noneString = " (NONE) ";


        //styles:
        static private GUISkin TPSkin;

        static private GUIStyle psty;//perceptions

        public static GUIStyle Psty
        {
            get
            {
                if (psty == null)
                    TPEditorTools.InitStyles();
                return TPEditorTools.psty;
            }

        }

        static public GUIStyle rsty;//resource

        public static GUIStyle Rsty
        {
            get
            {
                if (rsty == null)
                    TPEditorTools.InitStyles();
                return TPEditorTools.rsty;
            }

        }

        static private GUIStyle gsty;//goal

        public static GUIStyle Gsty
        {
            get
            {
                if (gsty == null)
                    TPEditorTools.InitStyles();
                return TPEditorTools.gsty;
            }
        }

        static private GUIStyle ssty;//skill
        static private GUIStyle sstyA;//skill activated

        public static GUIStyle Ssty
        {
            get
            {
                if (ssty == null)
                    TPEditorTools.InitStyles();
                return TPEditorTools.ssty;
            }
        }


        public static GUIStyle SstyA
        {
            get
            {
                if (ssty == null)
                    TPEditorTools.InitStyles();
                return TPEditorTools.sstyA;
            }
        }





        static private GUIStyle nsty;

        public static GUIStyle Nsty
        {
            get
            {
                if (nsty == null)
                    TPEditorTools.InitStyles();
                return TPEditorTools.nsty;
            }
        }

        static private GUIStyle bsty; //buttons

        public static GUIStyle Bsty
        {
            get
            {
                if (bsty == null)
                    TPEditorTools.InitStyles();
                return TPEditorTools.bsty;
            }

        }

        static private GUIStyle tsty;//text

        public static GUIStyle Tsty
        {
            get
            {
                if (tsty == null)
                    TPEditorTools.InitStyles();
                return TPEditorTools.tsty;
            }
        }

        static private GUIStyle hsty = null;//handles

        public static GUIStyle Hsty
        {
            get
            {
                if (hsty == null)
                    TPEditorTools.InitStyles();
                return TPEditorTools.hsty;
            }
        }



        static public Vector2 psize = new Vector2(20, 20);
        static public Vector2 plabelsize = new Vector2(80, 20);

        static public Vector2 nsize = new Vector2(10, 10);

        static public Vector2 gsize = new Vector2(60, 20);
        static public Vector2 ssize = new Vector2(100, 20);

        static public int sepx = 10;//min separation between blocks
        static public int sepy = 20;


        static public void InitStyles()
        {

            TPSkin = Resources.Load("TPSkin", typeof(GUISkin)) as GUISkin;


            psty = TPSkin.GetStyle("PerceptionStyle");		//perception style:		
            rsty = TPSkin.GetStyle("ResourceStyle"); 		//resource style



            tsty = TPSkin.GetStyle("textfield");

            gsty = TPSkin.GetStyle("GoalStyle"); 		//goal style
            ssty = TPSkin.GetStyle("SkillStyle"); 		//skill style

            sstyA = TPSkin.GetStyle("SkillStyleActivated");

            nsty = TPSkin.GetStyle("SkillStyle"); 		//node style

            hsty = TPSkin.GetStyle("HandleStyle");

            bsty = TPSkin.GetStyle("button");
            bsty.normal.background = bsty.hover.background;

        }





        public static TPPersonality selectPersonality(TPPersonality selected)
        {
            int index = 0;
            TPPersonality[] persoVector = (TPPersonality[])Resources.FindObjectsOfTypeAll(typeof(TPPersonality));
            //List<Animator> anims = new List<Animator>(animsVector);
            string[] persoNames = new string[persoVector.Length + 1];
            int i = 0;
            persoNames[0] = "  (none)  ";
            foreach (TPPersonality a in persoVector)
            {
                if (selected && a.name.Equals(selected.name))
                {
                    index = i + 1;
                }
                persoNames[i + 1] = a.name;
                i++;
            }
            index = EditorGUILayout.Popup("Personality:", index, persoNames);
            if (index > 0 && persoVector.Length > 0)
                return persoVector[index - 1];
            else
                return null;

        }


        public static AnimationClip selectAnimation(AnimationClip selected)
        {
            int index = 0;
            AnimationClip[] clipVector = (AnimationClip[])Resources.FindObjectsOfTypeAll(typeof(AnimationClip));
            //List<Animator> anims = new List<Animator>(animsVector);
            string[] clipNames = new string[clipVector.Length + 1];
            int i = 0;
            clipNames[0] = "  (none)  ";
            foreach (AnimationClip a in clipVector)
            {
                if (selected && a.name.Equals(selected.name))
                {
                    index = i + 1;
                }
                clipNames[i + 1] = a.name;
                i++;
            }
            index = EditorGUILayout.Popup("Animation:", index, clipNames);
            if (index > 0 && clipVector.Length > 0)
                return clipVector[index - 1];
            else
                return null;

        }

        //for callbacks:





        static void drawCallbackEditor(TPCallback cb)
        {


            if (cb.callbackType == TPCallback.CallbackType.Float)
            {
                cb.FloatP = EditorGUILayout.FloatField(cb.FloatP);
            }
            else if (cb.callbackType == TPCallback.CallbackType.Int)
            {
                cb.IntP = EditorGUILayout.IntField(cb.IntP);
            }
            else if (cb.callbackType == TPCallback.CallbackType.String)
            {
                cb.StringP = EditorGUILayout.TextField(cb.StringP);
            }
            else if (cb.callbackType == TPCallback.CallbackType.GameObject)
            {
                cb.GoP = (GameObject)EditorGUILayout.ObjectField(cb.GoP, typeof(GameObject), true);
            }
            else if (cb.callbackType == TPCallback.CallbackType.Bool)
            {
                cb.BoolP = EditorGUILayout.Toggle(cb.BoolP);
            }

        }



        public static void drawListItem(Component s)
        {
            GUILayout.BeginHorizontal();

            s.name = GUILayout.TextField(s.name);

            if (GUILayout.Button("Edit", GUILayout.Width(40)))
            {//! \todo find the look into icon
                Selection.activeTransform = s.transform;
                EditorGUIUtility.PingObject(s.gameObject);
            }
            if (GUILayout.Button("-", GUILayout.Width(20)))
            {
                GameObject.DestroyImmediate(s.gameObject);
            }
            GUILayout.EndHorizontal();



        }











        public static void drawTPCallbacks(Component a, TPCallback[] CallbacksSelected, ref  TPCallback removeCallback, ref TPCallback addCallback)
        {

            //the class AnimatorController can only be used in an editor, it cannot be compiled

            EditorGUILayout.BeginHorizontal();
            GUILayout.Space(16);
            EditorGUILayout.BeginVertical();




            string[] methodNames = TPCallbackReflection.GetMethodNames(a);



            //List<string> names = new List<string>();
            //List<TPCallback> nonSelectedCallbacks = new List <TPCallback> ();
            List<string> cb2choose = new List<string>();

            //we put them into a list for a popup:
            foreach (string temp in methodNames)
            {
                TPCallback testCallback = TPCallbackReflection.GetCallbackWithName(CallbacksSelected, temp);


                if (testCallback == null)
                {
                    cb2choose.Add(temp);
                }
            }


            EditorGUILayout.BeginVertical();

            int selectedIndex = 0;
            int defaultIndex = 0;
            foreach (TPCallback cb in CallbacksSelected)
            {
                EditorGUILayout.BeginHorizontal();

                if (GUILayout.Button("-", GUILayout.Width(20)))
                    removeCallback = cb;
                else
                {
                    defaultIndex = cb2choose.Count;
                    cb2choose.Add(cb.MethodName);
                    selectedIndex = defaultIndex;
                    selectedIndex = EditorGUILayout.Popup(selectedIndex, cb2choose.ToArray(), GUILayout.MinWidth(80));



                    if (selectedIndex != defaultIndex)
                    {
                        removeCallback = cb;
                        addCallback = TPCallbackReflection.GetCallback2Method(a, cb2choose[selectedIndex]);
                    }
                    else
                    {
                        //we update the values in the fields
                        drawCallbackEditor(cb);


                    }
                    cb2choose.Remove(cb.ToString());
                }
                EditorGUILayout.EndHorizontal();

            }



            EditorGUILayout.BeginHorizontal();
            if (cb2choose.Count > 0)
            {
                GUILayout.Label("+", GUILayout.Width(20));

                defaultIndex = cb2choose.Count;
                cb2choose.Add(noneString);

                selectedIndex = defaultIndex;
                selectedIndex = EditorGUILayout.Popup(selectedIndex, cb2choose.ToArray(), GUILayout.MinWidth(80));
                if (selectedIndex != defaultIndex)
                {

                    addCallback = TPCallbackReflection.GetCallback2Method(a, cb2choose[selectedIndex]);
                }
                cb2choose.Remove(noneString);

            }
            EditorGUILayout.EndHorizontal();
            EditorGUILayout.EndVertical();

            EditorGUILayout.EndVertical();
            EditorGUILayout.EndHorizontal();




        }









        //More general, adapted from the callbacks method:
        public static void selectStringsFromList(string[] selectedStrings, string[] allStrings, ref  string toRemove, ref string toAdd)
        {


            List<string> SelectedStrings = new List<string>(selectedStrings);
            List<string> nonSelectedParams = new List<string>();


            //we put the non selected strings into a list, for a popup:
            foreach (string temp in allStrings)
            {
                string test = SelectedStrings.Find(par => par.Equals(temp));
                if (test == null)
                {
                    nonSelectedParams.Add(temp);
                }
            }

            EditorGUILayout.BeginVertical();
            //EditorGUILayout.PrefixLabel(animationCParameter [perceptionID].name);

            int selectedIndex = 0;
            int defaultIndex = 0;
            foreach (string param in selectedStrings)
            {
                EditorGUILayout.BeginHorizontal();
                if (GUILayout.Button("-", GUILayout.Width(20)))
                    toRemove = param;
                else
                {
                    defaultIndex = nonSelectedParams.Count;
                    nonSelectedParams.Add(param);
                    selectedIndex = defaultIndex;
                    //selectedIndex = EditorGUILayout.Popup (selectedIndex, names.ToArray (), GUILayout.Width (80));
                    selectedIndex = EditorGUILayout.Popup(selectedIndex, nonSelectedParams.ToArray(), GUILayout.Width(80));


                    if (selectedIndex != defaultIndex)
                    {
                        toRemove = param;
                        toAdd = nonSelectedParams[selectedIndex];
                    }
                    else
                    {
                        //we update the values in the fields

                    }
                    nonSelectedParams.Remove(param);
                }
                EditorGUILayout.EndHorizontal();

            }



            EditorGUILayout.BeginHorizontal();
            if (nonSelectedParams.Count > 0)
            {
                string noneString = " (none) ";
                GUILayout.Label("+", GUILayout.Width(20));

                defaultIndex = nonSelectedParams.Count;
                nonSelectedParams.Add(noneString);

                selectedIndex = defaultIndex;
                selectedIndex = EditorGUILayout.Popup(selectedIndex, nonSelectedParams.ToArray(), GUILayout.Width(80));
                if (selectedIndex != defaultIndex)
                {

                    toAdd = nonSelectedParams[selectedIndex];
                }
                nonSelectedParams.Remove(noneString);

            }
            EditorGUILayout.EndHorizontal();
            EditorGUILayout.EndVertical();



        }

#if TRAMA
        public static string[] getAllPlotNames()
        {
            TPPlot[] plotVector = (TPPlot[])Resources.FindObjectsOfTypeAll(typeof(TPPlot));
            string[] plotNames = new string[plotVector.Length];
            for (int i = 0; i < plotVector.Length; i++)
                plotNames[i] = plotVector[i].name;
            return plotNames;

        }
#endif



    }
}
