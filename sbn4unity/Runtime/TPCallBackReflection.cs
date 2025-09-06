using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using System;
using System.Reflection;



/*!

 \brief A static class for TPCallback. It is used to encapsulate all the reflections and links between method Names and callback creation
 \date August 2014
 \details 




\sa TPActionBase, TPPerceptionBase, TPCallback, TPActionInspector
*/

namespace timepath4unity
{

    public static class TPCallbackReflection
    {



        // METHODS DEALING WITH REFLECTION:
        public static string[] GetMethodNames(Component co)
        {//! \todo find how to remove the TPAct from TPActionBase
            //FieldInfo[] test = co.GetType ().GetFields ();

            MethodInfo[] methods = co.GetType().GetMethods();
            List<MethodInfo> usefulMethods = new List<MethodInfo>();

            string[] exclusions = new string[] { "Update", "FixedUpdate", "LateUpdate", "Start", "Awake", "OnEnable", "OnDisable", "OnDrawGizmos" };


            foreach (MethodInfo method in methods)
            {
                System.Reflection.ParameterInfo[] parameters = method.GetParameters();

                // events are currently restrected to public, zero parameter functions, that aren't constructors
                if (parameters.Length > 1 || method.IsConstructor || !method.IsPublic) //we would need 
                    continue;

                // return type void only
                if (method.ReturnType != typeof(void))
                    continue;

                // skip all base class methods from monodevelop, component and UnityEngine.Object



                if (method.DeclaringType.IsAssignableFrom(typeof(MonoBehaviour)))
                    //if(method.DeclaringType.IsAssignableFrom(co.GetType()))
                    continue;

                // don't allow unity callbacks to be used as events (Update, Awake, etc)				
                if (System.Array.IndexOf(exclusions, method.Name) != -1)
                    continue;

                if (parameters.Length > 0 && !IsSupportedParamType(parameters[0].ParameterType))
                    continue;


                usefulMethods.Add(method);
            }





            string[] names = new string[usefulMethods.Count];
            for (int i = 0; i < usefulMethods.Count; i++)
                names[i] = methods[i].Name;
            return names;

        }

        public static TPCallback GetCallbackWithName(TPCallback[] array, string name)
        {
            List<TPCallback> list = new List<TPCallback>(array);

            return list.Find(par => TPCallbackReflection.CallbackPointsToThisMethod(par, name));
        }

        //! this method creates a TPCallback given a component and a methodName
        public static TPCallback GetCallback2Method(Component co, string name)
        {
            ParameterInfo[] pInfo = null;
            MethodInfo method;
            try { 
                method  = co.GetType().GetMethod(name);
            }
            catch
            {
                Debug.LogError("component " + co.name + "has no method called " + name);
                return null;
            }
            pInfo = method.GetParameters();


            TPCallback a = null;
            if (pInfo.Length == 0)
            { //it is a void method
                TPCallback.TPCallback_void cb = (TPCallback.TPCallback_void)System.Delegate.CreateDelegate(typeof(TPCallback.TPCallback_void), (object)co, method);
                a = new TPCallback(cb);

            }
            else if (pInfo[0].ParameterType == typeof(int))
            {
                TPCallback.TPCallback_int cb = (TPCallback.TPCallback_int)System.Delegate.CreateDelegate(typeof(TPCallback.TPCallback_int), (object)co, method);
                a = new TPCallback(cb);
            }
            else if (pInfo[0].ParameterType == typeof(float))
            {
                TPCallback.TPCallback_float cb = (TPCallback.TPCallback_float)System.Delegate.CreateDelegate(typeof(TPCallback.TPCallback_float), (object)co, method);
                a = new TPCallback(cb);

            }
            else if (pInfo[0].ParameterType == typeof(String))
            {
                TPCallback.TPCallback_string cb = (TPCallback.TPCallback_string)System.Delegate.CreateDelegate(typeof(TPCallback.TPCallback_string), (object)co, method);

                a = new TPCallback(cb);

            }
            else if (pInfo[0].ParameterType == typeof(GameObject))
            {
                TPCallback.TPCallback_go cb = (TPCallback.TPCallback_go)System.Delegate.CreateDelegate(typeof(TPCallback.TPCallback_go), (object)co, method);
                a = new TPCallback(cb);

            }
            else if (pInfo[0].ParameterType == typeof(bool))
            {
                TPCallback.TPCallback_bool cb = (TPCallback.TPCallback_bool)System.Delegate.CreateDelegate(typeof(TPCallback.TPCallback_bool), (object)co, method);
                a = new TPCallback(cb);
            }




            return (a);
        }

        private static bool CallbackPointsToThisMethod(TPCallback c, string name)
        {
            //FieldInfo fi = c.GetType ().GetField (name);
            MethodInfo fi = c.GetType().GetMethod(name);
            if (fi != null)
                return true;
            else
                return false;

        }

        private static bool IsSupportedParamType(System.Type parameterType)
        {
            if (parameterType == typeof(string))
                return true;

            if (parameterType == typeof(int))
                return true;

            if (parameterType == typeof(float))
                return true;

            if (parameterType == typeof(GameObject))
                return true;

            if (parameterType == typeof(bool))
                return true;
            /*
            if(typeof(UnityEngine.Object).IsAssignableFrom(parameterType))
                return true;
            */
            return false;
        }



    }


}