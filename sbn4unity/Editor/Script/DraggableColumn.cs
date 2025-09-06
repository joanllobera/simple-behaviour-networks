using UnityEngine;
using System.Collections;
namespace timepath4unity
{

    public static class ColumnKind
    {
        public const int ResList = 0;
        public const int PerList = 1;
        public const int SkillEffect = 2;
        public const int SkillWhen = 3;
        public const int GoalWhen = 4;
        public const int GoalCond = 5;

    }


    [SerializeField]
    public class DraggableColumn : GUIDraggableObject
    {

        new public string name;

        static float buttonwidth = 2.8f * 40;
        [SerializeField]
        int columnKind = -1;

        public int ColumnKind { get { return columnKind; } }






        public void init(string n, Vector2 pos, int kind)
        {
            base.setPosition(pos - new Vector2(buttonwidth / 2, 0.0f));
            name = n;
            //this.m_Position = pos - new Vector2(buttonwidth/2,0.0f);

            columnKind = kind;

        }




        public void OnGUI()
        {
            Rect drawRect = new Rect(m_Position.x, m_Position.y, buttonwidth, buttonwidth), dragRect;


            Color color = GUI.color;

            if (Dragging)
            {
                GUI.color = Color.red;
            }
            GUILayout.BeginArea(drawRect);

            GUILayout.Label(name, GUILayout.ExpandWidth(false));
            GUI.color = color;



            dragRect = GUILayoutUtility.GetLastRect();
            dragRect = new Rect(dragRect.x + m_Position.x, dragRect.y + m_Position.y, dragRect.width, dragRect.height);


            GUILayout.EndArea();

            DragHorizontal(dragRect);
        }



    }

}