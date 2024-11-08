
## Setup

1. Install Python and make sure it is added to your Windows PATH variable.
   > Run `python --version` to confirm.
2. Add `Release<Version>/Tools` to your Windows PATH variable.
   > Run `orb-util --help` to confirm.
3. Configure the ORB DFU mode driver as a 'WinUSB device' (using Zadig or a tool of your choice).
> [!NOTE]  
> This step is optional if you already have the firmware installed on your ORB.
4. Use `orb-util` to upload and compile Python programs or flash your firmware.

## Quick Guide

Execute these commands from your Python project directory. You may use 'Template' as a starting point.  
The Quick Guide suggests using the `.build` file, as found in 'Template':
```cpp
{
    "import": ["import1.py", "import2.py", ...],
    "main": "main.py"
}
```
This will allow you to compile multiple Python scripts into one binary.

### Flash firmware
```batch
orb-util --flash <version>
```

### Test your program on Windows
```batch
orb-util -b -t win
```

### Compile and Upload with ORB Monitor
1. You can start the ORB Monitor setup with your program using this command:
   ```batch
   orb-util -b
   ```
2. Select your port (USB).
3. Press "Download Program."
4. Recompilation can be done using:
   ```batch
   orb-util -b -c
   ```
   or initialize the program download right away:
   ```batch
   orb-util -b
   ```
