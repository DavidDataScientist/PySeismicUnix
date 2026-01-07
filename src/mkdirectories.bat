@echo off
REM Windows version of mkdirectories.sh
REM Make directories for CWP/SU codes installation

if "%CWPROOT%"=="" (
    echo ERROR: CWPROOT not set!
    exit /b 1
)

set "B=%CWPROOT%\bin"
set "I=%CWPROOT%\include"
set "L=%CWPROOT%\lib"

echo Making necessary directories...

REM Create root directory if it doesn't exist
if not exist "%CWPROOT%" mkdir "%CWPROOT%" 2>nul

REM Create main directories
if not exist "%B%" mkdir "%B%" 2>nul
if not exist "%L%" mkdir "%L%" 2>nul
if not exist "%I%" mkdir "%I%" 2>nul

REM Create subdirectories
if not exist "%I%\Xtcwp" mkdir "%I%\Xtcwp" 2>nul
if not exist "%L%\X11" mkdir "%L%\X11" 2>nul
if not exist "%L%\X11\app-defaults" mkdir "%L%\X11\app-defaults" 2>nul
if not exist "%I%\Xmcwp" mkdir "%I%\Xmcwp" 2>nul
if not exist "%I%\Triangles" mkdir "%I%\Triangles" 2>nul
if not exist "%I%\Wpc" mkdir "%I%\Wpc" 2>nul
if not exist "%I%\MGL" mkdir "%I%\MGL" 2>nul
if not exist "%I%\Reflect" mkdir "%I%\Reflect" 2>nul

echo Directories created.

exit /b 0

