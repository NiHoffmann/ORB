# ORB
This is just a simple dokumentation to setup the project and give a consice explaination as to what this projectis about. The Indepth Documentation is only available in German and can be found [here](Dokumentation).

The goal of this project is to integrate a MicroPython interpreter into the firmware of the Open Robotic Board (ORB).
The ORB is a hardware platform that enables the modular construction of a robot. The ORB firmware provides appropriate functions, such as motor control, for robot programming.
The aim is to make it possible to develop Python programs for the ORB, with a connection to the ORB monitor program.
The ORB monitor program is a development tool that will allow text output, as well as the uploading, starting, and stopping of MicroPython programs.
The existing functionalities of the ORB, such as the development of C++ programs, should remain functionally the same.
> [!NOTE]  
> The ORB uses the STM32F405 Microcontroller, schematics for the board are open to the public. All Informations on the Board, additional software, supported sensors, aswell as the original firmware can be found at: [https://github.com/ThBreuer/OpenRoboticBoard](https://github.com/ThBreuer/OpenRoboticBoard).  
This project is based on the design and work of Thomas Breuer in regards to the ORB-Projects.

The Following Steps will guide you through the Project-Setup. If you only want to be able to Develop and Run Python-Project on your ORB, skip to [User Environment](#user-environment).

# Project Setup
- [Setup Code:Blocks Windows Debug-Environment](#setup-codeblocks-windows-debug-environment)
- [Building with Code:Blocks](#building-with-codeblocks)
- [Setup EMbitz Firmware Development Environment](#setup-embitz-firmware-development-environment)
- [Flash the Firmware](#flash-the-firmware)
- [Develop a Python-Programm](#develop-a-python-programm)
- [Develop a C++-Programm](#develop-a-c-programm)


### Setup Code::Blocks Windows Debug-Environment

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
    
> [!NOTE]  
> If you already have Cygwin Installed, it might interfere with the Msys2 Installation process. If you encounter any problems here, try to remove Cygwin from your Windows-Paths-Variable. You may add it back to your Windows-Paths-Varibale, after the Installation is complete.

4. Initialize the micropython submodule
    ```
    git submodule init
    git submodule update
    ```
5. Build der mpy-cross compiler:
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
6. Open the Code:Blocks Project.  
   1. Select the GNU GCC-Compiler make sure you have the -Og compiler flag set.
       (This will suppress potentially application breaking aggressive optimiztaion)    
   2. Select the "Rebuil" Target this will build the Projekt as well as Rebuild the Micropython Embed Port.

7. Add `<ORB-PATH>\ORB-Python\tools` to your Windows-Path-Varibale.

### Building with Code::Blocks
There is two Build Targets for this Projekt: Build & Rebuild.  
Both Targets will when Build, Compile the File `program.py` located in `<ORB-Path>/ORB-Python/Program`.  
This File will be executed from the Micropython-Interpreter, you can write your Python-Code for Testing here.  

The Differences are as following:  
Build will only compile the Code:Blocks-Project-Part this can be used if there were no QStrings changed.  
These will be changed when: adding or changing names for Functions, Modules or Constants.  
You may use this build target to save on compile time.  
  
Rebuild will also regenerate the Micropython-Embed-Library this will register changes made to the Micropython-Interpreter.  
  
Important Note, just running the Project will not re-compile the program.py file. It is suggested to always use the Build and Run function.  

### Setup EMbitz Firmware Development Environment 
Right now Building the Firmware in only Supported in a Windows Environment

1. Follow the steps described in [Setup Code:Blocks Windows Debug-Environment](#setup-codeblocks-windows-debug-environment) for a basic Project Setup.
2. Install [EMbitz](https://www.embitz.org/).
3. Execute the `_setEnv.bat` inside the ORB-Firmware folder.
    > This will set environment needed for working with the EMBSysLib
4. Goto <ORB-Path>/ORB-Firmware/project and start the EMbitz project.
5. In the Build-Options of this Project make sure "GNU Arm Embedded Toolchain" is selected as compiler.
    > Commonly known as "gcc-arm-none-eabi", this should come pre-packed with the EMbitz IDE.
6. Select Build-Target `all` and rebuild the whole Project.

### Flash the Firmware
Build the Firmware using the EMbitz-Project. Alternativly you can download it from the [releases](./../releases). The Release will also Include Tools to Flash the Firmware. You may ignore the Steps after this and use the Instructions as described in the Release-Readme.

You may flash the Firmware with a tool of your choise.
There is a copy of the DfuSe tool at `ORB\ORB-Firmware\Tools\DfuSe\Bin`.
As well as dfu-util at `ORB\ORB-Python\tools`. (there is a `upload.py` which will flash the firmware for ease of your, you may have to change your firmware version berfore using this.) If you want to use dfu-util make sure to change the dfu-device driver to winusb. A easy to use tool to do this is zadige. Which is also placed inside the tools folder.
> [!NOTE]  
> Putting the ORB in DFU mode, is currently done by plugging in a jumper pin at the pins labeled boot0 (outer 2 pins of the 3 present).

### Develop a Python-Programm
There is two ways to Compile and Run a Python-Programm. 

1. ## **Development Environment**
    If you Followed the Steps to set up the ORB-Project you are ready to go. The `orb-util`-tool will be availabe as command. You can use this to Setup and Compile ORB-Python-Project. 
    Alternatively you can use the Windows-Environment for debugging, as described in [Building with Code::Blocks](#building-with-codeblocks).

2. ## **User Environment**
    1. Download the [Project-Release](./../../releases). 
    2. Follow the Instructions as describes in the Release-Readme. 
    
    This will allow you to develop Python-Programs without the need to setup the ORB-Project.

### Develop a C++-Programm

Goto `ORB/ORB-Application` and execute `_setEnv.bat`. Now you can check out the Example-Projects. Use the different `.bat` files to acces Documentation, ORB-Monitor(Upload-Tool), Clean the Project or Start EMBitz for Developing your Project. Just like Develop a Python-Programm these are Starting points to figure out the workflow.
> [!NOTE]  
> This is not part of my Work but i still feel like it is worth to mention.  
> Although i did some modifications to the ORB-Application as well (Adding Language Flags Section to Linker-Script and Moving Programm-ROM-Address to the same Location as the Python-Programm)
