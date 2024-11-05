
VM API
==============

.. module:: python_vm
   :synopsis: Python Virtual Machine (PythonVM) class and related functionality

.. cpp:class:: PythonVM

   A class to manage the Micropython-VirtualMachine. Providing methods for running, stopping, and retrieving the VM’s status.

   .. cpp:function:: void PythonVM::run(LoadLengthFunction loadLength, LoadProgramFunction loadProgram, uint8_t arg)

      Starts the VM with the specified load functions and argument.

      :param loadLength: Function pointer that returns the program length
      :param loadProgram: Function pointer that loads the program data
      :param arg: Additional argument passed to the program

   .. cpp:function:: bool PythonVM::isRunning()

      Checks if the VM is currently running.

      :returns: True if the VM is running, otherwise False
      :rtype: bool

   .. cpp:function:: void PythonVM::stopProgram()

      Stops the currently running program in the VM.

   .. cpp:function:: int PythonVM::getExitStatus()

      Retrieves the exit status of the VM.

      :returns: Exit status code
      :rtype: int

   .. cpp:function:: const char* PythonVM::getExitInfo()

      Returns additional information about the VM’s exit status.

      :returns: Exit information string
      :rtype: const char*

.. cpp:enum:: Status

   Enumeration for VM exit statuses, indicating various exit conditions.

   **Status codes**:

   - **NORMAL**: Program exited normally.
   - **EXCEPTION**: Program exited with an exception.
   - **INTERRUPT**: Program was interrupted by User.
