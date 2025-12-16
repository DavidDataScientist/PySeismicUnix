@echo off
REM Setup script for CWP/SU Flow GUI
REM Creates Python virtual environment and installs dependencies
REM This script is location-independent and works from any directory

setlocal EnableDelayedExpansion

echo ========================================
echo CWP/SU Flow GUI Setup
echo ========================================
echo.

REM Get script directory (location-independent)
set "SCRIPT_DIR=%~dp0"

REM Check for Python
where python >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Python not found in PATH
    echo Please install Python 3.8 or higher from https://www.python.org/
    echo Minimum required: Python 3.8
    echo Recommended: Python 3.10 or higher
    exit /b 1
)

REM Check Python version (must be 3.8 or higher)
echo Checking Python version...
for /f "tokens=2" %%v in ('python --version 2^>^&1') do set PYTHON_VERSION=%%v
for /f "tokens=1,2 delims=." %%a in ("!PYTHON_VERSION!") do (
    set PYTHON_MAJOR=%%a
    set PYTHON_MINOR=%%b
)

REM Verify Python 3.8 or higher
if not "!PYTHON_MAJOR!"=="3" (
    echo [ERROR] Python 3.x required, found Python !PYTHON_VERSION!
    echo Please install Python 3.8 or higher from https://www.python.org/
    exit /b 1
)

REM Check minor version (must be >= 8)
set /a PYTHON_MINOR_INT=!PYTHON_MINOR!
if !PYTHON_MINOR_INT! LSS 8 (
    echo [ERROR] Python 3.8 or higher required, found Python !PYTHON_VERSION!
    echo Please install Python 3.8 or higher from https://www.python.org/
    exit /b 1
)

REM Show Python version
echo Found Python: !PYTHON_VERSION!
echo.

REM Create virtual environment if it doesn't exist (using %~dp0 for location independence)
if not exist "!SCRIPT_DIR!venv" (
    echo Creating virtual environment...
    python -m venv "!SCRIPT_DIR!venv"
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
call "!SCRIPT_DIR!venv\Scripts\activate.bat"

echo.
echo Installing dependencies...
pip install --upgrade pip >nul
pip install -r "!SCRIPT_DIR!requirements.txt"

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
echo   1. cd "!SCRIPT_DIR!"
echo   2. venv\Scripts\activate
echo   3. python gui\suflow_gui.py
echo.
echo Or just run: run_gui.bat
echo ========================================

endlocal

