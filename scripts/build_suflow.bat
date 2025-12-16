@echo off
REM Build suflow.exe - Windows binary pipe wrapper
REM This script builds suflow.exe using MSVC compiler
REM
REM Usage: build_suflow.bat
REM
REM Builds:
REM   - suflow.exe: Windows binary pipe wrapper for SU pipelines
REM
REM Requirements:
REM   - Visual Studio 2022 (Community, Professional, or Enterprise)
REM   - MSVC compiler (cl.exe)

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
echo Building suflow.exe
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

REM Build suflow.exe
echo ========================================
echo Building suflow.exe
echo ========================================
echo.

cd /d "%CWPROOT_WIN%\zau\src\suflow"

if not exist "suflow.c" (
    echo "ERROR: suflow.c not found in %CWPROOT_WIN%\zau\src\suflow"
    exit /b 1
)

echo Compiling suflow.c...
cl /nologo /W3 /O2 /MD /Fe:suflow.exe suflow.c
if errorlevel 1 (
    echo "[FAIL] Build failed"
    cd /d "%CWPROOT_WIN%"
    exit /b 1
)

echo "[PASS] suflow.exe built successfully"
echo.

REM Install to bin directory
echo Installing suflow.exe to bin directory...
if not exist "%CWPROOT_WIN%\bin" mkdir "%CWPROOT_WIN%\bin"
copy /Y suflow.exe "%CWPROOT_WIN%\bin\suflow.exe" >nul
if errorlevel 1 (
    echo "[WARNING] Failed to copy suflow.exe to bin directory"
    cd /d "%CWPROOT_WIN%"
    exit /b 1
)

echo "[OK] suflow.exe installed to bin\suflow.exe"
echo.

REM Clean up build artifacts in source directory
echo Cleaning build artifacts...
if exist suflow.obj del /F /Q suflow.obj 2>nul
if exist suflow.pdb del /F /Q suflow.pdb 2>nul
if exist suflow.ilk del /F /Q suflow.ilk 2>nul
if exist suflow.exp del /F /Q suflow.exp 2>nul

cd /d "%CWPROOT_WIN%"

echo ========================================
echo Build Complete
echo ========================================
echo.

endlocal

