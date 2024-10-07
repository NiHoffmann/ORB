@echo off

call .\__set_path.bat
echo Running Tests
echo ------------------------------------

echo Compiling   %~n1
python "%ORB_PATH%\ORB-Python\tools\compile.py" %name%.py
echo ------------------------------------
echo Running     %~n1
echo ------------------------------------
call "%ORB_PATH%\ORB-Python\build\bin\Release\ORB-Python.exe" "%cd%\%name%.bin" !arg!   


endlocal

pause