using UnityEngine;
using System.Collections;
using UnityEditor;

/*!

 \brief The building blog of horizontal blocks for story plots to make them vertically draggable
 \date July 2014


\details To use this class: update the position of the destination from the mouse. Then call the function Drag on the origin rectangle.

 \sa  TPPSentenceEditor
*/

namespace timepath4unity
{
#if TRAMA

    public class TPSentenceBox : GUIDraggableObject
    {

        public TPPlotSentence s;

        public Rect origin;
        public Rect destiny;
       // public TPNode TminNode, TmaxNode;
       //public TPInternalCurve TminTmax;


        public void init(TPPlotSentence s, Rect r)
        {
            Vector2 nsize = TPEditorTools.nsize;

            this.origin = r;
            this.destiny = r;
            this.s = s;
            base.setPosition(r.center);

            /*
            TminNode = new TPNode(new Rect(r.xMin - nsize.x - (s.Tmax - s.Tmin) / s.Duration * (Screen.width / 10), r.yMin + (r.height / 2 - nsize.y / 2), nsize.x, nsize.y), TPNode.nodeType.TminTmax);
            TmaxNode = new TPNode(new Rect(r.xMin - nsize.x, r.yMin + (r.height / 2 - nsize.y / 2), nsize.x, nsize.y), TPNode.nodeType.TminTmax);

            TminTmax = TPInternalCurve.CreatePlotSentenceCurve(TminNode, TmaxNode, s);
            */
            


        }




        public void updateTo(float ypos)
        {
            destiny.y = ypos;
            destiny.x = origin.x;

        }
        public void updateTo2(Vector2 v)
        {
            destiny.center = v;

        }


        public void draw()
        {
            if (this.Dragging)
            {
                EditorGUI.LabelField(destiny, s.name, TPPlotInspectorBase.plotstyLight);
            }
            else
            {
                //! \todo make this work better, with a zone to drag, and another to edit the field

                //s.name = EditorGUI.TextField(pr.origin, s.name,  TPPlotEditor.plotsty);
                EditorGUI.LabelField(origin, s.name, TPPlotInspectorBase.plotsty);
            }
            //TminTmax.drawLine();

        }



    }
#endif
}