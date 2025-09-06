using UnityEngine;
using System.Collections;
using System.Collections.Generic;

using System.Linq;//needed to convert a hashTag in an array

/*!

 \brief A Timepath Role
 \date 08.2014
 \details  A basic extension of TPResource

	It also has a singleton used to manage what roles are declared within all the story plots.

 \todo: add something that corresponds to a tag associated to each role when starting the gameplay.

 \sa TPAgent, TPPlot
*/

namespace timepath4unity
{

#if TRAMA
    public class TPRole : TPResource
    {
        /*
        [SerializeField]
        TPPersonality perso;

        public TPPersonality Perso
        {
            get
            {
                return perso;
            }
            set
            {
                perso = value;
            }
        }*/

        //a singleton to deal with all the Roles. NEVER call the singleton directly (normally, dealt with private/public permissions


        class RolesSingleton
        {
            [SerializeField]
            private static List<TPRole> allRolesSingleton = null;
            public static List<TPRole> getInstance()
            {
                if (allRolesSingleton == null)
                {
                    RolesSingleton.FindAllRoles(ref allRolesSingleton);
                }
                return allRolesSingleton;


            }
            public static void updateGlobalList()
            {
                RolesSingleton.FindAllRoles(ref allRolesSingleton);
            }

            //!this functions rebuilds the GLOBAL list of roles from ALL PLOTS. but it preserves the existing roles!
            //! \todo this is not efficient, but it is wroks. and it is secure. and since we are not running this at runtime, nor often, and there are few roles in each story... (many sentences possible, though)
            //! \todo think how to deal with this using HashSets (this would avoid duplicates!, which are currently too often present)
            private static void FindAllRoles(ref List<TPRole> listRoles)
            {

                if (listRoles == null)
                    listRoles = new List<TPRole>();
                else
                    listRoles.Remove(null); //some roles could be deleted in the inspector.


                //is there also a gameObject to store them in the inspector?
                GameObject rolesGO = GameObject.Find("All Roles");
                if (rolesGO == null)
                {
                    rolesGO = new GameObject();
                    rolesGO.name = "All Roles";
                }




                TPPlot[] plotVector = (TPPlot[])Resources.FindObjectsOfTypeAll(typeof(TPPlot));
                string[] roles1sentence;

                TPRole roleInGlobalList;

                //we also monitor whether the roles in the global list are used:
                List<bool> isRoleUsed = new List<bool>();
                for (int i = 0; i < listRoles.Count; i++)
                    isRoleUsed.Add(false);


                foreach (TPPlot plot in plotVector)
                {
                    foreach (TPPlotSentence s in plot.Plot)
                    {
                        //roles1sentence = TPPlotSentence.getRoleNamesInSentence(s.name);
                        roles1sentence = s.getRoles();// TPPlotSentence.getRoleNamesInSentence(s.name);
                        foreach (string roleName in roles1sentence)
                        {


                            int i = listRoles.FindIndex(res => res.name.Equals(roleName));

                            //a. is the role in the global list of roles? If not, we add it
                            if (i == -1)
                            {
                                GameObject go = new GameObject();
                                roleInGlobalList = go.AddComponent<TPRole>();
                                roleInGlobalList.name = roleName;
                                roleInGlobalList.transform.parent = rolesGO.transform;
                                listRoles.Add(roleInGlobalList);
                                isRoleUsed.Add(true);

                                //b. if it is, we simply pick it.
                            }
                            else
                            {
                                roleInGlobalList = listRoles[i];
                                isRoleUsed[i] = true;
                            }


                        }
                    }
                }

                //are there some roles that are not been used in any plot? if so we remove them.
                for (int i = 0; i < listRoles.Count; i++)
                {
                    if (!isRoleUsed[i])
                        listRoles.RemoveAt(i);
                }
                listRoles.Remove(null); //we now remove the empty slots.

                //	   return allRoles.ToArray();

            }






        }


        private static List<TPRole> GetAllRoles()
        {
            return RolesSingleton.getInstance();
        }

        /*
        public static TPRole[] AllRoles{get{ 

                return TPRole.FindAllRoles().ToArray();}
        }*/


        public static TPRole getRole(string name)
        {
            return TPRole.GetAllRoles().Find(r => r.name.Equals(name));
        }



        //! this function returns the roles corresponding to a given plot 
        public static TPRole[] getRolesInPlot(TPPlot theplot)
        {

            //we need to check any new role in a story against the global list, since one Role can be in several plots.
            //we therefore update the global list for ALL plots (including this plot!)
            //FindAllRoles (allRoles);
            RolesSingleton.updateGlobalList();


            HashSet<TPRole> relevantRoles = new HashSet<TPRole>();//this grants it is not repeated
            string[] roles1sentence;
            foreach (TPPlotSentence s in theplot.Plot)
            {
                roles1sentence = s.getRoles();
                foreach (string roleName in roles1sentence)
                {

                    TPRole newRole = TPRole.GetAllRoles().Find(res => res.name.Equals(roleName));

                    //since it cannot be repeated, we can directly:
                    relevantRoles.Add(newRole);
                }
            }
            return relevantRoles.ToArray();


        }


    }

#endif

}