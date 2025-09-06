# Simple behaviour networks

Simple behaviour networks are a type of behaviour-based agent.

## Introduction

Simple behaviour networks are an example of Behaviour-based artificial intelligence. The motivation behind it is to control the behaviour of a robot (or a virtual reality character) with a network of interconnected modules, each of which encapsulates a skill. Each module is defined by a relatively small set of simple rules, and the result is a distributed control architecture whose components can be interpreted separately.

The formal definition of simple behaviour networks can be found in [this paper](https://doi.org/10.1109/TCIAIG.2017.2755699) submitted in 2016 (with a freely accessible pre-print [here](./files/SBN_preprint_final_version.pdf)).

## Background

Simple behaviour networks build from the early work of [Pattie Maes](https://www.media.mit.edu/people/pattie/overview/) and the view of distributed architectures to control robots put forward by [Rodney Brooks](https://people.csail.mit.edu/brooks/), as well as from the extensions proposed by [Klaus Dorer](https://scholar.google.com/citations?user=f4r7JbMAAAAJ&hl=de).

I used this kind of architecture in [my PhD](https://joanllobera.github.io/files/lloberaPhD2012.pdf) to control the behaviour of virtual reality characters, and show they could be compelled to coordinate their behaviour to satisfy a pre-defined story plot. 

Later, a grant of the [Hasler Foundation](https://haslerstiftung.ch/en/welcome-to-the-hasler-foundation/) while a postdoc at EPFL gave me the time and ressources needed to port it to Unity3D, a game engine commonly used for VR development. 

## Code organization

- The folder **C++** contains an implementation of behaviour networks. It is based on the older work of Klaus Dorer. What I did was, first, remove the dependencies from the Microsoft Foundation classes. It compiles into a simple .dll library that can be called from external software. As per my last test (September 2025) it compiles well using Microsoft Visual Studio on Windows. In principle the code is not dependent on windows classes, so it could compile for other platforms but I have never taken the time to do it.

- The folder **sbn4unity** contains a Unity package. It implement a wrapper of the C++ .dll with a visual interface to create and edit the behaviour networks.   To integrate it in your project add the following line in your project manifest: 
  
  ```json
      "io.timepath.sbn": "https://github.com/joanllobera/simple-behaviour-networks.git?path=sbn4unity#0.2.0",
  ```

  Make sure you also import the Sample files, to be able to define your perceptions and actions in a way that is consistent for the world you are creating.

- The folder **sbn4unity_compile** is a legacy folder containing older code, and files to compile the code in the package in .dll libraries. This was implemented before the Unity package system inspired on npm was introduced, and it is not needed anymore. Older projects, like the tutorial described below, still use these files.



## See also

A Tutorial showing the use of sbn4unity to create an agent that can take decisions on top of the ADAPT animation system, can be found [here](https://github.com/joanllobera/simple-behaviour-networks-tutorial.git))
