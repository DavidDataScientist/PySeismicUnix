@echo off
REM Windows version of Configure script
REM Get the system and processor type information and select appropriate Makefile.config

setlocal enabledelayedexpansion

set "CONFIG=Makefile.config"

REM Get the system and processor type information
REM On Windows, we'll use environment variables and system info
REM For Windows, we'll use the Windows_MSVC config
set "SYS=Windows"
set "ARCH=x64"

REM Check for a predefined config and use it if found

if exist "configs\%CONFIG%_%SYS%_%ARCH%" (
    echo.
    echo Copying configs\%CONFIG%_%SYS%_%ARCH% to %CONFIG%
    if exist "%CONFIG%" (
        move "%CONFIG%" "%CONFIG%_old" >nul 2>&1
    )
    copy "configs\%CONFIG%_%SYS%_%ARCH%" "%CONFIG%" >nul
    goto :done
)

REM Try Windows_MSVC config
if exist "configs\%CONFIG%_Windows_MSVC" (
    echo.
    echo Copying configs\%CONFIG%_Windows_MSVC to %CONFIG%
    if exist "%CONFIG%" (
        move "%CONFIG%" "%CONFIG%_old" >nul 2>&1
    )
    copy "configs\%CONFIG%_Windows_MSVC" "%CONFIG%" >nul
    goto :done
)

REM use the generic template

echo.
echo I don't find a predefined %CONFIG% for your system.
echo A generic template has been used.
echo.
echo You need to edit the file to describe your system.
echo Pay particular attention to the CWP_ENDIAN flag.

echo Copying configs\%CONFIG%_generic to %CONFIG%
if exist "%CONFIG%" (
    move "%CONFIG%" "%CONFIG%_old" >nul 2>&1
)

copy "configs\%CONFIG%_generic" "%CONFIG%" >nul

:done
echo.
echo Configuration file %CONFIG% has been set up.
echo You may need to edit it to match your system.
echo.

exit /b 0

