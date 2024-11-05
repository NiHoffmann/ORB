@echo off
set executionPath=%CD%
set toolsPath=%~f0
for %%i in ("%toolsPath%") do set toolsPath=%%~dpi

set args=
:loop
if "%~1"=="" goto endArgs
set args=%args% "%~1"
shift
goto loop
:endArgs

python %toolsPath%\ORBExecution.py "%executionPath%" %args%