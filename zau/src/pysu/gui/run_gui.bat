@echo off
REM Run SU Flow GUI with virtual environment
REM Requires Python 3 with PyQt6 installed in venv

echo Starting SU Flow GUI...

REM Get script directory
cd /d "%~dp0"
cd ..

REM Activate venv and run GUI
call venv\Scripts\activate.bat
python gui\suflow_gui.py

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error running GUI. Check that PyQt6 is installed.
    echo Run: pip install PyQt6
    pause
)
