using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using WellFired;

[USequencerFriendlyName("A New Timepath Story Plot")]
[USequencerEvent("Timepath/TPPlotBegin")]
public class TPPlotBegin : USEventBase  
{

	public string TPPlotName = "Story1";

	[HideInInspector]
	protected List<string> roles = new List<string>();

		
	
	
public override void FireEvent()
{
				
}
	
public override void ProcessEvent(float deltaTime)
{
		
		
}
}
