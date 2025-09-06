using UnityEngine;
using System.Collections;


namespace timepath4unity
{

    [System.Serializable]
    public class TPMecanimParam
    {
        public string name;
        public string type;
        public float duration;
        public bool b;
        public float f;
        public int i;

        public TPMecanimParam(string name, bool b)
        {
            this.name = name;
            this.b = b;
            this.type = TPMecanimParam.boolParam;
            duration = 0;
        }
        public TPMecanimParam(string name, int i)
        {
            this.name = name;
            this.i = i;
            this.type = TPMecanimParam.intParam;
            duration = 0;
        }
        public TPMecanimParam(string name, float f)
        {
            this.name = name;
            this.f = f;
            this.type = TPMecanimParam.floatParam;
            duration = 0;
        }

        //! \todo replace this with an enum for paramType.
        public const string floatParam = "float";
        public const string intParam = "int";
        public const string boolParam = "bool";
    }

}