using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;



namespace timepath4unity
{

    public class TPSkillBox
    {
        private Rect r;
        private Rect addResourceRect;

        TPPersonalityBoxBase box;

        public TPSkill s;
   
        public TPNode left = null;
        public TPNode right = null;

        List<TPInternalCurve> preconCurves = new List<TPInternalCurve>();
        List<TPInternalCurve> effectCurves = new List<TPInternalCurve>();

        public TPNode middle = null;
        List<TPInternalCurve> resCurves = new List<TPInternalCurve>();

        List<TPResourceBox> rBoxes = new List<TPResourceBox>();


        [SerializeField]
        bool showActionCallback = false;
        public bool ShowActionCallback { get { return showActionCallback; } }

        Vector2 nsize;
        Vector2 ssize;

        private bool destroyMe = false;
        public bool DestroyMe { get { return destroyMe; } }


        //to detect double clicks:
        int delete_step = 0;
        double lastclick = 0.0;
        string helptext = "\n click on the little triangle to Edit. \n double click to delete \n connect it using the yellow nodes";


        public TPSkillBox(TPSkill s,Vector2 spos, TPPersonalityBoxBase box)
        {
            this.box = box;
            this.s = s;
            Vector2 ssize = TPEditorTools.ssize;
            if (spos != null)
                this.r = new Rect(spos.x, spos.y, ssize.x, ssize.y);
            if (s != null)
                buildCurves();


        }


        public void buildCurves()
        {
            //the tree of curves
            nsize = TPEditorTools.nsize;
            ssize = TPEditorTools.ssize;

            //this.s.cleanRules();

            preconCurves.Clear();
            effectCurves.Clear();

            if (box.Columns[ColumnKind.SkillWhen].Position.x < box.Columns[ColumnKind.SkillEffect].Position.x)
            {
                left = new TPNode(new Rect(this.r.xMin - nsize.x, this.r.yMin + (ssize.y / 2 - nsize.y / 2), nsize.x, nsize.y), TPNode.nodeType.SkillWhen, this);
                preconCurves = TPInternalCurve.turnRuleIntoTree(s.Precons, left, box.SkillWhenPerBoxes);

                right = new TPNode(new Rect(this.r.xMax, this.r.yMin + (ssize.y / 2 - nsize.y / 2), nsize.x, nsize.y), TPNode.nodeType.SkillEffect, this);
                effectCurves = TPInternalCurve.turnRuleIntoTree(s.Effects, right, box.SkillEffectPerBoxes);
            }
            else
            {
                left = new TPNode(new Rect(this.r.xMin - nsize.x, this.r.yMin + (ssize.y / 2 - nsize.y / 2), nsize.x, nsize.y), TPNode.nodeType.SkillEffect, this);
                effectCurves = TPInternalCurve.turnRuleIntoTree(s.Effects, left, box.SkillEffectPerBoxes);

                right = new TPNode(new Rect(this.r.xMax, this.r.yMin + (ssize.y / 2 - nsize.y / 2), nsize.x, nsize.y), TPNode.nodeType.SkillWhen, this);
                preconCurves = TPInternalCurve.turnRuleIntoTree(s.Precons, right, box.SkillWhenPerBoxes);

            }


            middle = new TPNode(new Rect(this.r.center.x - nsize.x / 2, this.r.yMax, nsize.x, nsize.y), TPNode.nodeType.SkillRes, this);
            
            

        }

        public float checkSkillHeight()
        {
            //float height = 2 * TPEditorTools.psize.y + this.s.Resources.Resources.Length * TPEditorTools.psize.y;
            return this.addResourceRect.yMax - this.r.yMin + TPEditorTools.sepy;
        }

        public void setPos(Vector2 pos)
        {

            this.r.x = pos.x;
            this.r.y = pos.y;

        }



        public void draw()
        {

            //drawSkillCallback();



            drawSkillLabel();

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
            foreach (TPInternalCurve c in preconCurves)
            {
                c.drawLine();

            }
            foreach (TPInternalCurve c in effectCurves)
            {
                c.drawLine();
            }
            drawResourceLinks();

            //we check if we have to remove some curve

            TPInternalCurve curve2destroy = null;
            foreach (TPInternalCurve c in preconCurves)
            {
                if (c.DestroyMe)
                {
                    curve2destroy = c;
                }
            }
            if (curve2destroy != null)
            {
                TPInternalCurve.removeCurveFromRuleAndTree(curve2destroy, preconCurves, s.Precons);
                buildCurves();
            }

            curve2destroy = null;
            foreach (TPInternalCurve c in effectCurves)
            {
                if (c.DestroyMe)
                {
                    curve2destroy = c;
                }
            }
            if (curve2destroy != null)
            {
                TPInternalCurve.removeCurveFromRuleAndTree(curve2destroy, effectCurves, s.Effects);
                buildCurves();
            }

        }

        string[] getSkillResourceNames()
        {

            TPResource[] ress = this.s.Resources.Resources;
            string[] names = new string[ress.Length];
            for(int i=0; i < ress.Length; i++){
                names[i] = ress[i].name;

            }
            return names;
        }

        public void drawResourceLinks()
        {


             middle.r.x = this.r.center.x - nsize.x / 2;
             middle.r.y = this.r.yMax;

            //! \todo instead of redrawing always, find the conditions to redraw. otherwise, the amount used is not editable by dragging 

          //  if (rBoxes.Count != s.Resources.Count  || s.Resources.Count ==0 )
          //  {
                  rBoxes.Clear();
                   
                   Vector2[] rpos = new Vector2[s.Resources.Count];

                    int i = 0;
                    foreach (TPResource r in s.Resources.Resources)
                    {

                        TPProposition prop = s.Resources.getProposition(r);

                        rpos[i] = new Vector2(middle.r.x, middle.r.y + i *( 2 * middle.r.height +TPEditorTools.sepy/2) );
                        TPResourceBox rB = new TPResourceBox(r,prop, new Rect(rpos[i].x, rpos[i].y, TPEditorTools.psize.x, TPEditorTools.psize.y));
                        rBoxes.Add(rB);
                        i++;
                    }

                   // addResourceRect = new Rect(this.middle.r.x - TPEditorTools.psize.x - TPEditorTools.sepx, rpos[perceptionID-1].y + ( 2 * middle.r.height +TPEditorTools.sepy/2), TPEditorTools.psize.x, TPEditorTools.psize.y);
                    addResourceRect = new Rect(this.middle.r.x - TPEditorTools.psize.x - TPEditorTools.sepx, middle.r.y + i * (2 * middle.r.height + TPEditorTools.sepy / 2), TPEditorTools.psize.x, TPEditorTools.psize.y);
                
                
           //     }


            


            foreach (TPResourceBox rBox in rBoxes)
            {
                rBox.draw();

            }



            GUI.Label(this.addResourceRect, " + ", TPEditorTools.Psty);
            
            //button to add a new resource
            
            box.DrawAddResource2Skill(addResourceRect, getSkillResourceNames(),this.s);
              
  


            /*            TPInternalCurve curve2destroy = null;
            foreach (TPInternalCurve c in resCurves)
            {
                if (c.DestroyMe)
                {
                    curve2destroy = c;
                }
            }
            if (curve2destroy != null)
            {
                Debug.Log("TODO: redo the way resources are drawn in the skill");

                //TPInternalCurve.removeResourceFromTree(curve2destroy, resCurves, s.Resources);
                buildCurves();
            }
            */
        }

        /*
        protected void drawSkillCallback(){
	
            int perceptionID = this.index;
            Vector2[] spos = TPPersonalityWindow.spos;
            Vector2 ssize = TPPersonalityWindow.ssize;
		
            Rect skillArea;
            float skillWidth = ssize.x;
            if(!ShowActionCallback){
                TPPersonalityWindow.sheights[perceptionID] = ssize.y;
                skillArea = new Rect(spos[perceptionID].x  , spos[perceptionID].y,skillWidth, ssize.y);
            }else{
                float start = spos[perceptionID].x;
                float end = Mathf.Max (TPPersonalityWindow.Columns[ColumnKind.SkillWhen].Position.x,TPPersonalityWindow.Columns[ColumnKind.SkillEffect].Position.x)  -  TPPersonalityWindow.sepx;
                if(s.Action.useHardcodedCallback){
                    TPPersonalityWindow.sheights[perceptionID] = 4.0f*ssize.y;
                    skillArea = new Rect(start  , spos[perceptionID].y,end-start, 4.0f*ssize.y);		
                }else{	
                    TPPersonalityWindow.sheights[perceptionID] = 8*ssize.y;
                    skillArea = new Rect(start  , spos[perceptionID].y,end-start, 8*ssize.y);			
                }
            }
            this.r = skillArea;
            checkDoubleClick();	

            if (s.Action != null) {  //! \todo decide whether the changes in the action should be stored when going from Play mode to editor mode. Find out how to rebuild the skill boxes in this case??
                            string t = "Skill '" + this.s.Action.name + "' \n" + helptext;
		
                            GUILayout.BeginArea (skillArea, new GUIContent ("", t), TPPersonalityWindow.ssty);
                            //we make sure the last foldout opened corresponds to the object selected in the hierarchy:
		
                            EditorGUILayout.BeginHorizontal ();
                            EditorGUILayout.BeginVertical (GUILayout.MinHeight (ssize.y + TPPersonalityWindow.sepy));
	
                            //bool lastFoldoutOpened = EditorGUILayout.Foldout (showActionCallback, s.Action.name);
                bool lastFoldoutOpened = EditorGUILayout.Foldout (showActionCallback,s.Action.name +" "+ s.Action.A.ToString("F2"));


		
		
                            //Rect r= new Rect(0.0f  ,0.0f,0.8f*ssize.x, ssize.y);
                            //bool lastFoldoutOpened = EditorGUI.Foldout(r,showActionCallback,new GUIContent(s.name,helptext));
				
		
                            if (! showActionCallback && lastFoldoutOpened) {
                                    //if it just opened, we select the gameObject in the hierarchy (needed for EditorDelegateGUI.EventFields)	
                                    Selection.activeGameObject = s.Action.gameObject;			
                                    //EditorGUIUtility.PingObject( s.Action.gameObject );
                            } else if (showActionCallback && lastFoldoutOpened) {
                                    //it was already open, but we have selected another object (to avoid a blocking situation)
                                    if (Selection.activeGameObject != s.Action.gameObject) {
                                            lastFoldoutOpened = EditorGUILayout.Foldout (false, s.Action.name);	
                                    }
                            } else if (! showActionCallback && Selection.activeGameObject == s.Action.gameObject) {
                                    //! \todo put a method to highlight the name, like the PingObject function
                            }
                            showActionCallback = lastFoldoutOpened;
                            if (showActionCallback) {	
                                    if (!s.Action.useHardcodedCallback) {
                                            EditorDelegateGUI.EventFields (s.Action as Component);	
                                            if (GUI.changed) {
                                                    EditorUtility.SetDirty (s);
                                            }
                                    }
                                    s.Action.useHardcodedCallback = GUILayout.Toggle (s.Action.useHardcodedCallback, "Defined in code");
                                    s.Action.name = EditorGUILayout.TextField ("", s.Action.name, GUILayout.MaxWidth (skillArea.width));
                                    //this.destroyMe = GUILayout.Button("delete Skill");
                            }
                            EditorGUILayout.EndHorizontal ();
                            EditorGUILayout.EndVertical ();				
                            GUILayout.EndArea ();
		
            } else {
                //this.destroyMe = true;	
            }
		

		
        }
        */

        protected void drawSkillLabel()
        {


            string label = "";

            if (s.name.Length < 8)
                label = s.name  ;
            else
                label = s.name.Substring(0, 8) ;
            string t = "Skill " + this.s.name + "' \n" + helptext;

            GUIStyle temp = TPEditorTools.SstyA;
            if (EditorApplication.isPlayingOrWillChangePlaymode)
            {
                string spval = "";
                if (s.Action.Decisiveness > 0.99)
                    spval = "1";
                else
                    spval = s.Action.Decisiveness.ToString("F2").Remove(0, 1);


                label += " " + spval;


                if (s.Action.WasExecuted)
                {
                   
                    // Debug.Log("I have painted in execution style Skill: " + s.name + "with activat: " +s.Action.Activation + "and decided: " + s.Action.Decisiveness);
                     GUI.Label(this.r, new GUIContent(label, t), TPEditorTools.SstyA);

                    if(Event.current.type.Equals( EventType.Repaint) )
                        s.Action.WasExecuted = false; //we put it back to false once we are sure it was painted.
                }
                else {
                    GUI.Label(this.r, new GUIContent(label, t), TPEditorTools.Ssty);

                }

            }
            else { //we are not in play mode

                    if (GUI.Button(this.r, new GUIContent(label, t), TPEditorTools.Ssty))
                    {
                        if (delete_step == 0)
                            this.delete_step = 1;
                        else if (delete_step == 1)
                        { //meaning we already clicked it:
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
                        Selection.activeGameObject = s.gameObject;
                        EditorGUIUtility.PingObject(s.gameObject);

                    }
            }

        }



        /*
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
                    lastclick = EditorApplication.timeSinceStartup;

                }
            }
        }*/



        public List<TPNode> getNodes()
        {
            List<TPNode> list = new List<TPNode>();
            if (left != null)
                list.Add(left);
            if (right != null)
                list.Add(right);
            if (middle != null)
                list.Add(middle);
            return list;
        }




        public void drawPreconCurves()
        {

            foreach (TPInternalCurve c in preconCurves)
            {
                c.drawLine();
            }
        }




    }

}