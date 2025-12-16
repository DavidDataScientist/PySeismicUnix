@echo off
REM Run CWP/SU Flow GUI
REM This script is location-independent and works from any directory

setlocal EnableDelayedExpansion

REM Get script directory (location-independent)
set "SCRIPT_DIR=%~dp0"

REM Check if venv exists (using %~dp0 for location independence)
if not exist "!SCRIPT_DIR!venv\Scripts\python.exe" (
    echo Virtual environment not found. Running setup...
    call "!SCRIPT_DIR!setup_gui.bat"
    if errorlevel 1 exit /b 1
)

REM Activate and run (using %~dp0 for location independence)
call "!SCRIPT_DIR!venv\Scripts\activate.bat"
python "!SCRIPT_DIR!gui\suflow_gui.py"

endlocal

