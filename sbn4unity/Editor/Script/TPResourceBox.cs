

using UnityEngine;
using UnityEditor;

using System.Collections;
using System.Collections.Generic;


namespace timepath4unity
{
    public class TPResourceBox
    {
        protected Rect r;
        public TPResource res;
     //   protected DraggableColumn col;
     //   protected TPSkillBox box;
        protected TPProposition rule;

        public TPNode minus = null;


        private bool destroyMe = false;
        //double lastclick = 0.0;
        string helptext = "The amount available is shown in the grey box.  \n Double click to delete the resource. \n to The amount expected to be used by a skill is in the line label.";

        //public bool DestroyMe { get { return destroyMe; } }
        bool button_drag = false;
        Vector2 button_start = new Vector2(0, 0);

        public Rect R
        {
            get { return r; }
        }
       
        public TPResourceBox(TPResource res, TPProposition rule, Rect rect)
        {
            this.rule = rule;
            this.res = res;
            this.r = rect;
            minus = new TPNode(new Rect(this.r.xMin -TPEditorTools.psize.x - TPEditorTools.sepx , this.r.yMin + TPEditorTools.psize.y / 2, TPEditorTools.psize.x, TPEditorTools.psize.y), TPNode.nodeType.ResBox, this);
        }


        /*
        public List<TPNode> getNodes()
        {
            List<TPNode> list = new List<TPNode>();
            if (n != null)
                list.Add(n);

            return list;
        }*/

          public void setPos(Vector2 pos){

            this.r.x = pos.x;
            this.r.y = pos.y;

        }
        public void draw()
        {
            if (this.res == null)
            {
                destroyMe = true;
            }
            else
            {
                
                if (Event.current.type == EventType.MouseUp)
                {
                    button_drag = false;
                }
                else if (Event.current.type == EventType.MouseDown && this.r.Contains(Event.current.mousePosition))
                {
                    button_drag = true;
                    button_start = Event.current.mousePosition;
                }
                else if (button_drag)
                {
                    float dist = (Event.current.mousePosition.y - button_start.y);
                    if (Mathf.Abs(dist) > 20.02f)
                    {
                        rule.ResNeeded -= dist / 10000.0;
                        //res.ResMaxAvailable -= dist / 10000.0;
                    }
                }
                

                minus.r.y = this.r.y + this.r.height / 2 - this.minus.r.height / 2;
                if (GUI.Button(this.minus.r, "  - ", TPEditorTools.Psty))
                {
                    GameObject.DestroyImmediate(this.rule.gameObject);
                    destroyMe = true;
                    //! \todo the resources have to be destroyed in the list of resources
                    
                    //GameObject.DestroyImmediate(this.res.gameObject);

                }

                //


                //the amount needed:

                string label = rule.ResNeeded.ToString("F1");
                //string label = res.ResMaxAvailable.ToString("F1");
                if (GUI.Button(this.r, new GUIContent(label, helptext), TPEditorTools.Rsty))
                {
                    /* if (EditorApplication.timeSinceStartup - lastclick < 0.3f)
                    { //it is a double click
                        //destroyMe = true;
                        //GameObject.DestroyImmediate(this.res.gameObject);
                    }
                    lastclick = EditorApplication.timeSinceStartup;
                     * */
                }

                Rect temp = this.r;
                temp.width = TPEditorTools.psize.x * 4;
                temp.x = r.x + TPEditorTools.psize.x + TPEditorTools.sepx;
               // temp.y = temp.y ;

                if (GUI.Button(temp, res.AmountAvailable.ToString("F1") + " " + res.name, TPEditorTools.Psty))
                {
                    Selection.activeGameObject = res.gameObject;
                    EditorGUIUtility.PingObject(res.gameObject);
                }

              

            }
        }

        void resourceBoxEventUpdate()
        {
            if (r.Contains(new Vector2(Input.mousePosition.x, Input.mousePosition.y)) && Event.current.type == EventType.MouseDown && Event.current.button == 0)
            {
                Selection.activeGameObject = this.res.gameObject;
            }
        }
       

    }


}