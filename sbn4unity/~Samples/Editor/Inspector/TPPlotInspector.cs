using UnityEngine;
using UnityEditor;
using System.Collections;
using timepath4unity;

#if TRAMA

/*!

 \brief This class implements the main story editor. It is the Custom Inspector of the class TPPlot. 
 \date July 2014


\details
This class implements the main story editor. It is the Custom Inspector of the class TPPlot. 
The relation between TPPlotEditor and TPSentenceEditor is similar to TPCurvesAndRules and TPInternalCurve




 \sa  TPCurvesAndRules, TPSkillBox, TPGoalBox, TPProposition
*/

[UnityEditor.CustomEditor(typeof(TPPlot))]
public class TPPlotInspector : TPPlotInspectorBase
{

	


    public override void OnInspectorGUI()
    {

        plot = (TPPlot)target;
        drawPlotSentences();
     
        if (GUILayout.Button("+", GUILayout.Width(20)))
        {

            GameObject go = new GameObject();
            TPPlotSentence s = go.AddComponent<TPPlotSentence>();
            if (s != null) { //in case it was deleted, the button might still be active

                //! \todo give the plot vector, and put everything in a static function in TPPlotSentence

                s.name = "Sentence" + plot.Plot.Count.ToString();
                if (plot.Plot.Count > 0)
                    s.transform.parent = plot.Plot[plot.Plot.Count - 1].transform;
                else
                    s.transform.parent = plot.transform;
                plot.Plot.Add(s);

                SentenceSetup(s);

                
                //Selection.activeTransform = s.transform;
            }
        }

        drawRoles();

    }

    


}


#endif
