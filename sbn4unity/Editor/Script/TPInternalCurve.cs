
using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;


/*!

 \brief This class implements the lines representing the rules in the user interface. It is the GUI of the class TPProposition
 \date July 2014

\todo think if TPInternalCurve should not become the GUI of the class TPLogicExpression, and this one only be used by TPcurvesAndRules


 \sa  TPCurvesAndRules, TPSkillBox, TPGoalBox, TPProposition
*/

namespace timepath4unity
{

    [System.Serializable]
    public class TPInternalCurve : GUIDraggableObject
    {

        static TPInternalCurve CreateCurve(TPNode from, TPNode to, int type, TPProposition p)
        {
            TPInternalCurve c = ScriptableObject.CreateInstance<TPInternalCurve>();

            c.m_Position = new Vector2(from.r.center.x, from.r.center.y);
            c.predecessor = from;
            c.successor = to;
            c.type = type;

            c.rule = p;
            return c;
        }



        static TPInternalCurve CreateCurve(TPNode from, TPNode to, int type, TPResource r)
        {
            TPInternalCurve c = ScriptableObject.CreateInstance<TPInternalCurve>();
            c.m_Position = new Vector2(from.r.center.x, from.r.center.y);
            c.predecessor = from;
            c.successor = to;
            c.type = type;


            c.res = r;//! \todo this will provoke a BUG because we are implicitly saying here that rUsed will be common to all skills. It should probably go through a TPProposition, or similar

            return c;


        }

#if TRAMA
        public static TPInternalCurve CreatePlotSentenceCurve(TPNode from, TPNode to, TPPlotSentence s)
        {
            TPInternalCurve c = ScriptableObject.CreateInstance<TPInternalCurve>();
            c.m_Position = new Vector2(from.r.center.x, from.r.center.y);
            c.predecessor = from;
            c.successor = to;
            c.type = LineKind.TminTmax;
            c.s = s;
            return c;
        }
#endif


        static class LineKind
        { //more precisely: Tree Kind
            public const int notUsed = -1;
            public const int Skill2Resource = 1;
            public const int Skill2Effects = 2;
            public const int SkillWhen2Skill = 3;
            public const int GoalWhen2Goal = 4;
            public const int Goal2GoalCond = 5;
            public const int candidate = -2;

            public const int TminTmax = 6;
        }


#region Member Variables

        TPProposition rule;
        TPResource res;

#if TRAMA
        TPPlotSentence s;
#endif


        [SerializeField]
        private int type = -1;


        //[SerializeField]
        //private AnimationCurve animationCurve;

        [SerializeField]
        private TPNode predecessor;

        [SerializeField]
        private TPNode successor;

        private bool destroyMe = false;
        double lastclick = 0.0;

        public bool DestroyMe { get { return destroyMe; } }

        bool button_drag = false;
        Vector2 button_start = new Vector2(0, 0);



        Color currentColor;
#endregion

#region Properties

        public TPNode From
        {
            get
            {
                return predecessor;
            }
        }


        public TPNode To
        {
            get
            {
                return successor;
            }
            set
            {
                successor = To;
            }
        }


        public TPProposition Rule
        {
            get
            {
                return rule;
            }
        }


#endregion




        public static List<TPInternalCurve>
        turnRuleIntoTree(TPLogicExpression rule, TPNode origin, List<TPPerceptionBox> pBoxes)
        {

            int curveKind = TPInternalCurve.LineKind.notUsed;
            List<TPInternalCurve> curves = new List<TPInternalCurve>();
            foreach (TPPerceptionBase p in rule.Perceptions)
            {


                //! \todo. at runtime perceptions in pBoxes and perceptions in the rules have the same names but are not the same objects. 
                //! This makes it harder to find the things searched, and to calculate relevance values at runtime it does not work. Why does this happen?
                //!  example:
                //!  perceptionBox pB = pBoxes.Find(pBox => pBox.p.Equals(p) ); 
                //!  will work in editor mode, but not in runtime mode. Instead, 
                //!  perceptionBox pB = pBoxes.Find(pBox => pBox.p.name.Equals(p.name) ); 
                //!  does all the time. What happens in the transition. Is it something related to serializing the perceptions?


                //perceptionBox pB = pBoxes.Find(pBox => pBox.p.Equals(p) ); 
                TPPerceptionBox pB = pBoxes.Find(pBox => pBox.p.name.Equals(p.name));

                TPNode destination = null;
                if (pB == null)
                    Debug.LogError("perception not found");
                else
                {
                    //we find the destination and the curve Kind:


                    if (origin.type == TPNode.nodeType.SkillEffect)
                    {
                        curveKind = TPInternalCurve.LineKind.Skill2Effects;
                        if (pB.left != null && pB.left.type == TPNode.nodeType.SkillEffectBox)
                            destination = pB.left;
                        else if (pB.right != null && pB.right.type == TPNode.nodeType.SkillEffectBox)
                            destination = pB.right;
                    }
                    else if (origin.type == TPNode.nodeType.SkillWhen)
                    {
                        curveKind = TPInternalCurve.LineKind.SkillWhen2Skill;
                        if (pB.left != null && pB.left.type == TPNode.nodeType.SkillWhenBox)
                            destination = pB.left;
                        else if (pB.right != null && pB.right.type == TPNode.nodeType.SkillWhenBox)
                            destination = pB.right;
                    }
                    else if (origin.type == TPNode.nodeType.GoalWhen)
                    {
                        curveKind = TPInternalCurve.LineKind.GoalWhen2Goal;
                        if (pB.left != null && pB.left.type == TPNode.nodeType.GoalWhenBox)
                            destination = pB.left;
                        else if (pB.right != null && pB.right.type == TPNode.nodeType.GoalWhenBox)
                            destination = pB.right;
                    }
                    else if (origin.type == TPNode.nodeType.GoalCond)
                    {
                        curveKind = TPInternalCurve.LineKind.Goal2GoalCond;
                        if (pB.left != null && pB.left.type == TPNode.nodeType.GoalCondBox)
                            destination = pB.left;
                        else if (pB.right != null && pB.right.type == TPNode.nodeType.GoalCondBox)
                            destination = pB.right;
                    }
                }
                if (destination == null)
                    Debug.LogError("destination node not matched to apropriate destination type");
                else
                {
                    TPInternalCurve c = TPInternalCurve.CreateCurve(origin, destination, curveKind, rule.getProposition(p));
                    curves.Add(c);
                }

            }
            return curves;
        }



        public static bool removeCurveFromRuleAndTree(TPInternalCurve c, List<TPInternalCurve> curves, TPLogicExpression rule)
        {
            //bool b=	 rule.remove(c.rule.P);
            bool b = rule.remove(c.rule);
            b = b && curves.Remove(c);

            return b;

        }


        public static List<TPInternalCurve>
        buildResourcesTree(List<TPResource> resources, TPNode origin, List<TPResourceBox> rBoxes)
        {

            int curveKind = TPInternalCurve.LineKind.notUsed;
            List<TPInternalCurve> curves = new List<TPInternalCurve>();
            foreach (TPResource r in resources)
            {

                TPResourceBox rB = rBoxes.Find(rBox => rBox.res.Equals(r));


                TPNode destination = null;
                if (rB != null)
                {

                    //we find the destination and the curve Kind:
                    destination = rB.minus;
                    curveKind = TPInternalCurve.LineKind.Skill2Resource;
                    TPInternalCurve c = TPInternalCurve.CreateCurve(origin, destination, curveKind, r);
                    curves.Add(c);
                }

            }
            return curves;
        }


        public static bool removeResourceFromTree(TPInternalCurve c, List<TPInternalCurve> curves, List<TPResource> rs)
        {
            bool b = rs.Remove(c.res);
            b = b && curves.Remove(c);

            return b;

        }










        public void initCandidateCurve(TPNode from, TPNode to)
        {
            base.m_Position = new Vector2(from.r.center.x, from.r.center.y);
            this.predecessor = from;
            this.successor = to;

            this.type = TPInternalCurve.LineKind.candidate;
        }





        public void updateTo(Vector2 to)
        {
            //this.successor.r.Set(to.x,to.y,successor.r.width,successor.r.height);		
            this.successor.r.center = new Vector2(to.x, to.y);
        }




        public void drawLine()
        {
            if (this.type == TPInternalCurve.LineKind.TminTmax)
            {
                currentColor = Color.white;
                Drawing.DrawLine(new Vector2(predecessor.r.center.x, predecessor.r.center.y), new Vector2(successor.r.center.x, successor.r.center.y), currentColor, 2, true);
                GUI.Label(predecessor.r, " ", TPEditorTools.Nsty);
                GUI.Label(successor.r, " ", TPEditorTools.Nsty);
                return;
            }



            if (this.type == LineKind.candidate)
            {
                currentColor = Color.yellow;
            }
            else if (rule && !rule.IsTrue)
            {
                currentColor = Color.red;
            }
            else if (rule && rule.IsTrue)
            {
                currentColor = Color.green;
            }
            else
                currentColor = Color.white;

            string spval = "";

            if ((this.type == LineKind.Skill2Resource))
            {
                currentColor = Color.gray;
                spval = rule.ResNeeded.ToString("F1");
            }

            else if (this.type == LineKind.Skill2Effects)
            {
                if (this.rule.IsTrue)
                    currentColor = Color.grey;
                else
                    currentColor = Color.red;

                float ex = (float)rule.Ex;
                spval = ex.ToString("F2").Remove(0, 1);
                if (ex == 1.0f)
                    spval = "1";
                else if (ex == 0.0f)
                    spval = "0";
            }
            else if (this.type == LineKind.Goal2GoalCond)
            {
                if (this.rule.IsTrue)
                    currentColor = Color.grey;
                else
                    currentColor = Color.red;

                float importance = 0.0f;
                if (this.predecessor.gB != null)
                {
                    importance = this.predecessor.gB.g.I;
                }
                else
                {
                    //given the way lines are added it will probably never be used, but just in case:
                    importance = this.successor.gB.g.I;
                }

                //float ex = (float) rule.Ex;
                spval = importance.ToString("F2").Remove(0, 1);
                if (importance == 1.0f)
                    spval = "1";
                else if (importance == 0.0f)
                    spval = "0";
            }
            else if (this.rule && this.rule.IsTrue)
            {
                spval = " * ";
            }
            else
            {
                spval = "NO";
            }

            Drawing.DrawLine(new Vector2(predecessor.r.center.x, predecessor.r.center.y), new Vector2(successor.r.center.x, successor.r.center.y), currentColor, 2, true);

            if (this.type != LineKind.candidate)
                drawLabel(spval);
        }

        void drawLabel(string s)
        {

            GUIStyle tempsty = TPEditorTools.Bsty;


            Vector2 offset = new Vector2(-tempsty.normal.background.width / 2, -tempsty.normal.background.height / 2);



            Vector2 start = new Vector2(predecessor.r.center.x, predecessor.r.center.y);
            Vector2 end = new Vector2(successor.r.center.x, successor.r.center.y);


            float w = 0.35f;
            if (type == LineKind.Skill2Resource)
                w = 0.15f;

            if (start.y < end.y)
                w = w + 0.1f;

            Vector2 pos = new Vector2(w * start.x + (1 - w) * end.x + offset.x, w * start.y + (1 - w) * end.y + offset.y);


            Rect button = new Rect(pos.x, pos.y, TPEditorTools.psize.x, TPEditorTools.psize.y);

            if (type == LineKind.Skill2Effects || type == LineKind.Goal2GoalCond || type == LineKind.Skill2Resource)
            {
                tempsty.normal.textColor = tempsty.hover.textColor;

                /*
                if (Event.current.type == EventType.MouseUp   )
                {
                    button_drag = false;

                }
                else 
                
                if (Event.current.type == EventType.MouseDown && button.Contains(Event.current.mousePosition))
                {
                    button_drag = true;
                    button_start = Event.current.mousePosition;
                    Debug.Log("setting button_start: " + button_start);

                }*/
                /*
                else if (button_drag)
                {
                    float dist = (Event.current.mousePosition.y - button_start.y);

                   
                    if (Mathf.Abs(dist) > 1000.02f)
                    {
                        button_drag = false;


                    }
                    //! \todo something is WRONG about how this distance is calculated.
                    if (Mathf.Abs(dist) < 1000.02f && Mathf.Abs(dist) > 100)
                    {
                        if (type == LineKind.Skill2Effects)
                        {
                            rule.Ex = rule.Ex - dist / 10000.0f;
                            Debug.Log("dist is: " + dist + "rule.Ex: " + rule.Ex);

                        }
                        else if (type == LineKind.Goal2GoalCond)
                        {
                            if (this.predecessor.gB != null)
                            {
                                Debug.Log("dist is: " + dist + "g.I: " + this.predecessor.gB.g.I);
                                this.predecessor.gB.g.I -= dist / 10000.0f;
                            }
                            else
                            {//not used...
                                this.successor.gB.g.I -= dist / 10000.0f;
                            }
                        }
                        else if (type == LineKind.Skill2Resource)
                        {
                            if (this.res != null)
                            { //! \todo fix this, the resource needed MUST be adjustable. (long click, long click + shift)
                                rule.ResNeeded -= dist / 10000.0;
                                //res.ResNeeded -= dist/10000.0;
                            }
                        }
                    }
                }*/
            }
            else
            {
                tempsty.normal.textColor = currentColor;
            }

            if (GUI.Button(button, s, tempsty))
            {
                if (EditorApplication.timeSinceStartup - lastclick < 0.3f)
                { //it is a double click
                    destroyMe = true;
                }
                else
                {
                    if (type != LineKind.Skill2Resource)
                        rule.negate();
                }
                lastclick = EditorApplication.timeSinceStartup;
            }

        }



    }


}