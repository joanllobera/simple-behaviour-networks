using UnityEngine;
using System.Collections;


using WellFired;

[USequencerFriendlyName("A Timepath Plot Event")]
[USequencerEvent("Timepath/TPEvent")]
public class TPPlotEvent4US : USEventBase  
{

	public string TPsentence = "TPEvent is...";

	[HideInInspector]
	bool isExternal = false;

	[HideInInspector]
	string role = "";

		
	
	
public override void FireEvent()
{
		Debug.Log(TPsentence);
		Animator animator = AffectedObject.GetComponent<Animator>();
		if(!animator)
		{
			Debug.LogWarning("Affected Object has no Animator component, for uSequencer Event TPEvent", this);
			return;
		}
//! \todo add a way to check if TPAgent exists (i.e., if the timepath plugin is installed)		
/*		TPAgent agent = AffectedObject.GetComponent<TPAgent>();
		if(!agent)
		{
			Debug.LogWarning("Affected Object has no Timepath Agent component, for uSequencer Event TPEvent", this);
			return;
		}

		//get the event name, whether it is external or not....
*/		
}
	
public override void ProcessEvent(float deltaTime)
{
		
		
}
}
