@echo off
call .\__set_path.bat

python %ORB_PATH%\ORB-Python\tools\compile.py "%name%.py"
call   %ORB_PATH%\ORB-Python\tools\dfu-util.exe -a 0 --dfuse-address 0x080E0000 -D .\%name%.bin
@pause