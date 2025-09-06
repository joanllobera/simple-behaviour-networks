using UnityEngine;
using System.Collections;
using UnityEditor;

namespace timepath4unity { 

[UnityEditor.CustomEditor(typeof(TPResource))]
public class TPResourceInspector : Editor {


	//TPResource r;
	public override void OnInspectorGUI ()
	{
		//r = (TPResource)target;


		base.OnInspectorGUI ();
//		DrawEventInspector();	
	}
	
}


}