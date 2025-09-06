using UnityEngine;
using System.Collections;
using System.Collections.Generic;




/*!

 \brief An instrumental class used to display the nodes connecting TPInternalCurve to goalBox, skillBox, perceptionBox and TPPlotSentenceBox
 \date July 2014



 \sa  TPCurvesAndRules, TPInternalCurve, TPSkillBox, TPGoalBox, TPProposition, TPPlotSentence
*/




namespace timepath4unity
{

    public class
    TPNode
    {
        [SerializeField]
        public Rect r;//the rectangle
        [SerializeField]
        public int type = 0;

        //for basic nodes:
        public TPPerceptionBox pB = null;
        public TPSkillBox sB = null;
        public TPGoalBox gB = null;
        public TPResourceBox rB = null;


        public static class nodeType
        {
            public const int SkillWhenBox = 11; //attached to the preconditions of a skill
            public const int SkillEffectBox = 12; //attached to the effects of a skill
            public const int SkillWhen = 13;
            public const int SkillEffect = 14;
            public const int SkillRes = 15;

            public const int GoalCondBox = 21;
            public const int GoalWhenBox = 22;
            public const int GoalWhen = 23;
            public const int GoalCond = 24;

            public const int ResBox = 33;


            //public const int and = 101;
            public const int not = 102;
            public const int yes = 103;//when the not node is not negated.


            public const int candidate = 99;//drawn at the end of the candidate line

            public const int TminTmax = 98; //drawn at the begining of a TPPlotSentence

        }

        public TPNode(Rect rec, int kind)
        {
            this.r = rec;
            this.type = kind;
        }

        public TPNode(Rect rec, int kind, TPPerceptionBox p)
        {
            this.r = rec;
            this.type = kind;
            this.pB = p;


        }

        public TPNode(Rect rec, int kind, TPSkillBox s)
        {
            this.r = rec;
            this.type = kind;
            this.sB = s;


        }

        public TPNode(Rect rec, int kind, TPGoalBox g)
        {
            this.r = rec;
            this.type = kind;
            this.gB = g;

        }

        public TPNode(Rect rec, int kind, TPResourceBox r)
        {
            this.r = rec;
            this.type = kind;
            this.rB = r;

        }
    }


}