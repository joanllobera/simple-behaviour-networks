using UnityEngine;
using System.Collections;
using UnityEditor;
using System.Collections.Generic;



/*!

 \brief A box used to draw the goals in the user interface
 \date April 2014

 \sa  TPGoal, perceptionBox
*/
namespace timepath4unity
{

    public class TPGoalBox
    {
        protected Rect r;
        public TPGoal g;
        TPPersonalityBoxBase box;

        public TPNode left = null;
        public TPNode right = null;

        Vector2 nsize;
        Vector2 gsize;
        string helptext = "\n Use the yellow Node to connect it.  \n click on ' - ' to delete it. \n click on it to Edit it. ";

        private bool destroyMe = false;
        public bool DestroyMe { get { return destroyMe; } }


        //to detect double clicks:
       int delete_step = 0;
        double lastclick = 0.0;


        List<TPInternalCurve> relCondCurves = new List<TPInternalCurve>();
        List<TPInternalCurve> goalCondCurves = new List<TPInternalCurve>();



        public TPGoalBox(TPGoal g, Vector2 gpos, TPPersonalityBoxBase box)
        {
        this.box = box;
        this.g = g;
        Vector2 gsize = TPEditorTools.gsize;
        this.r = new Rect(gpos.x, gpos.y, gsize.x, gsize.y);
        buildCurves();
        }

        
        void checkDoubleClick()
        {
            //if we have a double click it means: delete me!!
            if (Event.current.type == EventType.MouseDown && this.r.Contains(Event.current.mousePosition))
            {
                if (delete_step == 0)
                    delete_step = 1;
            }
            else if (Event.current.type == EventType.MouseUp && this.r.Contains(Event.current.mousePosition))
            {
                if (delete_step == 1)
                {
                    delete_step = 0;
                    if (EditorApplication.timeSinceStartup - lastclick < 0.3f)
                    { //it is a double click
                        destroyMe = true;

                    }
                    else
                    {
                        Debug.Log("not double click");


                    }
                    lastclick = EditorApplication.timeSinceStartup;
 
                }
            
                
            }
            

        }


        
        public void buildCurves()
        {
            //the tree of curves
            nsize = TPEditorTools.nsize;
            gsize = TPEditorTools.gsize;


            if (g)
            {
                //when there are few perceptions, sometimes we have a proposition referring to an empty perception. Therefore:
                //this.g.cleanRules ();


                if (box.Columns[ColumnKind.GoalWhen].Position.x < box.Columns[ColumnKind.GoalCond].Position.x)
                {
                    left = new TPNode(new Rect(this.r.xMin - nsize.x, this.r.yMin + (gsize.y / 2 - nsize.y / 2), nsize.x, nsize.y), TPNode.nodeType.GoalWhen, this);
                    relCondCurves = TPInternalCurve.turnRuleIntoTree(g.Rcon, left, box.GoalWhenPerBoxes); //! \todo the notations "GoalWhen" and "RelCond" are both used and might bring to confusion

                    right = new TPNode(new Rect(this.r.xMax, this.r.yMin + gsize.y / 2 - nsize.y / 2, nsize.x, nsize.y), TPNode.nodeType.GoalCond, this);
                    goalCondCurves = TPInternalCurve.turnRuleIntoTree(g.Gcon, right, box.GoalCondPerBoxes);
                }
                else
                {
                    left = new TPNode(new Rect(this.r.xMin - nsize.x, this.r.yMin + (gsize.y / 2 - nsize.y / 2), nsize.x, nsize.y), TPNode.nodeType.GoalCond, this);
                    goalCondCurves = TPInternalCurve.turnRuleIntoTree(g.Gcon, left, box.GoalCondPerBoxes);

                    right = new TPNode(new Rect(this.r.xMax, this.r.yMin + (gsize.y / 2 - nsize.y / 2), nsize.x, nsize.y), TPNode.nodeType.GoalWhen, this);
                    relCondCurves = TPInternalCurve.turnRuleIntoTree(g.Rcon, right, box.GoalWhenPerBoxes);

                }

            }

        }

        //! Gives the relevance level related to the different perceptions involved
        public double getRelevance()
        { //note: trying to obtain this value from the perceptions stored in TPGoal will NOT work
            double relVal = 1.0f;
            double temp = 1.0f;
            foreach (TPInternalCurve c in relCondCurves)
            {
                if (c.To.pB == null)
                    Debug.Log("you are trying to access a null perception Box");
                else
                    temp = c.To.pB.p.getvalue();
                if (!c.Rule.IsTrue)
                    temp = 1.0 - temp;

                if (temp < relVal)//the minimum, assuming Fuzzy_min is defined within Tramanet.dll
                    relVal = temp;

            }
            return relVal;
        }

        public void setPos(Vector2 pos){

            this.r.x = pos.x;
            this.r.y = pos.y;

        }

        public void draw()
        {

            string spval = " - ";
            double rel = this.getRelevance();
            if (rel > 0.99)
                spval = "1";
            else
                spval = rel.ToString("F2").Remove(0, 1);


            string t = "Goal '" + this.g.name + "' \n" + helptext;
            

            //we draw the button:
            if (GUI.Button(this.r, new GUIContent(spval, t), TPEditorTools.Gsty))
            {
                if(delete_step ==0)
                    this.delete_step = 1;
                else if(delete_step ==1){ //meaning we already clicked it:
                    if (EditorApplication.timeSinceStartup - lastclick < 0.3f)
                    { //it is a double click
                        destroyMe = true;
                        delete_step = 0;
                    }
                }
                lastclick = EditorApplication.timeSinceStartup;
            }


            if (delete_step == 1 && EditorApplication.timeSinceStartup - lastclick > 0.3f)
            {
                delete_step = 0;
                Selection.activeGameObject = g.gameObject;
                EditorGUIUtility.PingObject(g.gameObject);

            }
           
            
            
            //! \todo allow the suppression of the goal using a TPButton
            /*if (GUI.Button(this.r, new GUIContent(spval, t), TPEditorTools.Gsty))
            {
                Selection.activeGameObject = g.gameObject;
                EditorGUIUtility.PingObject(g.gameObject);

              

            }*/



            g.name = g.getGoalName();
            //g.name = EditorGUI.TextField(this.r, g.name, TPPersonalityWindow.gsty);
            //! \todo deal with double clikc and "this.destroyMe"


            if (left != null)
            {
                left.r.x = this.r.xMin - nsize.x;
                left.r.y = this.r.yMin + (this.r.height / 2 - nsize.y / 2);
                GUI.Label(left.r, " ", TPEditorTools.Nsty);
            }

            if (right != null)
            {
                right.r.x = this.r.xMax;
                right.r.y = this.r.yMin + (this.r.height / 2 - nsize.y / 2);
                GUI.Label(right.r, " ", TPEditorTools.Nsty);
            }
            foreach (TPInternalCurve c in relCondCurves)
            {
                c.drawLine();

            }
            foreach (TPInternalCurve c in goalCondCurves)
            {
                c.drawLine();
            }


            //we check if we have to remove some curve

            TPInternalCurve curve2destroy = null;
            foreach (TPInternalCurve c in relCondCurves)
            {
                if (c.DestroyMe)
                {
                    curve2destroy = c;
                }
            }
            if (curve2destroy != null)
            {
                TPInternalCurve.removeCurveFromRuleAndTree(curve2destroy, relCondCurves, g.Rcon);
                buildCurves();
            }

            curve2destroy = null;
            foreach (TPInternalCurve c in goalCondCurves)
            {
                if (c.DestroyMe)
                {
                    curve2destroy = c;
                }
            }
            if (curve2destroy != null)
            {
                TPInternalCurve.removeCurveFromRuleAndTree(curve2destroy, goalCondCurves, g.Gcon);
                buildCurves();
            }


        }




        public List<TPNode> getNodes()
        {
            List<TPNode> list = new List<TPNode>();
            if (left != null)
                list.Add(left);
            if (right != null)
                list.Add(right);

            return list;
        }
    }

}