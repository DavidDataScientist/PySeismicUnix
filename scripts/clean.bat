@echo off
REM Clean all build artifacts from the entire project
REM This script runs clean/remake targets across all directories

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

REM Create CWPROOT_MAKE without drive letter and with forward slashes for Make
set "CWPROOT_MAKE=%CWPROOT_WIN%"
if "%CWPROOT_MAKE:~1,1%"==":" set "CWPROOT_MAKE=%CWPROOT_MAKE:~2%"
set "CWPROOT_MAKE=%CWPROOT_MAKE:\=/%"
set "MAKEPATH=C:\Tools\gnu\bin"

echo ========================================
echo Cleaning ALL Build Artifacts
echo ========================================
echo.
echo CWPROOT = %CWPROOT_WIN%
echo.

REM Find Visual Studio 2022
set "VS2022_PATH="
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
    goto :found_vs_clean
)
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Professional"
    goto :found_vs_clean
)
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise"
    goto :found_vs_clean
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community"
    goto :found_vs_clean
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional"
    goto :found_vs_clean
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise"
    goto :found_vs_clean
)

echo "ERROR: Visual Studio 2022 not found!"
exit /b 1

:found_vs_clean
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

REM Add GNU Make to PATH
set "PATH=%MAKEPATH%;%PATH%"

REM Verify make is available
where make >nul 2>&1
if errorlevel 1 (
    echo "ERROR: GNU Make not found in %MAKEPATH%"
    exit /b 1
)

REM Set CWPROOT for Make (use forward slashes)
set "CWPROOT=%CWPROOT_MAKE%"

echo Cleaning libraries...
echo.

REM Clean CWP library
if exist "%CWPROOT_WIN%\src\cwp\lib\Makefile" (
    echo Cleaning cwp/lib...
    cd /d "%CWPROOT_WIN%\src\cwp\lib"
    make clean 2>&1
)

REM Clean PAR library
if exist "%CWPROOT_WIN%\src\par\lib\Makefile" (
    echo Cleaning par/lib...
    cd /d "%CWPROOT_WIN%\src\par\lib"
    make clean 2>&1
)

REM Clean SU library
if exist "%CWPROOT_WIN%\src\su\lib\Makefile" (
    echo Cleaning su/lib...
    cd /d "%CWPROOT_WIN%\src\su\lib"
    make clean 2>&1
)

REM Clean win32compat library if it exists
if exist "%CWPROOT_WIN%\src\win32_compat\Makefile" (
    echo Cleaning win32_compat...
    cd /d "%CWPROOT_WIN%\src\win32_compat"
    make clean 2>&1
)

echo.
echo Cleaning SU main programs...
echo.

REM List of SU main subdirectories
set "SUBDIRS=amplitudes attributes_parameter_estimation convolution_correlation data_compression data_conversion datuming decon_shaping dip_moveout filters headers interp_extrap migration_inversion multicomponent noise operations picking stacking statics stretching_moveout_resamp supromax synthetics_waveforms_testpatterns tapering transforms velocity_analysis well_logs windowing_sorting_muting"

for %%d in (%SUBDIRS%) do (
    if exist "%CWPROOT_WIN%\src\su\main\%%d\Makefile" (
        cd /d "%CWPROOT_WIN%\src\su\main\%%d"
        make clean 2>&1 >nul
    )
)

REM Clean other components
echo.
echo Cleaning other components...
echo.

REM Clean CWP main
if exist "%CWPROOT_WIN%\src\cwp\main\Makefile" (
    cd /d "%CWPROOT_WIN%\src\cwp\main"
    make clean 2>&1 >nul
)

REM Clean PAR main
if exist "%CWPROOT_WIN%\src\par\main\Makefile" (
    cd /d "%CWPROOT_WIN%\src\par\main"
    make clean 2>&1 >nul
)

REM Clean export files from bin directory (intermediate build artifacts)
echo.
echo Removing export files from bin directory...
if exist "%CWPROOT_WIN%\bin\*.exp" (
    del /F /Q "%CWPROOT_WIN%\bin\*.exp" 2>nul
)

REM Clean object files and export files from lib directory
echo Removing object files from lib directory...
if exist "%CWPROOT_WIN%\lib\*.obj" (
    del /F /Q "%CWPROOT_WIN%\lib\*.obj" 2>nul
)
if exist "%CWPROOT_WIN%\lib\*.exp" (
    del /F /Q "%CWPROOT_WIN%\lib\*.exp" 2>nul
)

echo.
echo ========================================
echo Clean Complete
echo ========================================

cd /d "%CWPROOT_WIN%"
endlocal
