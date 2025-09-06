using UnityEngine;
using System.Collections;
using UnityEditor;

using timepath4unity;


#if TRAMA
[UnityEditor.CustomEditor(typeof(TPPlotSentence))]
    public class TPSentenceInspector : TPSentenceInspectorBase
    {

        public override void OnInspectorGUI()
        {



        
        TPPlotSentence s = (TPPlotSentence)target;

        base.drawName(s);
        base.drawRoles(s);


        checkAction(s);
        EditorGUILayout.Space();

        checkUserInput(s);
        checkIsExternal(s);
        
             

    }


    protected void checkAction(TPPlotSentence s)
    {
        TPAction a = null;
        try {
            a = s.transform.parent.GetComponentInChildren<TPAction>();
        } catch { }

        if (a == null) { 
            GameObject go = new GameObject();
            go.transform.parent = s.transform.parent;
            go.name = TPConstant.plotAction;
            go.AddComponent<TPAction>();
        }
        GUILayout.Label(TPConstant.plotAction);
        TPCallbackEditorTools.drawTPCallbacks(a);


    }


    protected void drawLogicExpression(TPPlotSentence s)
    {
        TPProposition[] props = preDrawLogicExpression(s);
        foreach (TPProposition prop in props)
        {
            GUILayout.BeginHorizontal();
            drawProposition(prop);
            Transform t = s.getPerceptionTransformFromProposition(prop);
            TPPerception p = t.GetComponent<TPPerception>();
            GUILayout.EndHorizontal();

            GUILayout.BeginHorizontal();

            TPCallbackEditorTools.drawTPCallbacks(p);
            GUILayout.EndHorizontal();

        }
        if (GUILayout.Button("+", GUILayout.Width(20)))
        {
        GameObject go = new GameObject();
        TPProposition prop = go.AddComponent<TPProposition>();
        prop.name = TPConstant.newExternalCondition;
        TPCallbackTools.setPerception(prop, s);

        prop.setName();

        }


    }

    protected void checkIsExternal(TPPlotSentence s)
    {

        //! \todo check if user input can be merged inside external input, similar to a IF x THEN y

        bool temp = GUILayout.Toggle(s.IsExternal, TPConstant.externalActionConditions, GUILayout.Width(150));
        if (temp)
        {
            Transform t = s.transform.parent.FindChild(TPConstant.externalActionFormula + s.name);
            TPLogicExpression le = null;
            if (t == null)
            {
                le = s.setExternalCondition();
            }
            drawLogicExpression(s);
        }
        else if (temp == false && s.IsExternal == true) //it was enabled and we disable it
        {
            s.destroyExternalCondition();
        }

    }






    protected void checkUserInput(TPPlotSentence s)
    {

        bool temp = GUILayout.Toggle(s.userCanPerform, TPConstant.userInputActions, GUILayout.Width(150));
        if (temp)
        {

            Transform t = s.transform.parent.FindChild(TPConstant.userInputActions + s.name);
            TPPerception p = null;
            if (t == null)
            {
                GameObject go = new GameObject();
                p = go.AddComponent<TPPerception>();
                p.name = TPConstant.userInputActions + s.name;
                p.transform.parent = s.transform.parent;
                //Selection.activeObject = a.gameObject;
            }
            else
            {
                p = t.GetComponent<TPPerception>();
            }
            TPCallbackEditorTools.drawTPCallbacks(p);
        }
        else if (temp == false && s.userCanPerform == true) //it was enabled and we disable it
        {
            Transform t = s.transform.parent.FindChild(TPConstant.userInputActions + s.name);
            DestroyImmediate(t.gameObject);
        }
        s.userCanPerform = temp;
    }


}

#endif
