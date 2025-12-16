@echo off
REM Run CWP/SU Flow GUI

set "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"

REM Check if venv exists
if not exist "venv\Scripts\python.exe" (
    echo Virtual environment not found. Running setup...
    call setup_gui.bat
    if errorlevel 1 exit /b 1
)

REM Activate and run
call venv\Scripts\activate.bat
python gui\suflow_gui.py

