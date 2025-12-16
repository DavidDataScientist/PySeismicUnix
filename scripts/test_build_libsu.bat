@echo off
REM Test build script for libsu only
REM This script builds only libsu to diagnose build issues

setlocal EnableDelayedExpansion

REM Change to workspace root directory
cd /d "%~dp0.."

REM Set CWPROOT: use environment variable if set, otherwise use current working directory
if not defined CWPROOT (
    set "CWPROOT=%CD%"
)

REM Ensure CWPROOT_WIN has a drive letter for Windows commands
set "CWPROOT_WIN=%CWPROOT%"
if not "%CWPROOT_WIN:~1,1%"==":" (
    REM No drive letter, prepend current drive
    for %%d in ("%CD%") do (
        if "!CWPROOT_WIN:~0,1!"=="\" (
            REM Absolute path without drive (e.g., \src\proto), prepend drive
            set "CWPROOT_WIN=%%~d!CWPROOT_WIN!"
        ) else (
            REM Relative path, resolve relative to current directory
            pushd "!CWPROOT_WIN!" >nul 2>&1
            if not errorlevel 1 (
                set "CWPROOT_WIN=!CD!"
                popd >nul 2>&1
            ) else (
                REM If pushd fails, assume it's meant to be absolute from root
                set "CWPROOT_WIN=%%~d\!CWPROOT_WIN!"
            )
        )
    )
)

echo ========================================
echo Test Build: libsu only
echo ========================================
echo.
echo CWPROOT = %CWPROOT_WIN%
echo.

REM Find Visual Studio 2022
set "VS2022_PATH="
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
    goto :found_vs
)
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Professional"
    goto :found_vs
)
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise"
    goto :found_vs
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community"
    goto :found_vs
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional"
    goto :found_vs
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise"
    goto :found_vs
)

echo "ERROR: Visual Studio 2022 not found!"
exit /b 1

:found_vs
echo Found Visual Studio 2022 at: %VS2022_PATH%
echo.

REM Initialize VS 2022 environment
echo Initializing Visual Studio 2022 Developer Command Prompt (x64)...
set "VCVARS=%VS2022_PATH%\VC\Auxiliary\Build\vcvarsall.bat"
call "%VCVARS%" x64
if errorlevel 1 (
    echo "ERROR: Failed to initialize VS 2022 environment"
    exit /b 1
)

REM Verify VS 2022 environment is active by checking for cl.exe
where cl >nul 2>&1
if errorlevel 1 (
    echo "ERROR: VS 2022 environment not properly initialized (cl.exe not found)"
    exit /b 1
)

REM Display confirmation that VS 2022 compiler is ready
echo VS 2022 C/C++ compiler (cl.exe) is ready
echo.

REM Add GNU Make to PATH
set "MAKEPATH=C:\Tools\gnu\bin"
set "PATH=%MAKEPATH%;%PATH%"

REM Verify make is available
where make >nul 2>&1
if errorlevel 1 (
    echo "ERROR: GNU Make not found in %MAKEPATH%"
    exit /b 1
)

REM Set CWPROOT for Make (use forward slashes)
set "CWPROOT_MAKE=%CWPROOT_WIN%"
if "%CWPROOT_MAKE:~1,1%"==":" set "CWPROOT_MAKE=%CWPROOT_MAKE:~2%"
set "CWPROOT_MAKE=%CWPROOT_MAKE:\=/%"
set "CWPROOT=%CWPROOT_MAKE%"

echo ========================================
echo Building libsu
echo ========================================
echo.

REM Check if libcwp and libpar exist (dependencies)
echo Checking dependencies...
if exist "%CWPROOT_WIN%\lib\libcwp.lib" (
    echo [OK] libcwp.lib found
) else (
    echo [WARNING] libcwp.lib not found - libsu requires libcwp
)

if exist "%CWPROOT_WIN%\lib\libpar.lib" (
    echo [OK] libpar.lib found
) else (
    echo [WARNING] libpar.lib not found - libsu requires libpar
)

echo.

REM Build SU library
echo Building SU library (libsu)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\su\lib\Makefile" (
    cd /d "%CWPROOT_WIN%\src\su\lib"
    echo Current directory: %CD%
    echo.
    echo Running: make INSTALL
    echo.
    make INSTALL
    if errorlevel 1 (
        echo.
        echo [FAIL] libsu build failed
        cd /d "%CWPROOT_WIN%"
        exit /b 1
    ) else (
        echo.
        echo [PASS] libsu built successfully
        echo.
        if exist "%CWPROOT_WIN%\lib\libsu.lib" (
            echo [OK] libsu.lib created in lib directory
            for %%A in ("%CWPROOT_WIN%\lib\libsu.lib") do echo   Size: %%~zA bytes
        ) else (
            echo [WARNING] libsu.lib not found in lib directory
        )
    )
) else (
    echo [ERROR] Makefile not found at: %CWPROOT_WIN%\src\su\lib\Makefile
    exit /b 1
)

cd /d "%CWPROOT_WIN%"

echo.
echo ========================================
echo Test Build Complete
echo ========================================
echo.

endlocal

