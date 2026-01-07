@echo off
REM Verify Intel oneAPI Fortran Compiler Installation
REM This script checks for available Fortran compilers and reports their status

setlocal

echo ========================================
echo Fortran Compiler Verification
echo ========================================
echo.

set "FOUND_ANY=0"

REM Check for Intel ifx (recommended)
echo Checking for Intel ifx (recommended)...
if exist "C:\Tools\Intel\oneAPI\compiler\latest\bin\ifx.exe" (
    echo   [FOUND] C:\Tools\Intel\oneAPI\compiler\latest\bin\ifx.exe
    set "FOUND_ANY=1"
    set "IFX_PATH=C:\Tools\Intel\oneAPI\compiler\latest\bin\ifx.exe"
) else if exist "C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\ifx.exe" (
    echo   [FOUND] C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\ifx.exe
    set "FOUND_ANY=1"
    set "IFX_PATH=C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\ifx.exe"
) else (
    echo   [NOT FOUND] ifx.exe
)

echo.

REM Check for Intel ifort (legacy)
echo Checking for Intel ifort (legacy)...
if exist "C:\Tools\Intel\oneAPI\compiler\latest\bin\ifort.exe" (
    echo   [FOUND] C:\Tools\Intel\oneAPI\compiler\latest\bin\ifort.exe
    set "FOUND_ANY=1"
    set "IFORT_PATH=C:\Tools\Intel\oneAPI\compiler\latest\bin\ifort.exe"
) else if exist "C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\ifort.exe" (
    echo   [FOUND] C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\ifort.exe
    set "FOUND_ANY=1"
    set "IFORT_PATH=C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\ifort.exe"
) else (
    echo   [NOT FOUND] ifort.exe
)

echo.

REM Check for gfortran (alternative)
echo Checking for gfortran (alternative)...
where gfortran.exe >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo   [FOUND] gfortran.exe in PATH
    where gfortran.exe
    set "FOUND_ANY=1"
) else (
    echo   [NOT FOUND] gfortran.exe
)

echo.

REM Check for setvars.bat
echo Checking for Intel oneAPI environment setup...
if exist "C:\Tools\Intel\oneAPI\setvars.bat" (
    echo   [FOUND] C:\Tools\Intel\oneAPI\setvars.bat
    set "SETVARS_PATH=C:\Tools\Intel\oneAPI\setvars.bat"
) else if exist "C:\Program Files (x86)\Intel\oneAPI\setvars.bat" (
    echo   [FOUND] C:\Program Files (x86)\Intel\oneAPI\setvars.bat
    set "SETVARS_PATH=C:\Program Files (x86)\Intel\oneAPI\setvars.bat"
) else (
    echo   [NOT FOUND] setvars.bat
)

echo.
echo ========================================
echo Summary
echo ========================================

if "%FOUND_ANY%"=="1" (
    echo.
    echo [SUCCESS] Fortran compiler(s) found!
    echo.
    if defined IFX_PATH (
        echo Recommended compiler: ifx.exe
        echo   Path: %IFX_PATH%
        echo.
        echo To test, run:
        echo   call "%SETVARS_PATH%"
        echo   ifx --version
    ) else if defined IFORT_PATH (
        echo Available compiler: ifort.exe
        echo   Path: %IFORT_PATH%
        echo.
        echo To test, run:
        echo   call "%SETVARS_PATH%"
        echo   ifort /help
    )
) else (
    echo.
    echo [WARNING] No Fortran compilers found!
    echo.
    echo Please verify installation at:
    echo   C:\Tools\Intel\oneAPI
    echo   OR
    echo   C:\Program Files (x86)\Intel\oneAPI
    echo.
    echo Or install Intel oneAPI Base Toolkit from:
    echo   https://www.intel.com/content/www/us/en/developer/tools/oneapi/toolkits.html
)

echo.
echo ========================================
endlocal
