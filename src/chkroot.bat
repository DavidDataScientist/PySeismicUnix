@echo off
REM Windows version of chkroot.sh
REM Check to see if the CWPROOT environment variable is set

if "%CWPROOT%"=="" (
    echo ERROR: CWPROOT environment variable is not set!
    echo Please read README_TO_INSTALL for more information!
    echo .... Aborting make
    exit /b 1
)

echo Installing the CWP codes under the ROOT = %CWPROOT%
echo Continue install? [y/n]
set /p RESP=
if /i not "%RESP%"=="y" (
    if /i not "%RESP%"=="yes" (
        echo Aborting make
        exit /b 1
    )
)

exit /b 0

