using UnityEngine;
using UnityEditor;
using System.Collections;


using timepath4unity; 
public class TPPersonalityBox : TPPersonalityBoxBase {

  

    public TPPersonalityBox(TPPersonality perso): base(perso){
        
    }


    public void DEBUGMousePos()
    {
        Vector2 mousePos = Event.current.mousePosition;
        string m = "x: " + mousePos.x.ToString() + " y: " + mousePos.y.ToString();
        GUI.Label(new Rect(mousePos.x, mousePos.y, 120, 40), m);

    }

    public void OnInspectorGUI()
    {
        //FOR DEBUG:

        /*
        EditorGUILayout.BeginHorizontal();
        GUILayout.Label("Roles using personality: " + perso.name);
        EditorGUILayout.BeginVertical();

        GUILayout.Label("TO DO DEBUG");
        //! \todo debug display of personalities
        EditorGUILayout.EndVertical();
        EditorGUILayout.EndHorizontal();
        */
        //DEBUGMousePos();

        PrepareOnInspectorGUI();

      


        showGoals = EditorGUILayout.Foldout(showGoals, "Goals: ");
        if (showGoals)
        {
            drawGoalsInBoundingBox(400);
          
      
        }
     
        GUILayout.Space(10);
     
    
        showSkills = EditorGUILayout.Foldout(showSkills, "Skills: ");

        if (showSkills)
        {
           
            drawSkillsInBoundingBox(400);

        }
    }

    public void OnInspectorGUISkills()
    {

        GUILayout.Space(50);

        PrepareOnInspectorGUI();

        drawSkillsInBoundingBox(400);
       


    }
    public void OnInspectorGUIGoals()
    {

        GUILayout.Space(50);

        PrepareOnInspectorGUI();

        drawGoalsInBoundingBox(400);



    }



    private void drawSkillsInBoundingBox(float height){

        float heightAllSkills = GetHeightSkills() + 4 * psize.y;
        float boundingBox = Mathf.Min(heightAllSkills, height);

        scrollPositionSkills = EditorGUILayout.BeginScrollView(scrollPositionSkills, GUILayout.Height(boundingBox));
        EditorGUILayout.BeginVertical(GUILayout.Height(heightAllSkills));

        DrawSkills();

        Rect[] rects = this.getAddPerceptionInSkillButtons();
        foreach (Rect prect in rects)
        {
            if (GUI.Button(prect, " + ", TPEditorTools.Psty))
            {
                //scrollPosition = new Vector2(0.0f, yPosLast);
                string name = "Perception" + SkillWhenPerBoxes.Count;
                TPCallbackTools.AddPerceptionByName(perso, name);
                rebuildAllCurves();
            }
        }

        Rect skillArea = this.getAddSkillButton();
        if (GUI.Button(skillArea, " + ", TPEditorTools.Ssty))
        {
            string name = "Skill" + this.SkillCount;
            TPCallbackTools.AddSkillByName(perso, name);
        }
        EditorGUILayout.EndVertical();
        EditorGUILayout.EndScrollView();



    }
    protected void drawGoalsInBoundingBox(float height)
    {

        float heightAllGoals = GetHeightGoals() + 4 * psize.y;
        float boundingBox = Mathf.Min(heightAllGoals, height);

        scrollPositionGoals = EditorGUILayout.BeginScrollView(scrollPositionGoals, GUILayout.Height(boundingBox));
        EditorGUILayout.BeginVertical(GUILayout.Height(heightAllGoals));

        DrawGoals();

        Rect[] rects = getAddPerceptionInGoalButtons();
        foreach (Rect prect in rects)
        {
            if (GUI.Button(prect, " + ", TPEditorTools.Psty))
            {
                string name = "Perception" + SkillWhenPerBoxes.Count;
                TPCallbackTools.AddPerceptionByName(perso, name);
                rebuildAllCurves();
            }
        }
        EditorGUILayout.EndVertical();
        EditorGUILayout.EndScrollView();


    }


}



