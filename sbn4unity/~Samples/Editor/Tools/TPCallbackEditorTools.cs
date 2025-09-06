using UnityEngine;
using UnityEditor;
using System.Collections;

using timepath4unity;
public class TPCallbackEditorTools : Editor {

	
        //! to show the callbacks available:
        public static void drawTPCallbacks(TPAction a)
        {
            TPCallback removeCallback = null;
            TPCallback addCallback = null;

            TPEditorTools.drawTPCallbacks(a, a.CallbacksSelected, ref removeCallback, ref addCallback);
            if (removeCallback != null)
                a.RemoveCallback(removeCallback);
            if (addCallback != null)
                a.AddCallback(addCallback);


            if (GUI.changed)
            {
                EditorUtility.SetDirty(a);
            }

        }



        public static void drawTPCallbacks(TPPerception a)
        {
            TPCallback removeCallback = null;
            TPCallback addCallback = null;


            TPEditorTools.drawTPCallbacks(a, a.CallbacksSelected, ref removeCallback, ref addCallback);
            if (removeCallback != null)
                a.RemoveCallback(removeCallback);
            if (addCallback != null)
                a.AddCallback(addCallback);


            if (GUI.changed)
            {
                EditorUtility.SetDirty(a);
            }

        }





}
