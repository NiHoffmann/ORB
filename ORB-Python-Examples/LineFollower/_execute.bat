@echo off
set ORB_PATH=C:\Users\nils9\Desktop\Bachelorarbeit\ORB\


echo Running Tests
echo ------------------------------------

echo Compiling   %~n1
python "%ORB_PATH%\ORB-Python\tools\compile.py" "line_follower.py"
echo ------------------------------------
echo Running     %~n1
echo ------------------------------------
call "%ORB_PATH%\ORB-Python\build\bin\Release\ORB-Python.exe" "%cd%\line_follower.bin" !arg!   


endlocal

pause