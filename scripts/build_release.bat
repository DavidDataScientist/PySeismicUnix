@echo off
REM Create CWP/SU Windows Distribution Package
REM This script creates a zip archive with all built binaries, libraries, and documentation

setlocal EnableDelayedExpansion

REM Change to workspace root directory
cd /d "%~dp0.."

REM Set CWPROOT: use environment variable if set, otherwise use current working directory
if not defined CWPROOT (
    set "CWPROOT=%CD%"
)

REM Ensure CWPROOT_WIN has a drive letter for Windows commands (cd, if exist, etc.)
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

set "DIST_NAME=cwpsu-windows-x64"
set "DIST_DIR=%CWPROOT_WIN%\dist\%DIST_NAME%"

echo ========================================
echo Creating CWP/SU Windows Distribution
echo ========================================
echo(
echo CWPROOT = %CWPROOT_WIN%
echo(

REM Build everything first (if not already built)
echo ========================================
echo Building all components...
echo ========================================
echo(
call "%~dp0build.bat"
if errorlevel 1 (
    echo(
    echo [ERROR] Build failed. Cannot create distribution package.
    echo Please fix build errors and try again.
    exit /b 1
)
echo(
echo ========================================
echo Packaging distribution...
echo ========================================
echo(

REM Verify source directories exist (bin, lib, include should exist from build)
echo Verifying source directories...
if not exist "%CWPROOT_WIN%\bin" (
    echo "WARNING: bin directory not found - no executables to copy"
    mkdir "%CWPROOT_WIN%\bin" 2>nul
)
if not exist "%CWPROOT_WIN%\lib" (
    echo "WARNING: lib directory not found - no libraries to copy"
    mkdir "%CWPROOT_WIN%\lib" 2>nul
)
if not exist "%CWPROOT_WIN%\include" (
    echo "WARNING: include directory not found - no headers to copy"
    mkdir "%CWPROOT_WIN%\include" 2>nul
)
echo(

REM Create distribution directory structure
echo Creating distribution directory structure...
if exist "%CWPROOT_WIN%\dist" rmdir /S /Q "%CWPROOT_WIN%\dist"
mkdir "%DIST_DIR%"
mkdir "%DIST_DIR%\bin"
mkdir "%DIST_DIR%\lib"
mkdir "%DIST_DIR%\include"
mkdir "%DIST_DIR%\doc"
mkdir "%DIST_DIR%\samples"
mkdir "%DIST_DIR%\gui"
mkdir "%DIST_DIR%\man"

REM Copy executables
echo Copying executables...
copy "%CWPROOT_WIN%\bin\*.exe" "%DIST_DIR%\bin\" >nul
echo   Copied executables to bin\

REM Copy libraries
echo Copying libraries...
copy "%CWPROOT_WIN%\lib\*.lib" "%DIST_DIR%\lib\" >nul
echo   Copied libraries to lib\

REM Copy essential headers
echo Copying headers...
xcopy "%CWPROOT_WIN%\include\*.h" "%DIST_DIR%\include\" /S /Q >nul
echo   Copied headers to include\

REM Copy documentation
echo Copying documentation...
copy "%CWPROOT_WIN%\documents\USER_GUIDE.md" "%DIST_DIR%\doc\" >nul
copy "%CWPROOT_WIN%\documents\CHANGES.md" "%DIST_DIR%\doc\" >nul
copy "%CWPROOT_WIN%\README" "%DIST_DIR%\doc\README_ORIGINAL.txt" >nul 2>nul
echo   Copied documentation to doc\

REM Copy sample data
echo Copying sample data...
if exist "%CWPROOT_WIN%\src\su\tutorial\*.su" (
    copy "%CWPROOT_WIN%\src\su\tutorial\*.su" "%DIST_DIR%\samples\" >nul 2>nul
)
if exist "%CWPROOT_WIN%\src\su\examples\*.su" (
    copy "%CWPROOT_WIN%\src\su\examples\*.su" "%DIST_DIR%\samples\" >nul 2>nul
)
echo   Copied sample data to samples\

REM Copy man pages
echo Copying man pages...
if exist "%CWPROOT_WIN%\man" (
    xcopy "%CWPROOT_WIN%\man" "%DIST_DIR%\man\" /E /I /Q >nul
    echo   Copied man pages to man\
) else (
    echo   No man pages found (run zau\tools\run_man_generator.bat first)
)

REM Copy PyQt GUI
echo Copying SU Flow GUI...
copy "%CWPROOT_WIN%\zau\src\pysu\gui\suflow_gui.py" "%DIST_DIR%\gui\" >nul
copy "%CWPROOT_WIN%\zau\src\pysu\requirements.txt" "%DIST_DIR%\gui\" >nul
copy "%CWPROOT_WIN%\zau\src\pysu\setup_gui.bat" "%DIST_DIR%\gui\" >nul
copy "%CWPROOT_WIN%\zau\src\pysu\run_gui.bat" "%DIST_DIR%\gui\" >nul
echo   Copied GUI to gui\

REM Create README for distribution
echo Creating distribution README...
(
echo CWP/SU for Windows x64
echo ======================
echo(
echo This is a Windows port of CWP/SU ^(Seismic Unix^) built with
echo Visual Studio 2022 and GNU Make.
echo(
echo Contents:
echo   bin\      - Executable programs ^(312 tools^)
echo   lib\      - Static libraries ^(9 libraries^)
echo   include\  - Header files
echo   doc\      - Documentation
echo   man\      - Unix man pages
echo   samples\  - Sample SU data files
echo   gui\      - PyQt6 SU Flow GUI
echo(
echo Quick Start:
echo   1. Add bin\ to your PATH
echo   2. Test: bin\surange.exe ^< samples\data.su
echo(
echo For pipelines, use suflow.exe:
echo   bin\suflow.exe "suplane ^| sugain agc=1 ^| sufilter"
echo(
echo GUI Setup ^(requires Python 3.x^):
echo   1. cd gui
echo   2. setup_gui.bat   ^(creates venv, installs PyQt6^)
echo   3. run_gui.bat     ^(launches the GUI^)
echo(
echo Documentation:
echo   doc\USER_GUIDE.md        - How to use SU programs
echo   doc\CHANGES.md           - Port summary and fixes
echo   doc\PYTHON-GUIDE.md      - Python plugin integration guide
echo(
echo Built: %DATE%
) > "%DIST_DIR%\README.txt"

REM Count files
echo(
echo Counting distribution contents...
for /f %%a in ('dir /b /a-d "%DIST_DIR%\bin\*.exe" 2^>nul ^| find /c /v ""') do set EXE_COUNT=%%a
for /f %%a in ('dir /b /a-d "%DIST_DIR%\lib\*.lib" 2^>nul ^| find /c /v ""') do set LIB_COUNT=%%a
set MAN_COUNT=0
for /f %%a in ('dir /b /a-d /s "%DIST_DIR%\man\*.1" 2^>nul ^| find /c /v ""') do set MAN_COUNT=%%a

echo(
echo ========================================
echo Distribution Summary
echo ========================================
echo   Executables: %EXE_COUNT%
echo   Libraries:   %LIB_COUNT%
echo   Man pages:   %MAN_COUNT%
echo   Location:    %DIST_DIR%
echo ========================================
echo(

REM Create zip archive if PowerShell available
echo Creating ZIP archive...
powershell -Command "Compress-Archive -Path '%DIST_DIR%' -DestinationPath '%CWPROOT_WIN%\dist\%DIST_NAME%.zip' -Force"

if exist "%CWPROOT_WIN%\dist\%DIST_NAME%.zip" (
    echo(
    echo [SUCCESS] Distribution package created:
    echo   %CWPROOT_WIN%\dist\%DIST_NAME%.zip
    echo(
    for %%A in ("%CWPROOT_WIN%\dist\%DIST_NAME%.zip") do echo   Size: %%~zA bytes
) else (
    echo(
    echo [WARNING] ZIP creation failed. Distribution folder available at:
    echo   %DIST_DIR%
)

echo(
echo Done!
endlocal

