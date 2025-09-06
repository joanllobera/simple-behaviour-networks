using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using System;
using System.Net;


 using UnityOSC;



/*!
\brief
oscGestFollow contains:
-an OSC connection with the gesture follower (for training, for real time feedback)
-a list of gestures to be stored in order to train the gesture follower

oscSendMove handles the sending of OSC data



*/

public class OSCSendRecMov : MonoBehaviour {

	public bool receive = false;
	public string characterID= "a name"; 

	public static string servername = "Max";
	public static string clientname = "sendMove";

	List<Transform> bones = new List<Transform> ();

	OSCPacket lastReceivedPacket = null;
	Animator anim = null;
	
	void Start () {
		anim = this.gameObject.GetComponent<Animator> (); 

		if (GameObject.Find ("OSCHandler") == null) {
						OSCHandler.Instance.Init ();
			OSCHandler.Instance.CreateServer (servername, 6666);
			OSCHandler.Instance.CreateClient (clientname, IPAddress.Parse ("127.0.0.1"), 5555);
				}

	}
	
	void Update () {
		OSCHandler.Instance.UpdateLogs();

		if (bones.Count <= 0)
						buildBoneSet ();

		if(!receive)
			sendMov ();
		else
			receiveMov ();

	}

	//! builds the bones structure from the standard Humanoid character.
	void buildBoneSet(){
		//right arm animation data:
		Transform t=anim.GetBoneTransform (HumanBodyBones.RightHand);
		Transform last = anim.GetBoneTransform (HumanBodyBones.Chest);
		while (t!=null && t!= last) {
			bones.Add(t);
			t = t.parent;			
		}
		
		//left arm animation data:
		t=anim.GetBoneTransform (HumanBodyBones.LeftHand);
		while (t!=null && t!= last) {
			bones.Add(t);
			t = t.parent;			
		}
		
		//head animation data:
		t=anim.GetBoneTransform (HumanBodyBones.Head);
		last = anim.GetBoneTransform (HumanBodyBones.Hips);
		while (t!=null && t!= last) {
			bones.Add(t);
			t = t.parent;			
		}

		}


	//! sends the movement data obtained from the standard Humanoid character.
	void sendMov(){

		//right arm animation data:
		Transform t=anim.GetBoneTransform (HumanBodyBones.RightHand);
		Transform last = anim.GetBoneTransform (HumanBodyBones.Chest);
		List<float> trackData = new List<float>();
		while (t!=null && t!= last) {
			trackData.AddRange(new List<float>(){t.localRotation.x, t.localRotation.y, t.localRotation.z,t.localRotation.w}); //3*4 = 12 channels
			t = t.parent;			
		}

		//left arm animation data:
		t=anim.GetBoneTransform (HumanBodyBones.LeftHand);
		while (t!=null && t!= last) {
			trackData.AddRange(new List<float>(){t.localRotation.x, t.localRotation.y, t.localRotation.z,t.localRotation.w}); //3*4 = 12 channels
			t = t.parent;			
		}

		//head animation data:
		t=anim.GetBoneTransform (HumanBodyBones.Head);
		last = anim.GetBoneTransform (HumanBodyBones.Hips);
		while (t!=null && t!= last) {
			trackData.AddRange(new List<float>(){t.localRotation.x, t.localRotation.y, t.localRotation.z,t.localRotation.w}); //3*4 = 12 channels
			t = t.parent;			
		}

		//Debug.Log ("the number of channels is: " + trackData.Count);//answer: 52

		string addresspattern = "/" + characterID + "/liveMovData";
		OSCHandler.Instance.SendMessageToClient (clientname, addresspattern, trackData);

	}

	void receiveMov(){
		string addresspattern = "/" + characterID + "/processedMovData";
		if (OSCHandler.Instance.Servers [servername].server.LastReceivedPacket != null && OSCHandler.Instance.Servers [servername].server.LastReceivedPacket.Address.Contains (characterID)) {

						if (lastReceivedPacket == null || lastReceivedPacket.TimeStamp < OSCHandler.Instance.Servers [servername].server.LastReceivedPacket.TimeStamp) {
								lastReceivedPacket = OSCHandler.Instance.Servers ["Max"].server.LastReceivedPacket;
								
									Debug.Log ("I am receiving data under my name: " + characterID);
									for (int i=0; i  < this.bones.Count; i++) {
											Quaternion a = new Quaternion ((float)lastReceivedPacket.Data [4 * i], (float)lastReceivedPacket.Data [4 * i + 1], (float)lastReceivedPacket.Data [4 * i + 2], (float)lastReceivedPacket.Data [4 * i + 3]);
											bones [i].localRotation = a;
									}
							
						}
				}
	}

	void teachGesture(){


		}


}
