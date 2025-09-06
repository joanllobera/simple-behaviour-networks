using UnityEngine;
using UnityEditor;

using System.Collections;
using System.Collections.Generic;



namespace timepath4unity{
public class TPPerceptionBox {


    

        public TPPerceptionBase p; //the perception
        protected Rect r; //the square
        protected DraggableColumn c; //column

        private bool destroyMe = false;
        //double lastclick = 0.0;
        string helptext = "\n Use the yellow Node to connect it.  \n click on ' - ' to delete it. \n click on it to Edit it. ";

        public bool DestroyMe { get { return destroyMe; } }


        public string label;

        public TPNode left = null;
        public TPNode right = null;
    

       
        public TPPerceptionBox(TPPerceptionBase per, float ypos, DraggableColumn col)
        {
            this.p = per;
            this.c = col;
      

            this.r = new Rect(col.Position.x, ypos, TPEditorTools.psize.x, TPEditorTools.psize.y);
           
            if (col.ColumnKind == ColumnKind.SkillWhen)
            {
                TPNode node = new TPNode(new Rect(this.r.xMax, this.r.yMin + TPEditorTools.nsize.y / 2, TPEditorTools.nsize.x, TPEditorTools.nsize.y), TPNode.nodeType.SkillWhenBox, this);
                
                right = node;
                
            }
            else if (col.ColumnKind == ColumnKind.SkillEffect)
            {
                TPNode node = new TPNode(new Rect(this.r.xMax, this.r.yMin + TPEditorTools.nsize.y / 2, TPEditorTools.nsize.x, TPEditorTools.nsize.y), TPNode.nodeType.SkillEffectBox, this);
               
                {
                    left = node;
                }
            }
            else if (col.ColumnKind == ColumnKind.GoalWhen)
            {
                TPNode node = new TPNode(new Rect(this.r.xMax, this.r.yMin + TPEditorTools.nsize.y / 2, TPEditorTools.nsize.x, TPEditorTools.nsize.y), TPNode.nodeType.GoalWhenBox, this);
                
                    right = node;
               
            }
            else if (col.ColumnKind == ColumnKind.GoalCond)
            {
                TPNode node = new TPNode(new Rect(this.r.xMax, this.r.yMin + TPEditorTools.nsize.y / 2, TPEditorTools.nsize.x, TPEditorTools.nsize.y), TPNode.nodeType.GoalCondBox, this);
               
                {
                    left = node;
                }

            }
           

        }
        public TPNode[] getNodes()
        {
            List<TPNode> list = new List<TPNode>();
            if (left != null)
                list.Add(left);
            if (right != null)
                list.Add(right);

            return list.ToArray();
        }

       




        public void draw(float ppos, DraggableColumn fromCol, DraggableColumn toCol)
        {
            
            EditorGUILayout.BeginHorizontal();
            if (this.p == null)
            { //the gameObject was deleted
                Debug.LogError("the perception was destroyed before being removed from the goal and/or skill conditions  ");
                destroyMe = true;
            }
            else
            {
                string spval = p.getvalue().ToString("F2");
                if (p.getvalue() > 0.99)
                    spval = "1";
                else
                    spval = spval.Remove(0, 1);
                this.label = spval;

                this.r.x = fromCol.Position.x;
                this.r.y = ppos;

                string t = "Perception '" + this.p.name + "' \n" + helptext;
                if (GUI.Button(this.r, new GUIContent(this.label, t), TPEditorTools.Psty))
                {
                    /* if (EditorApplication.timeSinceStartup - lastclick < 0.3f)
                    { //it is a double click
                        destroyMe = true;
                    }
                    lastclick = EditorApplication.timeSinceStartup;
                    */
                    Selection.activeGameObject = p.gameObject;
                    EditorGUIUtility.PingObject(p.gameObject);
                }
                  //we draw the activation:
                    float totallength = (toCol.Position.x - (fromCol.Position.x + TPEditorTools.psize.x));
                    float activLength = totallength * (float)this.p.getvalue();
                    GUI.DrawTexture(new Rect(fromCol.Position.x + TPEditorTools.psize.x, this.r.yMin, activLength, TPEditorTools.psize.y), TPEditorTools.Psty.active.background);
                    GUI.DrawTexture(new Rect(fromCol.Position.x + TPEditorTools.psize.x + activLength, this.r.yMin, totallength - activLength, TPEditorTools.psize.y), TPEditorTools.Psty.onActive.background);             
        
         
                if (left != null)
                {
                    this.left.r.x = this.r.xMin - this.left.r.width;
                    this.left.r.y = this.r.y + this.r.height / 2 - this.left.r.height / 2;
                    GUI.Label(this.left.r, " ", TPEditorTools.Nsty);
                }
                if (right != null)
                {
                    this.right.r.x = this.r.xMax;
                    this.right.r.y = this.r.y + this.r.height / 2 - this.right.r.height / 2;
                    GUI.Label(this.right.r, " ", TPEditorTools.Nsty);
                }

               

               /*
                if (GUILayout.Button(p.name, GUILayout.Width(80)))
                {//! \todo find the look into icon
                    Selection.activeTransform = p.transform;
                    EditorGUIUtility.PingObject(p.gameObject);
                }
                if (GUILayout.Button("-", GUILayout.Width(20)))
                {
                    GameObject.DestroyImmediate(p.gameObject);
                }
               */

              

            }
            EditorGUILayout.EndHorizontal();
        }

        public void drawOnlyName(float ppos, DraggableColumn fromCol)
        {

            EditorGUILayout.BeginHorizontal();
            if (this.p == null)
            { //the gameObject was deleted
                destroyMe = true;
            }
            else
            {
                
                this.r.x = fromCol.Position.x;
                this.r.y = ppos;
                this.label = p.name;
                string t = "Perception '" + this.p.name + "' \n" + helptext;

                this.label = GUI.TextField(new Rect(this.r.x, this.r.y, TPEditorTools.plabelsize.x, this.r.height), this.label, TPEditorTools.Psty);
                p.name = this.label;
                
                //if (GUI.Button(new Rect(this.r.x, this.r.y, TPEditorTools.plabelsize.x, this.r.height), new GUIContent(this.label, t), TPEditorTools.Psty))
                //{
                    /*
                    if (EditorApplication.timeSinceStartup - lastclick < 0.3f)
                    { //it is a double click
                        destroyMe = true;
                    }
                    lastclick = EditorApplication.timeSinceStartup;
                    */
                //    Selection.activeGameObject = p.gameObject;
                //    EditorGUIUtility.PingObject(p.gameObject);
                //}
                if (GUI.Button(new Rect(this.r.x + TPEditorTools.plabelsize.x + TPEditorTools.sepx, this.r.y, 2 * TPEditorTools.psize.x, this.r.height), new GUIContent("Edit", t)))
                {
                    Selection.activeTransform = p.transform;
                    EditorGUIUtility.PingObject(p.gameObject);
                }


                if (GUI.Button(new Rect(this.r.x + TPEditorTools.plabelsize.x + 2*TPEditorTools.psize.x+ TPEditorTools.sepx, this.r.y, TPEditorTools.psize.x, this.r.height), new GUIContent("-", t)))
                //if (GUILayout.Button("-", GUILayout.Width(20)))
                {
                    this.destroyMe = true;
                    //GameObject.DestroyImmediate(p.gameObject);
                }

                if (left != null)
                {
                    this.left.r.x = this.r.xMin - this.left.r.width;
                    this.left.r.y = this.r.y + this.r.height / 2 - this.left.r.height / 2;
                    GUI.Label(this.left.r, " ", TPEditorTools.Nsty);
                }
                if (right != null)
                {
                    this.right.r.x = this.r.xMax;
                    this.right.r.y = this.r.y + this.r.height / 2 - this.right.r.height / 2;
                    GUI.Label(this.right.r, " ", TPEditorTools.Nsty);
                }

            }
            EditorGUILayout.EndHorizontal();
        }



    }




}
