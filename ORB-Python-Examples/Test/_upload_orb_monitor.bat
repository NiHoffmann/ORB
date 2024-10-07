@echo off
call __set_path.bat
python "%ORB_PATH%\ORB-Python\tools\compile.py" %name%.py
timeout /t 2 /nobreak > nul
start %ORB_PATH%\ORB-Python\tools\ORB-Monitor.exe -prog %cd%\%name%.hex