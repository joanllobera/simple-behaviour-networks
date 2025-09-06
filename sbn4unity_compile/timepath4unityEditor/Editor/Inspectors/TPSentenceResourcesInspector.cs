using UnityEngine;
using System.Collections;
using UnityEditor;


namespace timepath4unity
{
#if TRAMA


    //! class not used!
    [UnityEditor.CustomEditor(typeof(TPSentenceResources))]

    public class TPSentenceResourcesInspector : Editor
    {
        /*

    //This does not work:
        public static void Collapse(GameObject go)
        {
            SelectObject(go.transform.GetChild(0).gameObject);
            //EditorApplication.delayCall += () => SelectObject(go); // If you don't put the delay, it would be too fast for the editor - he would select the child and immediately select the parent go so there won't be enough time to unfold - this line is optional all together...
        }

        public static void SelectObject(Object obj)
        {
            Selection.activeObject = obj;
        }
        */

        public override void OnInspectorGUI()
        {

            TPSentenceResources s = (TPSentenceResources)target;
            Debug.Log("i am collapsing: " + s.name);
          //  Collapse(s.gameObject);
        }


    }
    
#endif
}