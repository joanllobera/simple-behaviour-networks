using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using UnityEditor;

//for mecanimParameters:
//using UnityEditorInternal;

namespace timepath4unity
{


    [UnityEditor.CustomEditor(typeof(TPActionBase))]
    public class TPActionBaseInspector : Editor
    { //or simply, extend the editor?
        //bool showMecanim;
        //bool selectCustomAnimation;
        //bool showHow = false;

        //bool showTPCallback = false;
      
        TPActionBase a;
        TPPersonality ebn = null;




        public override void OnInspectorGUI()
        {
            a = (TPActionBase) target;
            try
            {
                bool ba = (a.transform.parent != null);
                bool bb = (a.transform.parent.parent != null);
                bool bc = false;
                if (bb)
                    bc = (a.transform.parent.parent.GetComponent<TPPersonality>() != null);
                if ((ba && bb) && bc)
                {
                    //it is a classic action within a personality
                    ebn = a.transform.parent.parent.GetComponent<TPPersonality>();
                }
#if TRAMA
                else
                {
                    //! \todo this will not work properly if the sentence has several ROLES

                    string s = a.gameObject.GetComponent<TPPlotSentence>().getRoles()[0];
                   
                    //it is an action within a story role:
                    List<TPRole> listRoles = new List<TPRole>((TPRole[])GameObject.FindObjectsOfType(typeof(TPRole)));

                    //! \todo find out what is the point of these lines exactly (it does not work since roles do not have a perso anymore):
                    /*
                    TPRole role = listRoles.Find(r => r.name.Equals(s));
                    if (role)
                    {
                        ebn = role.Perso;
                    }*/
                }
#endif
            }
            catch { }



        }


        /*
        void drawMecanimCallbacks()
        {
            //started arranging this, TPMecanimParam can be removed freely

            //OBSOLETE:
            //the class AnimatorController can only be used in an editor, it cannot be compiled

            EditorGUILayout.BeginHorizontal();
            GUILayout.Space(16);
            EditorGUILayout.BeginVertical();

            GUILayout.Label("TODO: animation controllers");


            //the controls in the animator:
            

            UnityEditor.Animations.AnimatorController animatorController = (UnityEditor.Animations.AnimatorController)ebn.AnimControl;//AnimatorController.GetEffectiveAnimatorController(a);
         //   int countParameters = animatorController.parameterCount;
         //   UnityEngine.AnimatorControllerParameter[] animationCParameter = new UnityEngine.AnimatorControllerParameter[countParameters];
            UnityEngine.AnimatorControllerParameter[] animationCParameter = animatorController.parameters;
            int countParameters = animationCParameter.Length;

            //TPMecanimParam[] animationParameters = new TPMecanimParam[countParameters];
            UnityEngine.AnimatorControllerParameter[] animationParameters = new UnityEngine.AnimatorControllerParameter[countParameters];

            TPMecanimParam removeParam = null;
            TPMecanimParam addParam = null;


            //we load the list of parameters:
            for (int perceptionID = 0; perceptionID < countParameters; perceptionID++)
            {
                //                animationCParameter[perceptionID] = animatorController.GetParameter(perceptionID);
                animationCParameter[perceptionID] = (UnityEngine.AnimatorControllerParameter) animationParameters[perceptionID];

                if (animationCParameter[perceptionID].type == AnimatorControllerParameterType.Bool)
                {
                    if (animationParameters[perceptionID] == null)
                        animationParameters[perceptionID] = new TPMecanimParam(animationCParameter[perceptionID].name, animationCParameter[perceptionID].defaultBool);
                }
                else if (animationCParameter[perceptionID].type == AnimatorControllerParameterType.Float)
                {
                    if (animationParameters[perceptionID] == null)
                        animationParameters[perceptionID] = new TPMecanimParam(animationCParameter[perceptionID].name, animationCParameter[perceptionID].defaultFloat);
                }
                else if (animationCParameter[perceptionID].type == AnimatorControllerParameterType.Int)
                {
                    if (animationParameters[perceptionID] == null)
                        animationParameters[perceptionID] = new TPMecanimParam(animationCParameter[perceptionID].name, animationCParameter[perceptionID].defaultInt);
                }
                else if (animationCParameter[perceptionID].type == AnimatorControllerParameterType.Trigger)
                {
                    Debug.Log("Warning: no trigger support implemented in actionEditor!"); //! \todo solve GUILayout.Label ("no trigger support implemented!");
                }
            }

            List<string> names = new List<string>();//[animationParameters.Length - a.mecanimParamsSelected.Count];
            List<TPMecanimParam> nonSelectedParams = new List<TPMecanimParam>();


            //we put them into a list for a popup:
            foreach (TPMecanimParam temp in animationParameters)
            {
                TPMecanimParam testMecanParam = a.mecanimParamsSelected.Find(par => par.name.Equals(temp.name));
                if (testMecanParam == null)
                {
                    names.Add(temp.name);
                    nonSelectedParams.Add(temp);
                }
            }
            
            EditorGUILayout.BeginVertical();
            //EditorGUILayout.PrefixLabel(animationCParameter [perceptionID].name);

            int selectedIndex = 0;
            int defaultIndex = 0;
            foreach (TPMecanimParam param in a.mecanimParamsSelected)
            {
                EditorGUILayout.BeginHorizontal();
                if (GUILayout.Button("-", GUILayout.Width(20)))
                    removeParam = param;
                else
                {
                    defaultIndex = names.Count;
                    names.Add(param.name);
                    selectedIndex = defaultIndex;
                    selectedIndex = EditorGUILayout.Popup(selectedIndex, names.ToArray(), GUILayout.MinWidth(80));



                    if (selectedIndex != defaultIndex)
                    {
                        removeParam = param;
                        addParam = nonSelectedParams[selectedIndex];
                    }
                    else
                    {
                        //we update the values in the fields
                        drawParamEditor(param);


                    }
                    names.Remove(param.name);
                }
                EditorGUILayout.EndHorizontal();

            }
          


            EditorGUILayout.BeginHorizontal();
            if (names.Count > 0)
            {
                GUILayout.Label(" +", GUILayout.Width(20));

                defaultIndex = names.Count;
                names.Add(TPEditorTools.noneString);

                selectedIndex = defaultIndex;
                selectedIndex = EditorGUILayout.Popup(selectedIndex, names.ToArray(), GUILayout.MinWidth(80));
                if (selectedIndex != defaultIndex)
                {

                    addParam = nonSelectedParams[selectedIndex];
                }
                names.Remove(TPEditorTools.noneString);

            }
            EditorGUILayout.EndHorizontal();
            EditorGUILayout.EndVertical();

            EditorGUILayout.EndVertical();
            EditorGUILayout.EndHorizontal();

            if (GUI.changed)
            {
                EditorUtility.SetDirty(target);
            }

            if (removeParam != null)
                a.mecanimParamsSelected.Remove(removeParam);
            if (addParam != null)
                a.mecanimParamsSelected.Add(addParam);
        }
        

        
        void drawParamEditor(TPMecanimParam param)
        {
            if (param.type == TPMecanimParam.floatParam)
            {
                param.f = EditorGUILayout.FloatField(param.f, GUILayout.MaxWidth(30));
            }
            else if (param.type == TPMecanimParam.intParam)
            {
                param.i = EditorGUILayout.IntField(param.i, GUILayout.MaxWidth(30));
            }
            else if (param.type == TPMecanimParam.boolParam)
            {
                param.b = EditorGUILayout.Toggle(param.b);
            }

        }
        
    */




    }





}

