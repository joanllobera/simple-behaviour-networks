using UnityEngine;
using System.Collections;


/*!

 \brief The basis of action and perception callbacks as edited in the menu
 \date August 2014
 \details 

The basis of action and perception callbacks as edited in the menu

Partially inspired by the free package called
Unity Editor Delegates 

by Cratesmith (Kieran Lord)
www.cratesmith.com 
cratesmith@cratesmith.com 



\sa TPActionBase, TPPerceptionBase, TPmecanimParam
*/
namespace timepath4unity
{

    [System.Serializable]
    public class TPCallback
    {

        //the possible parameters:


        [SerializeField]
        int intP;

        [SerializeField]
        float floatP;

        [SerializeField]
        string stringP;

        [SerializeField]
        GameObject goP;

        [SerializeField]
        bool boolP;

        //we need to store the name of the method because it will have to be reconstructed for each character using the personality.
        [SerializeField]
        private string method2Callback;


        //the possible delegates kinds:
        [SerializeField]
        public delegate void TPCallback_void();

        [SerializeField]
        public delegate void TPCallback_int(int f);

        [SerializeField]
        public delegate void TPCallback_float(float f);
        [SerializeField]
        public delegate void TPCallback_string(string s);
        [SerializeField]
        public delegate void TPCallback_go(GameObject go);
        [SerializeField]
        public delegate void TPCallback_bool(bool b);


        //The possible kinds:
        public enum CallbackType
        {
            Void = 0,
            Int = 1,
            Float = 2,
            String = 3,
            GameObject = 4,
            Bool = 5
        }

        [SerializeField]
        public CallbackType callbackType;


        //the actual delegate "containers":
        [SerializeField]
        TPCallback_void cv = null;

        [SerializeField]
        TPCallback_int ci = null;

        [SerializeField]
        TPCallback_float cf = null;

        [SerializeField]
        TPCallback_string cs = null;

        [SerializeField]
        TPCallback_go co = null;

        [SerializeField]
        TPCallback_bool cb = null;



        //! a function reconnecting the Action to the selected callback methods. Use for initialisation
        internal TPCallback Reconnect2Mind(Component co)
        {

            TPCallback newcb = null;

            if (this.callbackType == CallbackType.Void)
            {
                newcb = TPCallbackReflection.GetCallback2Method(co, method2Callback);

            }
            else if (this.callbackType == CallbackType.Int)
            {
                newcb = TPCallbackReflection.GetCallback2Method(co, method2Callback);
                newcb.intP = this.intP;
            }
            else if (this.callbackType == CallbackType.Float)
            {
                newcb = TPCallbackReflection.GetCallback2Method(co, method2Callback);
                newcb.floatP = this.floatP;
            }
            else if (this.callbackType == CallbackType.String)
            {
                newcb = TPCallbackReflection.GetCallback2Method(co, method2Callback);
                newcb.stringP = this.stringP;
            }
            else if (this.callbackType == CallbackType.GameObject)
            {
                newcb = TPCallbackReflection.GetCallback2Method(co, method2Callback);
                newcb.goP = this.goP;
            }
            else if (this.callbackType == CallbackType.Bool)
            {
                newcb = TPCallbackReflection.GetCallback2Method(co, method2Callback);
                newcb.boolP = this.boolP;
            }

            return newcb;

        }


        //constructors to set them:
        public TPCallback(TPCallback_void cv)
        {
            this.callbackType = CallbackType.Void;
            this.cv = cv;
            this.method2Callback = cv.Method.Name;
        }

        public TPCallback(TPCallback_int ci)
        {
            this.callbackType = CallbackType.Int;
            this.ci = ci;
            this.method2Callback = ci.Method.Name;
        }
        public TPCallback(TPCallback_float cf)
        {
            this.callbackType = CallbackType.Float;
            this.cf = cf;
            this.method2Callback = cf.Method.Name;
        }
        public TPCallback(TPCallback_string cs)
        {
            this.callbackType = CallbackType.String;
            this.cs = cs;
            this.method2Callback = cs.Method.Name;
        }

        public TPCallback(TPCallback_go co)
        {
            this.callbackType = CallbackType.GameObject;
            this.co = co;
            this.method2Callback = co.Method.Name;
        }
        public TPCallback(TPCallback_bool cb)
        {
            this.callbackType = CallbackType.Bool;
            this.cb = cb;
            this.method2Callback = cb.Method.Name;
        }

        public int IntP
        {
            get
            {
                return intP;
            }
            set
            {
                intP = value;
            }
        }

        public float FloatP
        {
            get
            {
                return floatP;
            }
            set
            {
                floatP = value;
            }
        }

        public string StringP
        {
            get
            {
                return stringP;
            }
            set
            {
                stringP = value;
            }
        }

        public GameObject GoP
        {
            get
            {
                return goP;
            }
            set
            {
                goP = value;
            }
        }

        public bool BoolP
        {
            get
            {
                return boolP;
            }
            set
            {
                boolP = value;
            }
        }


        //! this property gets the method Name from the callbacks, not from the variable method2Callback
        public string MethodName
        {
            get
            {
                return this.method2Callback;

            }





        }


        internal void Invoke()
        {//! \todo test Invoke with all the possible parameters.
            if (callbackType == CallbackType.Void)
            {
                cv();
            }
            else if (callbackType == CallbackType.Int)
            {
                ci(this.intP);
            }
            else if (callbackType == CallbackType.Float)
            {
                cf(this.floatP);
            }
            else if (callbackType == CallbackType.String)
            {
                this.cs(this.stringP);
            }
            else if (callbackType == CallbackType.GameObject)
            {
                this.co(this.goP);
            }
            else if (callbackType == CallbackType.Bool)
            {
                this.cb(this.boolP);
            }
        }

    }





}