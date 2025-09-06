using UnityEngine;
using System.Collections;
using System.Collections.Generic;

//accessory class to address the fact that classes with generics cannot be serialized. As a workaroudn:
//see: http://answers.unity3d.com/questions/214300/serializable-class-using-generics.html


namespace timepath4unity
{
#if TRAMA
    //! \todo use this to remove completely TPPlottest in TPAgent

    [System.Serializable]
    public class TPAgentStories : HashSet<TPPlot>
    {

    }

#endif

}