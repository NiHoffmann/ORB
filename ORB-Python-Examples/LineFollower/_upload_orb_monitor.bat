@echo off
call __set_path.bat
python "%ORB_PATH%\ORB-Python\tools\compile.py" line_follower.py
timeout /t 2 /nobreak > nul
start %ORB_PATH%\ORB-Python\tools\ORB-Monitor.exe -prog %cd%\line_follower.hex