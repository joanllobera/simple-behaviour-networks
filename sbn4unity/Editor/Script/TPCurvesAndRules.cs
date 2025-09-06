using UnityEngine;
using System.Collections;
using System.Collections.Generic;



/*!

 \brief This class deals with the curve candidate
 \date July 2014
 
 
 \todo move the rebuild functions of TPPersonality in this class
 \sa  TPPersonalityWindow
*/
namespace timepath4unity
{

    [System.Serializable]
    public class TPCurvesAndRules : ScriptableObject
    {
        [SerializeField]
        public TPInternalCurve candidate = null;
        private TPPersonality ebn = null;

        //! \todo remove TPPersonalityWindow from TPCurvesAndRules
        private TPPersonalityBoxBase persoBox;

        //! it is true when we just added a new element in the personality
        bool success = false;

        
        public void personalityUpdate(TPPersonality ebn, TPPersonalityBoxBase persoBox)
        {
            this.ebn = ebn;
            this.persoBox = persoBox;
        }


        //! this functions checks if a perception is in the origin or in the destination of a curve.
        public void checkCurveCandidate(TPNode[] nodes)
        {
            //! \todo highlight the nodes where it can connect
            bool IwasDragging;

            
            if (candidate == null)
            {
                IwasDragging = false;

                foreach (TPNode node in nodes)
                {
                    //if (node.r.Contains(Event.current.mousePosition))
                    if (node.r.Contains(Event.current.mousePosition) && Event.current.type == EventType.MouseDown)
                    {

                        //TPNode to = new TPNode(new Rect(Event.current.mousePosition.x - node.r.width, Event.current.mousePosition.y - node.r.y, node.r.width, node.r.width), TPNode.nodeType.candidate);
                        TPNode to = new TPNode(new Rect(Event.current.mousePosition.x - node.r.width, Event.current.mousePosition.y , node.r.width, node.r.width), TPNode.nodeType.candidate);
                        GUI.Label(to.r, " ", TPEditorTools.Psty);
                        candidate = TPInternalCurve.CreateInstance<TPInternalCurve>();
                        candidate.initCandidateCurve(node, to);
                    }
                }

            }
            //else
            if(candidate != null)
            {
                IwasDragging = this.candidate.Dragging;
                this.candidate.updateTo(Event.current.mousePosition);

                Rect dragRect = this.candidate.To.r;
                if (this.candidate.Dragging)    
                {
                    this.candidate.drawLine();
                }
                this.candidate.Drag(dragRect);


                if (IwasDragging && !this.candidate.Dragging)
                {
                    foreach (TPNode node in nodes)
                    {
                        if (node.r.Contains(Event.current.mousePosition))
                        {
                            success = this.addCandidateCurve(node);
                            break;
                        }
                    }

                    IwasDragging = false;
                    this.candidate = null;
                }
                //if (this.candidate != null && !this.candidate.Dragging)
                if (this.candidate != null && IwasDragging && !this.candidate.Dragging)
                {
                    this.candidate = null;
                }
            }

            if (success && Event.current.type == EventType.Layout)
            {
                success = false;
               
                persoBox.rebuildAllCurves();
            }

        }



        //! \todo this is skiped until we find a way to add a new skill despite the fact it is defined in a separate dll, then uncomment this
/*        private bool addNewSkill(TPNode from, TPNode to)
        {
            TPSkill temp = TPCallbackTools.AddSkillByName(ebn,"Skill" + ebn.Skills.Length);
            //temp.Precons.add(from.pB.p);
            temp.Precons.add(from.pB.p.name);
            temp.Effects.addEffect(to.pB.p.name, 1.0f);

            return true;
        }
*/


        private bool addNewPrecon(TPNode from, TPNode to)
        {
            if (to.sB != null && to.sB.s != null)
            {
                to.sB.s.Precons.add2Personality(from.pB.p.name);
                return true;
            }
            else
            {
                return false;
            }
        }


        private bool addNewEffect(TPNode from, TPNode to)
        {
            if (to.sB != null && to.sB.s != null)
            {
                to.sB.s.Effects.addEffect(from.pB.p.name, 1.0f);
                return true;
            }
            else
            {
                return false;
            }
        }

        private bool addNewResource(TPNode from, TPNode to)
        {
            if (to.sB != null && to.sB.s != null)
            {
                //to.sB.s.Resources.Add(from.rB.res);	
                to.sB.s.Resources.addResource(from.rB.res.name, 1.0f);
                return true;
            }
            else
            {
                return false;
            }
        }



        //! \todo this is skiped until we find a way to add a new skill despite the fact it is defined in a separate dll, then uncomment this
/*        private bool addNewGoal(TPNode from, TPNode to)
        {
            TPGoal temp = ebn.AddGoal();
            temp.Rcon.add(from.pB.p.name);
            temp.Gcon.add(to.pB.p.name);
            temp.I = 1.0f;

            return true;
        }

*/

        private bool addNewRelevanceCond(TPNode from, TPNode to)
        {
            if (to.gB != null && to.gB.g != null)
            {
                to.gB.g.Rcon.add2Personality(from.pB.p.name);
                return true;
            }
            else
            {
                return false;
            }
        }

        private bool addNewGoalAchieveCond(TPNode from, TPNode to)
        {
            if (to.gB != null && to.gB.g != null)
            {
                to.gB.g.Gcon.add2Personality(from.pB.p.name);
                to.gB.g.I = 1.0f;
                return true;
            }
            else
            {
                return false;
            }
        }







        public bool addCandidateCurve(TPNode to)
        {
            //! \todo add a check in order to prevent that the same line be created twice by a mistake from the user.


            //Debug.Log("adding candidate curve");
            TPNode from = candidate.From;
            candidate.To = to;

            bool result = false;
            //FOR SKILLS:
            if (from.type == TPNode.nodeType.SkillWhenBox)
            {
                if (to.type == TPNode.nodeType.SkillEffectBox)
                {//we are adding a new skill
                    //! \todo find a way to add a new skill despite the fact it is defined in a separate dll
                    //result = addNewSkill(from, to);
                }
                else if (to.type == TPNode.nodeType.SkillWhen)
                {
                    result = addNewPrecon(from, to);
                }
            }
            else if (from.type == TPNode.nodeType.SkillEffectBox)
            {
                if (to.type == TPNode.nodeType.SkillWhenBox)
                {
                    //! \todo find a way to add a new skill despite the fact it is defined in a separate dll
                    //result = addNewSkill(from, to);
                }
                else if (to.type == TPNode.nodeType.SkillEffect)
                {
                    result = addNewEffect(from, to);
                }
            }
            else if (from.type == TPNode.nodeType.SkillWhen)
            {
                if (to.type == TPNode.nodeType.SkillWhenBox)
                {
                    result = addNewPrecon(to, from);
                }
                else if (to.type == TPNode.nodeType.ResBox)
                {
                    result = addNewResource(to, from);

                }
            }
            else if (from.type == TPNode.nodeType.SkillEffect)
            {
                if (to.type == TPNode.nodeType.SkillEffectBox)
                {
                    result = addNewEffect(to, from);
                }
                else if (to.type == TPNode.nodeType.ResBox)
                {
                    result = addNewResource(to, from);
                }
            }
            else if (from.type == TPNode.nodeType.SkillRes)
            {
                if (to.type == TPNode.nodeType.ResBox)
                {
                    result = addNewResource(to, from);
                }//FOR GOALS:
            }
            else if (from.type == TPNode.nodeType.GoalWhenBox)
            {
                if (to.type == TPNode.nodeType.GoalCondBox)
                {
                    //! \todo this is skiped until we find a way to add a new skill despite the fact it is defined in a separate dll, then uncomment this
                    //result = addNewGoal(from, to);
                }
                else if (to.type == TPNode.nodeType.GoalWhen)
                {
                    result = addNewRelevanceCond(from, to);
                }
            }
            else if (from.type == TPNode.nodeType.GoalCondBox)
            {
                if (to.type == TPNode.nodeType.GoalWhenBox)
                {
                    //! \todo this is skiped until we find a way to add a new skill despite the fact it is defined in a separate dll, then uncomment this
                    //result = addNewGoal(to, from);
                }
                else if (to.type == TPNode.nodeType.GoalCond)
                {
                    result = addNewGoalAchieveCond(from, to);
                }
            }
            else if (from.type == TPNode.nodeType.GoalWhen)
            {
                if (to.type == TPNode.nodeType.GoalWhenBox)
                {
                    result = addNewRelevanceCond(to, from);
                }
            }
            else if (from.type == TPNode.nodeType.GoalCond)
            {
                if (to.type == TPNode.nodeType.GoalCondBox)
                {
                    result = addNewGoalAchieveCond(to, from);
                }
            }//FOR RESOURCES:
            else if (from.type == TPNode.nodeType.ResBox)
            {
                if (to.type == TPNode.nodeType.SkillEffect || to.type == TPNode.nodeType.SkillWhen || to.type == TPNode.nodeType.SkillRes)
                {
                    result = addNewResource(from, to);
                }
            }

            return result;

        }



    }


}