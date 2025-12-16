@echo off
REM Setup script for CWP/SU Flow GUI
REM Creates Python virtual environment and installs dependencies

echo ========================================
echo CWP/SU Flow GUI Setup
echo ========================================
echo.

REM Check for Python
where python >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Python not found in PATH
    echo Please install Python 3.x from https://www.python.org/
    exit /b 1
)

REM Show Python version
echo Found Python:
python --version
echo.

REM Get script directory
set "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"

REM Create virtual environment if it doesn't exist
if not exist "venv" (
    echo Creating virtual environment...
    python -m venv venv
    if errorlevel 1 (
        echo [ERROR] Failed to create virtual environment
        exit /b 1
    )
    echo Virtual environment created.
) else (
    echo Virtual environment already exists.
)

echo.
echo Activating virtual environment...
call venv\Scripts\activate.bat

echo.
echo Installing dependencies...
pip install --upgrade pip >nul
pip install -r requirements.txt

if errorlevel 1 (
    echo [ERROR] Failed to install dependencies
    exit /b 1
)

echo.
echo ========================================
echo Setup Complete!
echo ========================================
echo.
echo To run the GUI:
echo   1. cd %SCRIPT_DIR%
echo   2. venv\Scripts\activate
echo   3. python gui\suflow_gui.py
echo.
echo Or just run: run_gui.bat
echo ========================================

