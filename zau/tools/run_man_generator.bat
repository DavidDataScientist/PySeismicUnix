@echo off
REM Run the man page generator script

setlocal

set "SCRIPT_DIR=%~dp0"
REM Change to project root (2 levels up from zau/tools/)
cd /d "%SCRIPT_DIR%..\.."

REM Check for Python
where python >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Python not found
    echo Please install Python 3.x
    exit /b 1
)

echo Running man page generator...
echo.

python "%SCRIPT_DIR%generate_man_pages.py"

if errorlevel 1 (
    echo.
    echo [ERROR] Generation failed
    exit /b 1
)

echo.
echo Done!
endlocal

