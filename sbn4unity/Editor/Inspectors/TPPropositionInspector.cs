
using UnityEditor;

namespace timepath4unity { 

[UnityEditor.CustomEditor(typeof(TPProposition))]
public class TPPropositionInspector : Editor
{

    
    public override void OnInspectorGUI()
    {
            TPProposition p = (TPProposition)target;
            
            if (p.HasRes)
            {
                p.ResNeeded= EditorGUILayout.DoubleField(p.ResName + " used:",p.ResNeeded);
                p.setName();
            }
            else if(p.HasEffect)
            {
                p.Ex = EditorGUILayout.DoubleField("effect likeliness:", p.Ex);
            }

    }

}

} 