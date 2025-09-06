using UnityEditor;
using UnityEngine;
using System.Collections;

using WellFired;


[CustomUSEditor(typeof(TPPlotEvent4US))]
public class TPPlotEventEditor : USEventBaseEditor
{

	
	new public Rect RenderEvent(Rect myArea, USEventBase thisEvent)
	{


		TPPlotEvent4US tpe = thisEvent as TPPlotEvent4US;
	
	

	DrawDefaultBoxTP(myArea, tpe);
	
	GUILayout.BeginArea(myArea);
	

	if (! USSelection.IsInUnitySelection(thisEvent)){
		GUILayout.Label("TP: "+tpe.TPsentence, defaultBackground);
		}
	else{
			GUILayout.Label("TP: "+tpe.TPsentence+"\n", defaultBackground);	
		}	
	GUILayout.EndArea();
	
	return myArea;
	}
	
	public void DrawDefaultBoxTP(Rect myArea, TPPlotEvent4US thisEvent)
	{
		Color prevColor = GUI.color;
		if (USSelection.IsInUnitySelection(thisEvent))
			GUI.color = Color.cyan;
			//GUI.TextField(myArea, thisEvent.displayValue.ToString());
			GUI.Box(myArea, "");

		GUI.color = prevColor;
	}
	
}

