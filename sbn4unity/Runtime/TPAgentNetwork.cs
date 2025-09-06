using UnityEngine;
using System.Collections;
using System.Collections.Generic;


using System;
//for callbacks:
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


/*!

 \brief A static class that  imports functions from a dll, which stands as the mind of the agent
 
 
*/

/*
 * Examples used to handle callback functions: 
 * http://stackoverflow.com/questions/9396038/call-a-c-sharp-method-function-from-a-c-dll-which-is-loaded-from-c-sharp-with
 * http://social.msdn.microsoft.com/Forums/en-US/9bdc75f0-364d-4568-8469-4071a7d2ecd7/callback-from-native-c-dll-to-c-delegate?forum=netfxcompact
 */


namespace timepath4unity
{

    internal static class TPAgentNetwork
    {

        private const  string dllname = "sbncore";

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void logPointer([MarshalAs(UnmanagedType.LPStr)] string cs);

       

        public static void logPlugin(string a)
        {
            Debug.Log("timepath ebn: " + a);
        }

        #region test import functions

        /*

        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern void testAddName([MarshalAs(UnmanagedType.LPStr)] string cs);

        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern void testLog2Unity(IntPtr logP);

        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern double testPerception(int netID, IntPtr perceptionP);

        //extern "C"__declspec(dllexport) double testPerceptionsInNet(int netID,char* name){
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern double testPerceptionInNet(int netID, [MarshalAs(UnmanagedType.LPStr)] string cs);

        //extern "C"__declspec(dllexport) void testAction(int netID, int actionID, UnityAction pFn, double activation){
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern double testAction(int netID, int actionID, TPActionBase.TPAct actionP, double activation);

        // extern "C"__declspec(dllexport) int testActionInNet(int netID, char* name, double activation){
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern double testActionInNet(int netID, [MarshalAs(UnmanagedType.LPStr)] string cs, double activation);

        //a dumb action to test if callbacks work:
        public static void action4test2(int netID, double d)
        {

            Debug.Log(" executed function testAction2 with value " + d);

        }


        public static int tesDLLConnection()
        {


            Debug.Log(" test log callback: ");
            logPointer lPtr = logPlugin;
            IntPtr fPtr2 = Marshal.GetFunctionPointerForDelegate(lPtr);


            testLog2Unity(fPtr2); //OK
            InitLog(fPtr2);



            string name = "INTHENAMEOF";
            Debug.Log(" test string passing ");
            testAddName(name);//OK!!   

            return 1;
        }
        */
        #endregion



        #region public plugin functions
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitLog(IntPtr logP);

        //extern "C"__declspec(dllexport) int CreateNet(char * id){
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CreateNet(string uniqueID);

        //extern "C"__declspec(dllexport) int AddPerception(int netID, char* name, UnityPerception pFn)
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern int AddPerception(int mindID, int perceptionID, [MarshalAs(UnmanagedType.LPStr)] string cs, TPPerceptionBase.TPPerceive percepP);


        //extern "C"__declspec(dllexport) int AddAction(int netID, char* name, UnityAction pFn)
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern int AddAction(int mindID, int actionID, [MarshalAs(UnmanagedType.LPStr)] string cs, TPActionBase.TPAct actionP);





        //! Adds a resource
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern int AddResource(int mindID, int resourceID, [MarshalAs(UnmanagedType.LPStr)] string cs, TPPerceptionBase.TPPerceive percepP, double amount);
        //public static extern int AddResource(int netID, int resourceID, [MarshalAs(UnmanagedType.LPStr)] string cs, double amount);

        /*
        //! updates the amount of resource available. NOT USED ANYMORE, resources are updated within the updatemind loop
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        //extern "C"__declspec(dllexport) int UpdateResourceAvailable(int netID, char* name, double iAmount){
        public static extern int UpdateResourceAvailable(int netID, [MarshalAs(UnmanagedType.LPStr)] string cs, double amount);

         */
 
        //extern "C"__declspec(dllexport) double GetResourceNotUsed(int netID, char* name){
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetResourceNotUsed(int mindID, [MarshalAs(UnmanagedType.LPStr)] string cs);


        //! it performs a complete perception / activation spreading / decision 
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern void UpdateNet(int mindID);


        //! it connects all the elements in the mind (an extended behavior network), as well as the story or stories if they are defined
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ConnectNet(int mindID, [MarshalAs(UnmanagedType.LPStr)] string rules);

        // ! it creates a new story within an existing (and connected) mind
        //[DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        //public static extern void CreateStory(int netID, [MarshalAs(UnmanagedType.LPStr)] string plot);

#if TRAMA
        //! it starts all the stories declared
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StartStories();
#endif
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ClearNets();


#endregion

#if TRAMA
#region story related plugin functions
        //! give an action and also function to estimate how much a certain action was already executed by some other agent 
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        //extern "C"__declspec(dllexport) void AddActionPerception(int mindID, int perceptionID, int actionID, char* actionName, UnityPerception percepCall, UnityAction actCall){
        public static extern int AddActionPerception(int mindID, int perceptionID, int actionID, [MarshalAs(UnmanagedType.LPStr)] string cs, TPPerceptionBase.TPPerceive percepP, TPActionBase.TPAct actionP);

        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern int AddActionPerceptionRole(int mindID, int perceptionID, int actionID, [MarshalAs(UnmanagedType.LPStr)] string actionName, TPPerceptionBase.TPPerceive percepP, TPActionBase.TPAct actionP, [MarshalAs(UnmanagedType.LPStr)] string role);


        //! sets the delay interval to start at tmin 	
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetMinDelay(int mindID, [MarshalAs(UnmanagedType.LPStr)] string actionName, double time);

        //! sets a delay interval to end at tmax
        [DllImport(dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetMaxDelay(int mindID, [MarshalAs(UnmanagedType.LPStr)] string actionName, double time);

#endregion
#endif

    }




}