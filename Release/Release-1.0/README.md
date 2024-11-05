## Setup

1. Install Python, make sure it is added to your windows-path-variable.
> Run `python --version` to confirm.
3. Add `Release<Version>\Tools` to your windows-paths-variable.
> Run `orb-util --help` to confirm.
4. Configure ORB-DFU-mode-driver as 'win-usb-device' (with zadige or your tool of choice)
>[!NOTE]  
>This step is optional if you already has the Firmware installed on your ORB.
5. Use `orb-util` to upload and compile python-programs, or flash your firmware.

## Quick Guide

Execute these commands from your python-project-directory. You may use 'Template' as a starting point.  
The Quick Guide suggest using the .build file, as found in 'Template':
```
{
    "import": ["import1.py", "import2.py", ....],
    "main": "main.py"
}
```
This will allow you to compile mutiple python-scripts into one binary.

### Flash firmware
```batch
orb-util --flash <version>
```

### Test your Program on Widnows:
```batch
orb-util -b -t win
```

### Compile and Upload with ORB-Monitor
1. You can Start the ORB-Monitor setup with your program using this command:
    ```batch
    orb-util -b
    ```
2. Select your Port (USB)
3. Press "Download Program"
4. Recompilation can be done using:
   ```batch
   orb-util -b -c
   ```
   or initialize the program download right away:
   ```batch
   orb-util -b
   ```
