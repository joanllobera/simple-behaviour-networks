using UnityEngine;
using UnityEditor;

using System.Collections;
using System.Collections.Generic;

/*!

 \brief Everything needed to draw a personality in the user interface
 \date August 2014
 

  \todo update all help texts
  
 \sa  TPPerceptionBox, TPPersonalityInspector, this
*/


namespace timepath4unity
{ 
public class TPPersonalityBoxBase {

       #region variables


        public Vector2 scrollPositionGoals = Vector2.zero;
        public Vector2 scrollPositionSkills = Vector2.zero;


        protected TPPersonality perso = null;
        public static GameObject timepathprefab = null;


        //float windowWidth = 0.0f;

        static int ypos = 0;//where stuff starts

        //float yPosLast = 0; //a tracker of the last position used to draw a button

        static public int sepx = TPEditorTools.sepx;
        static public int sepy = TPEditorTools.sepy;
        static public Vector2 psize = TPEditorTools.psize;
        static public Vector2 gsize = TPEditorTools.gsize;
        static public Vector2 ssize = TPEditorTools.ssize;

        //simple toggles
       protected  bool showResources = false;
       protected bool showSkills = true;
       protected bool showGoals = true;


        //important: do we need to rebuild what is displayed?
        bool rebuildSkillsAndGoals = false;
        bool rebuildPerceptionBoxes = false;



        public string[] getPersonalityResourceNames(){
            string[] names = new string[perso.Resources.Length];
            int i=0;
            foreach (TPResource r in perso.Resources)
            {
                names[i]= r.name;
                i++;
            }
            return names;

        }


        public void rebuildAllCurves() { rebuildSkillsAndGoals = true; }

        //to draw all the lines and curves we need:
        TPCurvesAndRules curulesGoals;
        TPCurvesAndRules curulesSkills;

        private List<DraggableColumn> columns = new List<DraggableColumn>();
        internal List<DraggableColumn> Columns { get { return columns; } }


        public TPPersonalityBoxBase(TPPersonality perso){

            this.perso = perso;
            rebuildSkillsAndGoals = true;
          
            this.curulesGoals = TPCurvesAndRules.CreateInstance<TPCurvesAndRules>();
            this.curulesGoals.personalityUpdate(perso, this);
            this.curulesSkills = TPCurvesAndRules.CreateInstance<TPCurvesAndRules>();
            this.curulesSkills.personalityUpdate(perso, this);
      

        }



        public class colposstruct
        {

            public float offset = 30.0f;

            public float GoalCond;
            public float GoalWhen;
            public float SkillWhen;
            public float SkillEffect;
            public float PerList;
            public float ResList;

            public static void initcolpos4inspector(colposstruct colposinit, Rect scale)
            {
                colposinit.ResList = scale.x + 0.2f * scale.width;
                colposinit.PerList = scale.x + scale.width - TPEditorTools.plabelsize.x - TPEditorTools.sepx;
                colposinit.SkillEffect = scale.x +  0.9f * scale.width - TPEditorTools.plabelsize.x - TPEditorTools.sepx;
                colposinit.SkillWhen = scale.x + 0.1f * scale.width;
                colposinit.GoalWhen = scale.x + 0.1f * scale.width;
                colposinit.GoalCond = scale.x + 0.9f * scale.width - TPEditorTools.plabelsize.x - TPEditorTools.sepx;

            }


        }
        [SerializeField]
        colposstruct colpos = new colposstruct();




        public List<TPPerceptionBox> SkillWhenPerBoxes = new List<TPPerceptionBox>(); //the precondition perception rectangles.
        public  List<TPPerceptionBox> SkillEffectPerBoxes = new List<TPPerceptionBox>(); //the effect perception rectangles (also used for goal conditions)
        public  List<TPPerceptionBox> GoalWhenPerBoxes = new List<TPPerceptionBox>(); //the relevance condition perception rectangles.
        public  List<TPPerceptionBox> GoalCondPerBoxes = new List<TPPerceptionBox>();

    
        [SerializeField]
        private List<TPSkillBox> sBoxes = new List<TPSkillBox>();

        public int SkillCount { get {return sBoxes.Count; } }

        [SerializeField]
        private List<TPGoalBox> gBoxes = new List<TPGoalBox>();



        public  List<TPResourceBox> rBoxes = new List<TPResourceBox>();//ressources



        public class AllNodes
        {
            public List<TPNode> SkillPerBoxes = new List<TPNode>();
            public List<TPNode> GoalPerBoxes = new List<TPNode>();
            public List<TPNode> SkillBoxes = new List<TPNode>();
            public List<TPNode> GoalBoxes = new List<TPNode>();
            public List<TPNode> ResBoxes = new List<TPNode>();
            List<TPNode> allnodes = new List<TPNode>();

            public TPNode[] getAllNodes()
            {
                allnodes.Clear();
                allnodes.AddRange(SkillPerBoxes);
                allnodes.AddRange(GoalPerBoxes);
                allnodes.AddRange(SkillBoxes);
                allnodes.AddRange(GoalBoxes);
                allnodes.AddRange(ResBoxes);
                return allnodes.ToArray();

            }
            public TPNode[] getAllGoalNodes()
             {
                allnodes.Clear();
                allnodes.AddRange(GoalPerBoxes);
                allnodes.AddRange(GoalBoxes);
                return allnodes.ToArray();

            }

            public TPNode[] getAllSkillNodes()
            {
                allnodes.Clear();
                allnodes.AddRange(SkillPerBoxes);
                allnodes.AddRange(SkillBoxes);
                allnodes.AddRange(ResBoxes);
                return allnodes.ToArray();

            }



        }
        protected AllNodes nodes = new AllNodes();
        //private List<TPNode> nodes = new List<TPNode>(); //all the nodes


    
        //for drawPerceptions:
        //int lastdrawheight = 0;
      //  private List<bool> showPerceptionCallbacks = new List<bool>();


        //to delete perceptions with double clicks:
        //private List<int> delete_step = new List<int>();
        //private List<double> lastclick = new List<double>();
        //to detect double clicks


      
        [SerializeField]
        private List<float> positionGoalPerception = new List<float>();  //where is placed each particular perception 

        [SerializeField]
        private List<float> positionSkillPerception = new List<float>(); 


        [SerializeField]
        private List<float> sheights; //how "LARGE" is that particular skill

        [SerializeField]
        private Vector2[] spos = new Vector2[0];

        [SerializeField]//! \todo it probably makes no sense to keep this array, it should be removed
        private Vector2[] gpos = new Vector2[0];

    
        //List<string> ls = new List<string>();

        #endregion





    




        #region functions to draw ebn components







        private void initcolumns(colposstruct colposinit){

            
            
            DraggableColumn d = DraggableColumn.CreateInstance<DraggableColumn>();
            d.init(" Resources: ", new Vector2(colposinit.ResList, ypos / 2), ColumnKind.ResList);

            columns.Insert(ColumnKind.ResList, d);

            d = DraggableColumn.CreateInstance<DraggableColumn>();
            d.init(" Perceptions: ", new Vector2(colposinit.PerList, ypos / 2), ColumnKind.PerList);
            columns.Insert(ColumnKind.PerList, d);


            d = DraggableColumn.CreateInstance<DraggableColumn>();
            d.init(" Effect:  ", new Vector2(colposinit.SkillEffect, ypos / 2), ColumnKind.SkillEffect);
            columns.Insert(ColumnKind.SkillEffect, d);


            d = DraggableColumn.CreateInstance<DraggableColumn>();
            d.init(" When:  ", new Vector2(colposinit.SkillWhen, ypos / 2), ColumnKind.SkillWhen);
            columns.Insert(ColumnKind.SkillWhen, d);

            d = DraggableColumn.CreateInstance<DraggableColumn>();
            d.init(" When: ", new Vector2(colpos.GoalWhen, ypos / 2), ColumnKind.GoalWhen);
            columns.Insert(ColumnKind.GoalWhen, d);

            d = DraggableColumn.CreateInstance<DraggableColumn>();
            d.init(" Goal: ", new Vector2(colpos.GoalCond, ypos / 2), ColumnKind.GoalCond);
            columns.Insert(ColumnKind.GoalCond, d);

        }



       //! when we want to draw in the MAIN GUI, we will redefine these.
      private void updateColPos4inspector()
        {


          //to get the width of the inspector:
            EditorGUILayout.BeginHorizontal();
            GUILayout.FlexibleSpace();
            EditorGUILayout.EndHorizontal();
            Rect scale = GUILayoutUtility.GetLastRect();

            scale.width = Screen.width;


         //   if (columns.Count <= 0)
         //   {
                this.colpos = new colposstruct();
                colposstruct.initcolpos4inspector(this.colpos,scale);
                initcolumns(this.colpos);

          /*  }
            else
            {
                if (scale.width != windowWidth)
                {// we are resizing

                    if (Event.current.type == EventType.Layout)
                    {
                        foreach (DraggableColumn col in columns)
                        {
                            col.setPosition(new Vector2(scale.x  + col.Position.x * scale.width / windowWidth, col.Position.y));
                        }
                    }
                    windowWidth = scale.width;
                }

            
       
            }
          */


        }


       
             void checkPerceptionsDeleted()
            {
               

                TPPerceptionBase toRemove = null;
                //we check whether we need to remove some perception (because we deleted it in the pBoxes from the GUI)

                foreach (TPPerceptionBox p in SkillWhenPerBoxes)
                {
                    if (p.DestroyMe)
                        toRemove = p.p;
                }
                foreach (TPPerceptionBox p in SkillEffectPerBoxes)
                {
                    if (p.DestroyMe)
                        toRemove = p.p;
                }
                foreach (TPPerceptionBox p in GoalWhenPerBoxes)
                {
                    if (p.DestroyMe)
                        toRemove = p.p;
                }
                foreach (TPPerceptionBox p in GoalCondPerBoxes)
                {
                    if (p.DestroyMe)
                        toRemove = p.p;
                }

                if (toRemove != null)
                {
                    TPPerceptionBox pB = SkillWhenPerBoxes.Find(pBox => pBox.p.Equals(toRemove));
                    if (pB.right != null)
                        nodes.SkillPerBoxes.Remove(pB.right);
                    if (pB.left != null)
                        nodes.SkillPerBoxes.Remove(pB.left);
                    pB = SkillEffectPerBoxes.Find(pBox => pBox.p.Equals(toRemove));
                    if (pB.right != null)
                        nodes.SkillPerBoxes.Remove(pB.right);
                    if (pB.left != null)
                        nodes.SkillPerBoxes.Remove(pB.left);
                    pB = GoalCondPerBoxes.Find(pBox => pBox.p.Equals(toRemove));
                    if (pB.right != null)
                        nodes.GoalPerBoxes.Remove(pB.right);
                    if (pB.left != null)
                        nodes.GoalPerBoxes.Remove(pB.left);
                    pB = GoalWhenPerBoxes.Find(pBox => pBox.p.Equals(toRemove));
                    if (pB.right != null)
                        nodes.GoalPerBoxes.Remove(pB.right);
                    if (pB.left != null)
                        nodes.GoalPerBoxes.Remove(pB.left);


                    TPPerceptionBase.removePerception(perso, toRemove);
                    if (toRemove != null)
                        GameObject.DestroyImmediate(toRemove.gameObject);

                    rebuildSkillsAndGoals = true;
                }

            }


  
            //! \todo draw better compared to the most important perceptions (something like a sortGoals from gCon)
            private void drawGoals()
            {


                int i = 0;

                float start = colpos.GoalWhen + 2* psize.x + sepx;
                float end = colpos.GoalCond  - psize.x - sepx - gsize.x;
                float xpos = start + (end - start) * 0.5f;
                gpos = new Vector2[perso.Goals.Length];
                float ypos2 = ypos;


      
                //should we remove some goal in the personality?
                foreach (TPGoalBox g in gBoxes)
                {
                    if (g.DestroyMe || g.g == null)
                    { //we have pressed the remove button or the Action gameObject has been destroyed
                        nodes.GoalBoxes.Remove(g.left);
                        nodes.GoalBoxes.Remove(g.right);
                        perso.removeGoal(g.g);
                    }
                }


                //should we update the boxes because the number of goals has changed?
                if (gBoxes.Count != perso.Goals.Length || rebuildPerceptionBoxes)
                    rebuildSkillsAndGoals = true;
                else
                {
                    foreach (TPGoalBox g in gBoxes)
                    {

                        //in order the position represents the relevance:
                        if (EditorApplication.isPlayingOrWillChangePlaymode)
                        {
                            xpos = start + (end - start) * (float)g.getRelevance();
                        }

                        if (i < perso.Perceptions.Length)
                        {
                            //where should the goal be drawn (relative to the perceptions):
                            float yposgoal = psize.y + sepy / 2 - gsize.y / 2;

                            gpos[i] = new Vector2(xpos, ypos2 + yposgoal - gsize.y / 2);//! 
                            ypos2 = ypos2 + 2f * sepy;
                        }
                        else
                        {
                            gpos[i] = new Vector2(xpos, ypos2 + sepy - gsize.y / 2);//! \todo recheck this part HERE HERE
                            ypos2 = ypos2 + psize.y + sepy;

                        }
                        g.setPos(gpos[i]);
                        i++;

                    }


                    foreach (TPGoalBox g in gBoxes)
                    {
                        g.draw();
                    }
                }



            }
       

        
             void rebuildSkillPerBoxes()
            {
                int i = 0;
                SkillWhenPerBoxes.Clear();
                SkillEffectPerBoxes.Clear();

                nodes.SkillPerBoxes.Clear();
                    
                foreach (TPPerceptionBase p in perso.Perceptions)
                {
                    TPPerceptionBox pB = new TPPerceptionBox(p, 0.0f, columns[ColumnKind.SkillWhen]);
                    SkillWhenPerBoxes.Add(pB);

                    nodes.SkillPerBoxes.AddRange(pB.getNodes());

                    TPPerceptionBox pB2 = new TPPerceptionBox(p, 0.0f, columns[ColumnKind.SkillEffect]);
                    SkillEffectPerBoxes.Add(pB2);

                    nodes.SkillPerBoxes.AddRange(pB2.getNodes());

                    i++;
                }

            }

             void rebuildGoalPerBoxes()
            {
                int i = 0;
                GoalWhenPerBoxes.Clear();
                GoalCondPerBoxes.Clear();
                nodes.GoalPerBoxes.Clear();
                foreach (TPPerceptionBase p in perso.Perceptions)
                {
                    TPPerceptionBox pB = new TPPerceptionBox(p, 0.0f, columns[ColumnKind.GoalWhen]);
                    GoalWhenPerBoxes.Add(pB);

                    nodes.GoalPerBoxes.AddRange(pB.getNodes());

                    TPPerceptionBox pB2 = new TPPerceptionBox(p, 0.0f, columns[ColumnKind.GoalCond]);
                    GoalCondPerBoxes.Add(pB2);
                    nodes.GoalPerBoxes.AddRange(pB2.getNodes());
                    i++;
                }
            }

             void rebuildGoalBoxes()
            {

                int i = 0;

                gBoxes.Clear();
                nodes.GoalBoxes.Clear();

                if (gpos.Length != perso.Goals.Length)
                    gpos = new Vector2[perso.Goals.Length];
                foreach (TPGoal g in perso.Goals)
                {

                    TPGoalBox gB = new TPGoalBox(g,gpos[i], this);
                    gBoxes.Add(gB);
                    nodes.GoalBoxes.AddRange(gB.getNodes());
                 //   perceptionID++;

                }


            }

        void rebuildSkillBoxes()
            {


                int i = 0;

                sBoxes.Clear();
                nodes.SkillBoxes.Clear();

                if (spos.Length != perso.Skills.Length)
                    spos = new Vector2[perso.Skills.Length];
                foreach (TPSkill s in perso.Skills)
                {

                    TPSkillBox sB = new TPSkillBox(s,spos[i], this);
                    sBoxes.Add(sB);
                    nodes.SkillBoxes.AddRange(sB.getNodes());
                    i++;

                }


            }
        


        //! todo : draw better compared to the most important perceptions (something like a sortSkills from eff. )
        private void drawSkills()
        {

         


            if (sheights == null || sheights.Count != perso.Skills.Length)
                sheights = new List<float>(new float[perso.Skills.Length]);
            //we check about our foldouts:
            int i = 0;
            float start = colpos.SkillWhen + 2*psize.x + sepx;
            float end = colpos.SkillEffect - psize.x - ssize.x - sepx;
            float ypos2 = ypos;
            spos = new Vector2[perso.Skills.Length];




            //should we remove some skill in the personality?
            foreach (TPSkillBox s in sBoxes)
            {
                if (s.s == null || s.DestroyMe || s.s.Action == null)
                { //we have pressed the remove button or the Action gameObject has been destroyed
                    nodes.SkillBoxes.Remove(s.left);
                    nodes.SkillBoxes.Remove(s.right);
                    TPSkill.removeSkill(s.s);
                }
            }

            //should we update the boxes because the number of skills has changed?
            if (sBoxes.Count != perso.Skills.Length || rebuildPerceptionBoxes)
            {
                rebuildSkillsAndGoals = true;
            }
            else
            {
                float maxactivation = 0.0f;
                foreach (TPSkillBox s in sBoxes)
                {
                   if(s.s.Action.Activation > maxactivation)
                    {
                        maxactivation = (float) s.s.Action.Activation;
                    }

                }


                    foreach (TPSkillBox s in sBoxes)
                {
                    //the more to the right, the more active is the skill:
                    float xpos = start + (end - start) / 2;
                    if (EditorApplication.isPlayingOrWillChangePlaymode)
                    {
                        /*
                        if ((float)s.s.Action.Decisiveness <= 0)
                        {
                            xpos = start;
                        }
                        else
                        {
                            xpos = start + (end - start) * ((float)s.s.Action.Decisiveness)/2.0f;
                        }*/

                        if ( ((float)s.s.Action.Activation <= 0 ) || ( maxactivation <= 0) )
                        {
                            xpos = start;
                        }
                        else
                        {
                            xpos = start + (end - start) * ((float)s.s.Action.Activation) / (2.0f * maxactivation);
                        }


                    }

                    //where to draw the skill relative to the previous skill
                    spos[i] = new Vector2(xpos, ypos2 + TPEditorTools.ssize.y / 2);
                    s.setPos(spos[i]);

                  /*  if (perceptionID < perso.Perceptions.Length)
                    {
                        ypos2 = ypos2 + s.checkSkillHeight();

                    }
                    else
                    {
                      }*/
                    ypos2 = ypos2 + s.checkSkillHeight();
                    
                    i++;
                }


                foreach (TPSkillBox s in sBoxes)
                {
                    if (s.s)
                        s.draw();
                }
            }
        }




        protected void DrawAddGoalButtons()
        {
            float yPosLastGoal = ypos;

            //the space taken by the perceptions:
            if (positionGoalPerception.Count > 0)
            {
                yPosLastGoal = Mathf.Max(yPosLastGoal, positionGoalPerception[positionGoalPerception.Count - 1] + 2 * sepy);
            }

            //the space taken by the goals:
            if (gpos.Length > 0)
                yPosLastGoal = Mathf.Max(yPosLastGoal, gpos[gpos.Length - 1].y + gsize.y + sepy);


            //button to add a new goal	

            float xGoal = Screen.width/2 - gsize.x / 2;
            Rect goalArea = new Rect(xGoal, yPosLastGoal, gsize.x, gsize.y);
            if (GUI.Button(goalArea, " + ", TPEditorTools.Gsty))
            {
                scrollPositionGoals = new Vector2(0.0f, yPosLastGoal);
                perso.AddGoal();
                rebuildSkillsAndGoals = true;
            }



           

        }

        protected Rect[] getAddPerceptionInGoalButtons(){
             
            float[] xNewPer = new float[2] { this.Columns[ColumnKind.GoalWhen].Position.x, this.Columns[ColumnKind.GoalCond].Position.x + TPEditorTools.plabelsize.x  };
            float yPosLastGoal = GetHeightGoals();



            Rect[] rects = new Rect[2];

            rects[0] = new Rect(xNewPer[0], yPosLastGoal, psize.x, psize.y);
            rects[1] = new Rect(xNewPer[1], yPosLastGoal, psize.x, psize.y);
            return rects;


        }


        protected Rect[] getAddPerceptionInSkillButtons()
        {
            //buttons to add a new perception
            float[] xNewPer = new float[2] { this.Columns[ColumnKind.SkillWhen].Position.x, this.Columns[ColumnKind.SkillEffect].Position.x + TPEditorTools.plabelsize.x  };

            float yPosLastSkill = GetHeightSkills();
                

            Rect[] rects = new Rect[2];

            rects[0] = new Rect(xNewPer[0], yPosLastSkill, psize.x, psize.y);
            rects[1] = new Rect(xNewPer[1], yPosLastSkill, psize.x, psize.y);
            return rects;


        }

        public Rect getAddSkillButton()
        {
            float yPosLastSkill = ypos;

            //the space taken by the perceptions:
            if (positionSkillPerception.Count > 0)
            {
                yPosLastSkill = Mathf.Max(yPosLastSkill, positionSkillPerception[positionSkillPerception.Count - 1] + 2 * sepy);
            }

            //the space taken by the skills:
            float skillHeight = 0f;
            if (sBoxes.Count > 0 && sBoxes[sBoxes.Count - 1] != null)
                skillHeight = sBoxes[sBoxes.Count - 1].checkSkillHeight();
            if (spos.Length > 0)
            {
                yPosLastSkill = Mathf.Max(yPosLastSkill, spos[spos.Length - 1].y + skillHeight);
            }


            //button to add a new skill	
            float xSkill = (this.Columns[ColumnKind.SkillWhen].Position.x + this.Columns[ColumnKind.SkillEffect].Position.x) / 2 + psize.x / 2 - ssize.x / 2;
            Rect skillArea = new Rect(xSkill, yPosLastSkill, ssize.x, ssize.y);
            return skillArea;
             

        }

        public void DrawAddResource2Skill(Rect r, string[] a, TPSkill skill  ){

            string[] b = this.getPersonalityResourceNames();

            List<string> c = new List<string>();
            foreach (string s in b)
            {
                List<string> temp = new List<string>(a);
                if (temp.Find(str => str.Equals(s)) == null) //if we are not using it already
                {
                    c.Add(s);
                }

            }
            int selectedIndex = c.Count;
            c.Add(TPEditorTools.noneString);
            c.Add(" + New Resource");



            int tempIndex = EditorGUI.Popup(new Rect(r.x + 2*( r.width + TPEditorTools.sepx),r.y,TPEditorTools.plabelsize.x,TPEditorTools.plabelsize.y), selectedIndex, c.ToArray());
            if (tempIndex != selectedIndex)
            {
                if (tempIndex == c.Count - 1)
                {
                    GameObject go = perso.AddResourceByName("Resource" + perso.Resources.Length).gameObject;
                    skill.Resources.addResource(go.name, 1.0f);
                    //Selection.activeGameObject = go;
                    EditorGUIUtility.PingObject(go);
                }
                else
                {
                    skill.Resources.addResource(c[tempIndex], 1.0f);
                }
                
            }


        }


        #endregion


        private void drawPerceptionsInGoals()
        {
            GoalWhenPerBoxes.Remove(null);
            GoalCondPerBoxes.Remove(null);
            if (positionGoalPerception.Count != perso.Perceptions.Length)
            {
                positionGoalPerception = new List<float>(new float[perso.Perceptions.Length]);
            }
          

            //we first draw the callbacks part, which gives us the height of each perception.
            //if(lastdrawheight == 0){
            int lastdrawheight = 60 * perso.Perceptions.Length;
            int i = 0;


            //we find the distance to the next column:
            //float distance = Screen.width;
         
            lastdrawheight = 0;
            if (positionGoalPerception.Count > 0)
                positionGoalPerception[0] = ypos;

 

           
            foreach (TPPerceptionBase p in perso.Perceptions)
            {

                lastdrawheight += (int)2f * sepy;
                i++;
                if (i < perso.Perceptions.Length)
                    positionGoalPerception[i] = ypos + lastdrawheight;
            }

            i = 0;
          
            foreach (TPPerceptionBox p in GoalWhenPerBoxes)
            {
                p.draw(positionGoalPerception[i], columns[ColumnKind.GoalWhen],columns[ColumnKind.GoalCond]);
                i++;
            }
            i = 0;
          
            foreach (TPPerceptionBox p in GoalCondPerBoxes)
            {
                p.drawOnlyName(positionGoalPerception[i], columns[ColumnKind.GoalCond]);
                i++;
            }

        }



        private void drawPerceptionsInSkills()
        {

            SkillWhenPerBoxes.Remove(null);
            SkillEffectPerBoxes.Remove(null);

            if (positionSkillPerception.Count != perso.Perceptions.Length)
            {
                positionSkillPerception = new List<float>(new float[perso.Perceptions.Length]);
            }
          
            int i = 0;

            //we find the distance to the next column:
            //float distance = Screen.width;

            float lastdrawheight = 0;
            if (positionSkillPerception.Count > 0)
                positionSkillPerception[0] = ypos;


            foreach (TPPerceptionBase p in perso.Perceptions)
            {

               
                if (i < this.sBoxes.Count)
                {
                    lastdrawheight += sBoxes[i].checkSkillHeight();
                }
                else
                {
                    lastdrawheight += 2f * sepy ;

                }

                i++;
                if (i < perso.Perceptions.Length)
                    positionSkillPerception[i] = ypos + lastdrawheight;
            }

            i = 0;
            foreach (TPPerceptionBox p in SkillWhenPerBoxes)
            {
                p.draw(positionSkillPerception[i], columns[ColumnKind.SkillWhen], columns[ColumnKind.SkillEffect]);
                i++;
            }
            i = 0;
            foreach (TPPerceptionBox p in this.SkillEffectPerBoxes)
            {
                p.drawOnlyName(positionSkillPerception[i], columns[ColumnKind.SkillEffect]);
                i++;
            }

        }


        //! it verifies if any object was deleted.
        protected void  PrepareOnInspectorGUI()
        {

           

            //we load the prefab:
            if (!timepathprefab)
            {
                // Try to grab our prefab.
                string name = "TimepathPrefab";
                timepathprefab = Resources.Load(name, typeof(GameObject)) as GameObject;
                if (!timepathprefab)
                {
                    Debug.Log("The prefab " + name + " does not seem to be in the Timepath Resources folder");
                    return;
                }
            }
            //we deal with the columns:
            updateColPos4inspector();
          

            Event evt = Event.current;
            checkPerceptionsDeleted();

            //if a perception was added or removed, we also need to:
            if ((SkillWhenPerBoxes.Count != perso.Perceptions.Length) || rebuildPerceptionBoxes || rebuildSkillsAndGoals)
            {
                rebuildSkillPerBoxes();
                rebuildGoalPerBoxes();
            }

            if (rebuildSkillsAndGoals && evt.type == EventType.Layout)
            {
                rebuildSkillsAndGoals = false;
                this.rebuildSkillBoxes();//since we changed the layout of curves, we need to rebuild them
                this.rebuildGoalBoxes();
            }


        }


     




    /*
        protected void UpdatePrepareOnInspectorGUI()
        {

            Event evt = Event.current;
            checkPerceptionsDeleted();

            //if a perception was added or removed, we also need to:
            if ((SkillWhenPerBoxes.Count != perso.Perceptions.Length) || rebuildPerceptionBoxes || rebuildSkillsAndGoals)
            {
                rebuildSkillPerBoxes();
            }

            if (rebuildSkillsAndGoals && evt.type == EventType.Layout)
            {
                this.rebuildSkillBoxes();//since we changed the layout of curves, we need to rebuild them
            }


        }
    */

         protected void DrawGoals(){   
            

                drawPerceptionsInGoals();
                drawGoals();
                
                
                this.DrawAddGoalButtons();

                TPNode[] temp = nodes.getAllGoalNodes();
                curulesGoals.checkCurveCandidate(temp);
    

            }

         protected float GetHeightGoals()
         {

             float yPosLastGoal = 0;
             //the space taken by the perceptions:
             if (positionGoalPerception.Count > 0)
             {
                 yPosLastGoal = Mathf.Max(yPosLastGoal, positionGoalPerception[positionGoalPerception.Count - 1] + 2 * sepy);
             }

             //the space taken by the goals:
             if (gpos.Length > 0)
                 yPosLastGoal = Mathf.Max(yPosLastGoal, gpos[gpos.Length - 1].y + gsize.y + sepy);

             return yPosLastGoal;
         }



         protected float GetHeightSkills()
         {
              float yPosLastSkill = 0.0f;
            //the space taken by the perceptions:
            if (positionSkillPerception.Count > 0)
            {

                yPosLastSkill = Mathf.Max(yPosLastSkill, positionSkillPerception[positionSkillPerception.Count - 1] + 2 * sepy);
   
            }

            //the space taken by the skills:
            if (spos.Length > 0)
            {

                float skillHeight = 0f;
                if (sBoxes.Count > 0 && sBoxes[sBoxes.Count - 1] != null)
                    skillHeight = sBoxes[sBoxes.Count - 1].checkSkillHeight();
            
                yPosLastSkill = Mathf.Max(yPosLastSkill,spos[spos.Length - 1].y + skillHeight);
            }

             return yPosLastSkill;
         }

    protected void DrawSkills(){

         
               

                drawPerceptionsInSkills();

                drawSkills();



                TPNode[] temp = nodes.getAllSkillNodes();
                curulesSkills.checkCurveCandidate(temp);





        }

    protected void DrawTooltip()
    {


            //THIS DOES NOT WORK:
            Rect rtooltip = new Rect(10.0f + ssize.x + sepx, 0.8f * Screen.height + ssize.y, Screen.width / 4, 4 * ssize.y);
            GUI.TextArea(rtooltip, GUI.tooltip);





    }


        }


 

    }





