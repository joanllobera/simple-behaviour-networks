using UnityEngine;
using System.Collections;

namespace timepath4unity
{

    public class GUIDraggableObject : ScriptableObject
    {
        [SerializeField]
        protected Vector2 m_Position;
        private Vector2 m_DragStart;
        private bool m_Dragging;

        public GUIDraggableObject()
        {
        }
        public void setPosition(Vector2 position)
        {
            m_Position = position;
        }



        public GUIDraggableObject(Vector2 position)
        {
            m_Position = position;
        }

        public bool Dragging
        {
            get
            {
                return m_Dragging;
            }
        }

        public Vector2 Position
        {
            get
            {
                return m_Position;
            }

            set
            {
                m_Position = value;
            }
        }

        public void Drag(Rect draggingRect)
        {
            if (Event.current.type == EventType.MouseUp)
            {

                m_Dragging = false;
            }
            else if (Event.current.type == EventType.MouseDown && draggingRect.Contains(Event.current.mousePosition))
            {
                m_Dragging = true;
                m_DragStart = Event.current.mousePosition - m_Position;
                Event.current.Use();
            }

            if (m_Dragging)
            {
                m_Position = Event.current.mousePosition - m_DragStart;
            }
        }

        public void DragHorizontal(Rect draggingRect)//! \todo there seems to be some bug in this function, since it does vertical draggings...
        {
            if (Event.current.type == EventType.MouseUp)
            {
                m_Dragging = false;
            }
            else if (Event.current.type == EventType.MouseDown && draggingRect.Contains(Event.current.mousePosition))
            {
                m_Dragging = true;
                m_DragStart = Event.current.mousePosition - m_Position;
                Event.current.Use();
            }

            if (m_Dragging)
            {
                float ypos = m_Position[1];
                m_Position = Event.current.mousePosition - m_DragStart;
                m_Position[1] = ypos;
            }
        }





    }


}