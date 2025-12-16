@echo off
REM Wipe all build outputs (libraries, executables, and distribution)
REM This script removes all final build products from lib/, bin/, and dist/ directories
REM
REM WARNING: This permanently deletes all built libraries, executables, and distribution!

setlocal EnableDelayedExpansion

REM Change to workspace root directory
cd /d "%~dp0.."
set "CWPROOT=%CD%"
if "%CWPROOT:~1,1%"==":" set "CWPROOT=%CWPROOT:~2%"

echo ========================================
echo Wiping Build Outputs
echo ========================================
echo.
echo CWPROOT = %CWPROOT%
echo.

REM Confirm deletion
echo WARNING: This will permanently delete:
echo   - All libraries from lib/
echo   - All executables from bin/
echo   - Distribution directory dist/
echo.
set /p CONFIRM="Are you sure? (yes/no): "
if /i not "%CONFIRM%"=="yes" (
    echo Cancelled.
    exit /b 0
)

echo.
echo ========================================
echo Removing Libraries
echo ========================================
echo.

REM Remove libraries from lib/
if exist "%CWPROOT%\lib\*.lib" (
    echo Removing .lib files from lib/...
    del /F /Q "%CWPROOT%\lib\*.lib" 2>nul
    if errorlevel 1 (
        echo [WARNING] Some .lib files could not be deleted
    ) else (
        echo [OK] .lib files removed
    )
) else (
    echo [SKIP] No .lib files found in lib/
)

if exist "%CWPROOT%\lib\*.a" (
    echo Removing .a files from lib/...
    del /F /Q "%CWPROOT%\lib\*.a" 2>nul
    if errorlevel 1 (
        echo [WARNING] Some .a files could not be deleted
    ) else (
        echo [OK] .a files removed
    )
) else (
    echo [SKIP] No .a files found in lib/
)

echo.
echo ========================================
echo Removing Executables
echo ========================================
echo.

REM Remove executables from bin/
if exist "%CWPROOT%\bin\*.exe" (
    echo Removing .exe files from bin/...
    del /F /Q "%CWPROOT%\bin\*.exe" 2>nul
    if errorlevel 1 (
        echo [WARNING] Some .exe files could not be deleted
    ) else (
        echo [OK] .exe files removed
    )
) else (
    echo [SKIP] No .exe files found in bin/
)

echo.
echo ========================================
echo Removing Distribution
echo ========================================
echo.

if exist "%CWPROOT%\dist" (
    echo Removing dist/ directory...
    rmdir /S /Q "%CWPROOT%\dist" 2>nul
    if errorlevel 1 (
        echo [WARNING] dist/ directory could not be deleted (may be in use)
    ) else (
        echo [OK] dist/ directory removed
    )
) else (
    echo [SKIP] dist/ directory not found
)

REM Count remaining files
echo.
echo ========================================
echo Wipe Summary
echo ========================================
echo.

set "LIB_COUNT=0"
set "EXE_COUNT=0"

if exist "%CWPROOT%\lib\*.lib" (
    for %%f in ("%CWPROOT%\lib\*.lib") do set /a LIB_COUNT+=1
)
if exist "%CWPROOT%\lib\*.a" (
    for %%f in ("%CWPROOT%\lib\*.a") do set /a LIB_COUNT+=1
)
if exist "%CWPROOT%\bin\*.exe" (
    for %%f in ("%CWPROOT%\bin\*.exe") do set /a EXE_COUNT+=1
)

echo Remaining libraries: !LIB_COUNT!
echo Remaining executables: !EXE_COUNT!

if not "!LIB_COUNT!"=="0" goto :show_warning
if not "!EXE_COUNT!"=="0" goto :show_warning

echo.
echo [SUCCESS] All build outputs have been removed.
goto :summary_done

:show_warning
echo.
echo [WARNING] Some files could not be removed (may be in use).
echo           Close any programs using these files and try again.

:summary_done

echo.
echo ========================================
echo Wipe Complete
echo ========================================
echo.

endlocal

