@echo off

setlocal enabledelayedexpansion

echo Running Tests
echo ------------------------------------
if "%~1"=="" (
        for %%f in (test.*.py) do (
        set "filename=%%~nf"
        set "testName=!filename:test.=!"
        
        IF EXIST "%%~nf.par" (
            set /p arg=<"%%~nf.par"
        )

        echo Compiling   !testName!
        python ../../../ORB-Python/tools/compile.py "%%~nf.py"
        echo ------------------------------------
        echo Running     !testName!
        echo ------------------------------------
        call "../../../ORB-Python/build/bin/Release/ORB-Python.exe" "%cd%\%%~nf.bin" !arg!
        echo.
        echo ------------------------------------
        echo Description !testName!
        echo ------------------------------------
        type "%%~nf.md"
        echo.
        echo ------------------------------------
        del %%~nf.bin
    )
) else (
    set /p arg=<"%~n1.par"

    echo Compiling   %~n1
    python ../../../ORB-Python/tools/compile.py "%~n1.py"
    echo ------------------------------------
    echo Running     %~n1
    echo ------------------------------------
    call "../../../ORB-Python/build/bin/Release/ORB-Python.exe" "%cd%\%~n1.bin" !arg!   
    echo.
    echo ------------------------------------
    echo Description %~n1
    echo ------------------------------------
    type "%~n1.md"
    echo.
    echo ------------------------------------
    del %~n1.bin
)

endlocal

pause