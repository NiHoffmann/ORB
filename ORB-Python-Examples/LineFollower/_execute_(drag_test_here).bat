@echo off
set ORB_PATH=C:\Users\nils9\Desktop\Bachelorarbeit\ORB\

setlocal enabledelayedexpansion

echo Running Tests
echo ------------------------------------

set /p arg=<"%~n1.par"

echo Compiling   %~n1
python "%ORB_PATH%\ORB-Python\tools\compile.py "%~n1.py"
echo ------------------------------------
echo Running     %~n1
echo ------------------------------------
call "%ORB_PATH%\ORB-Python\build\bin\Release\ORB-Python.exe" "%cd%\%~n1.bin" !arg!   


endlocal

pause