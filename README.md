# ORB
The goal of this project is to integrate a MicroPython interpreter into the firmware of the Open Robotic Board (ORB).
The ORB is a hardware platform that enables the modular construction of a robot. The ORB firmware provides appropriate functions, such as motor control, for robot programming.
The aim is to make it possible to develop Python programs for the ORB, with a connection to the ORB monitor program.
The ORB monitor program is a development tool that will allow text output, as well as the uploading, starting, and stopping of MicroPython programs.
The existing functionalities of the ORB, such as the development of C++ programs, should remain functionally the same.

## Project Setup

1. (Install Git and) Clone the project
    ```
    git clone https://github.com/NiHoffmann/ORB
    ```
2. Download and Install [Python3](https://www.python.org/downloads/).  
   Make sure to add python3 to your environment variables, the python3 command should be accessible.   
   To Check if correctly installed execute:
    ```code
    python3 --version
    ```
3. Install the gcc-compiler, make, rm (utils)  
   I suggest using [Msys2](https://www.msys2.org/) for this installation.
   It will come prepacked with all the needed util for this Project.  
   Make sure to add the Msys2 bin folder for your installation to paths.  
   To Check if correctly installed execute:
    ```code
    gcc --version
    ```
5. Initialize the micropython submodule
    ```
    git submodule init
    git submodule update
    ```
6. Build der mpy-cross compiler:
    ```
    cd <ORB-PATH>/micropython/mpy-cross
    make
    ```
    or installed it via pip:  
    ```code
    pip install mpy-cross
    ```
    Make sure to add mpy-cross to your path.  
    To Check if correctly installed execute:
    ```code
    mpy-cross --verison
    ```
7. Open the Code:Blocks Project.  
   1. Select the GNU GCC-Compiler make sure you have the -Og compiler flag set.
       (This will suppress potentially application breaking aggressive optimiztaion)    
   2. Select the "Rebuil" Target this will build the Projekt as well as Rebuild the Micropython Embed Port.
       Once this step if completed the Projekt setup is complete.

## Building with Code:Blocks
There is two Build Targets for this Projekt: Build & Rebuild.  
Both Targets will when Build Compile the File 'program.py' located in '<ORB-Path>/ORB-Python/Program'.  
This File will be executed from the Micropython-Interpreter, you can write your Python-Code for Testing here.  

The Differences are as following:  
Build will only compile the Code:Blocks-Project-Part this can be used if there were no QStrings changed.  
These will be changed when: adding or changing names for Functions, Modules or Constants.  
You may use this build target to save on compile time.  
  
Rebuild will also regenerate the Micropython-Embed-Library this will register changes made to the Micropython-Interpreter.  
  
Important Note, just running the Project will not re-compile the program.py file. It is suggested to always use the Build and Run function.  

## Building the ORB-Firmware
