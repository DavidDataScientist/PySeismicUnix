@echo off
REM Build all Fortran components
REM This script builds Cshot, Cwell, Triso, Vplusz, and Vzest Fortran programs
REM Requires: Intel oneAPI Fortran Compiler (ifx) and MSVC

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
set "FORTRAN_DIR=%CWPROOT_WIN%\src\Fortran"

echo ========================================
echo Building Fortran Components
echo ========================================
echo.
echo CWPROOT = %CWPROOT_WIN%
echo.

REM Find Visual Studio 2022
set "VS2022_PATH="
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
    goto :found_vs_fortran
)
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Professional"
    goto :found_vs_fortran
)
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise"
    goto :found_vs_fortran
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community"
    goto :found_vs_fortran
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional"
    goto :found_vs_fortran
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise"
    goto :found_vs_fortran
)
REM Also check for BuildTools (used by some installations)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\BuildTools"
    goto :found_vs_fortran
)

echo "ERROR: Visual Studio 2022 not found!"
exit /b 1

:found_vs_fortran
echo Found Visual Studio 2022 at: %VS2022_PATH%
echo.

REM Initialize VS 2022 environment
echo Initializing Visual Studio 2022 Developer Command Prompt (x64)...
set "VCVARS=%VS2022_PATH%\VC\Auxiliary\Build\vcvarsall.bat"
call "%VCVARS%" x64
if errorlevel 1 (
    echo "ERROR: Failed to initialize VS 2022 environment"
    echo "Please ensure Visual Studio 2022 is installed"
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
    
    REM Extract MSVC library path from LIB environment variable for vcruntime140.lib and ucrt.lib
    REM These are needed for mainCRTStartup and _fltused symbols
    set "MSVC_LIB_PATH="
    if defined LIB (
        REM Search LIB paths for vcruntime140.lib to find MSVC library directory
        for %%p in (%LIB%) do (
            if exist "%%p\vcruntime140.lib" (
                set "MSVC_LIB_PATH=%%p"
                goto :found_msvc_lib_path
            )
        )
    )
    
    REM If not in LIB, search common BuildTools locations
    if not defined MSVC_LIB_PATH (
        if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC" (
            for /d %%d in ("C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\*\lib\x64") do (
                if exist "%%d\vcruntime140.lib" (
                    set "MSVC_LIB_PATH=%%d"
                    goto :found_msvc_lib_path
                )
            )
        )
    )
    
    :found_msvc_lib_path
    if defined MSVC_LIB_PATH (
        echo "  [OK] Found MSVC library path: %MSVC_LIB_PATH%"
        REM Export MSVC_LIB_PATH as environment variable for Makefiles to use
        set "MSVC_LIB_PATH=%MSVC_LIB_PATH%"
    ) else (
        echo "  [WARNING] Could not find MSVC library path for vcruntime140.lib"
        echo "  Linker may fail to find modern C runtime libraries"
    )
    
    REM Find and copy msvcrt.lib to local lib directory for linking
    set "MSVCRT_LIB_FOUND=0"
    if defined LIB (
        REM Search LIB paths for msvcrt.lib
        for %%p in (%LIB%) do (
            if exist "%%p\msvcrt.lib" (
                set "MSVCRT_LIB_SOURCE=%%p\msvcrt.lib"
                set "MSVCRT_LIB_FOUND=1"
                goto :found_msvcrt
            )
        )
    )
    
    REM If not in LIB, search common BuildTools locations
    if "%MSVCRT_LIB_FOUND%"=="0" (
        if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC" (
            for /d %%d in ("C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\*\lib\x64") do (
                if exist "%%d\msvcrt.lib" (
                    set "MSVCRT_LIB_SOURCE=%%d\msvcrt.lib"
                    set "MSVCRT_LIB_FOUND=1"
                    goto :found_msvcrt
                )
            )
        )
    )
    
    :found_msvcrt
    if "%MSVCRT_LIB_FOUND%"=="1" (
        REM Ensure local lib directory exists
        if not exist "%CWPROOT_WIN%\lib" mkdir "%CWPROOT_WIN%\lib"
        
        REM Copy msvcrt.lib to local lib directory if not already there or if source is newer
        if not exist "%CWPROOT_WIN%\lib\msvcrt.lib" (
            copy /Y "%MSVCRT_LIB_SOURCE%" "%CWPROOT_WIN%\lib\msvcrt.lib" >nul 2>&1
            if errorlevel 1 (
                echo "  [WARNING] Failed to copy msvcrt.lib to local lib directory"
            ) else (
                echo "  [OK] Copied msvcrt.lib to local lib directory"
            )
        ) else (
            REM Check if source is newer (optional - could skip this check for simplicity)
            echo "  [OK] msvcrt.lib already exists in local lib directory"
        )
        
        REM Also copy oldnames.lib if found (needed for some MSVC linking scenarios)
        set "OLDNAMES_LIB_FOUND=0"
        if defined LIB (
            for %%p in (%LIB%) do (
                if exist "%%p\oldnames.lib" (
                    set "OLDNAMES_LIB_SOURCE=%%p\oldnames.lib"
                    set "OLDNAMES_LIB_FOUND=1"
                    goto :found_oldnames
                )
            )
        )
        :found_oldnames
        if "%OLDNAMES_LIB_FOUND%"=="1" (
            if not exist "%CWPROOT_WIN%\lib\oldnames.lib" (
                copy /Y "%OLDNAMES_LIB_SOURCE%" "%CWPROOT_WIN%\lib\oldnames.lib" >nul 2>&1
                if not errorlevel 1 (
                    echo "  [OK] Copied oldnames.lib to local lib directory"
                )
            )
        )
    ) else (
        echo "  [WARNING] msvcrt.lib not found - linker may fail"
        echo "  Searched in LIB paths and BuildTools locations"
    )
) else (
    echo "ERROR: Visual Studio 2022 not found"
    echo "Please ensure Visual Studio 2022 is installed"
    exit /b 1
)
echo.

REM Initialize Intel oneAPI environment
echo [2/3] Initializing Intel oneAPI environment...
set "INTEL_LIB_PATH="
if exist "C:\Tools\Intel\oneAPI\setvars.bat" (
    call "C:\Tools\Intel\oneAPI\setvars.bat" >nul 2>&1
    if errorlevel 1 (
        echo "  [WARNING] setvars.bat returned error, but continuing..."
    ) else (
        echo "  [OK] Intel oneAPI environment initialized"
    )
    REM Ensure ifx is in PATH by adding it explicitly
    set "PATH=%PATH%;C:\Tools\Intel\oneAPI\compiler\latest\bin"
    REM Set Intel library path for copying runtime libraries
    if exist "C:\Tools\Intel\oneAPI\compiler\latest\lib\intel64" (
        set "INTEL_LIB_PATH=C:\Tools\Intel\oneAPI\compiler\latest\lib\intel64"
    ) else if exist "C:\Tools\Intel\oneAPI\compiler\latest\lib" (
        set "INTEL_LIB_PATH=C:\Tools\Intel\oneAPI\compiler\latest\lib"
    )
) else if exist "C:\Program Files (x86)\Intel\oneAPI\setvars.bat" (
    call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat" >nul 2>&1
    if errorlevel 1 (
        echo "  [WARNING] setvars.bat returned error, but continuing..."
    ) else (
        echo "  [OK] Intel oneAPI environment initialized"
    )
    REM Ensure ifx is in PATH by adding it explicitly
    set "PATH=%PATH%;C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin"
    REM Set Intel library path for copying runtime libraries
    if exist "C:\Program Files (x86)\Intel\oneAPI\compiler\latest\lib\intel64" (
        set "INTEL_LIB_PATH=C:\Program Files (x86)\Intel\oneAPI\compiler\latest\lib\intel64"
    ) else if exist "C:\Program Files (x86)\Intel\oneAPI\compiler\latest\lib" (
        set "INTEL_LIB_PATH=C:\Program Files (x86)\Intel\oneAPI\compiler\latest\lib"
    )
) else (
    echo "  [WARNING] Intel oneAPI setvars.bat not found"
    echo "  Fortran compiler may not be in PATH"
    REM Try to add path directly anyway
    if exist "C:\Tools\Intel\oneAPI\compiler\latest\bin\ifx.exe" (
        set "PATH=%PATH%;C:\Tools\Intel\oneAPI\compiler\latest\bin"
        echo "  [INFO] Added Intel oneAPI compiler path to PATH"
        REM Set Intel library path for copying runtime libraries
        if exist "C:\Tools\Intel\oneAPI\compiler\latest\lib\intel64" (
            set "INTEL_LIB_PATH=C:\Tools\Intel\oneAPI\compiler\latest\lib\intel64"
        ) else if exist "C:\Tools\Intel\oneAPI\compiler\latest\lib" (
            set "INTEL_LIB_PATH=C:\Tools\Intel\oneAPI\compiler\latest\lib"
        )
    )
)

REM Copy Intel Fortran runtime libraries to local lib directory
if defined INTEL_LIB_PATH (
    if not exist "%CWPROOT_WIN%\lib" mkdir "%CWPROOT_WIN%\lib"
    
    REM Copy ifconsol.lib (Intel Fortran console library) - needed for console programs
    set "IFCONSOL_FOUND=0"
    if exist "%INTEL_LIB_PATH%\ifconsol.lib" (
        set "IFCONSOL_SOURCE=%INTEL_LIB_PATH%\ifconsol.lib"
        set "IFCONSOL_FOUND=1"
    ) else (
        REM Try searching in intel64 subdirectory
        if exist "%INTEL_LIB_PATH%\intel64\ifconsol.lib" (
            set "IFCONSOL_SOURCE=%INTEL_LIB_PATH%\intel64\ifconsol.lib"
            set "IFCONSOL_FOUND=1"
        )
    )
    
    if "%IFCONSOL_FOUND%"=="1" (
        if not exist "%CWPROOT_WIN%\lib\ifconsol.lib" (
            copy /Y "%IFCONSOL_SOURCE%" "%CWPROOT_WIN%\lib\ifconsol.lib" >nul 2>&1
            if errorlevel 1 (
                echo "  [WARNING] Failed to copy ifconsol.lib"
            ) else (
                echo "  [OK] Copied ifconsol.lib to local lib directory"
            )
        ) else (
            echo "  [OK] ifconsol.lib already exists in local lib directory"
        )
    ) else (
        echo "  [WARNING] ifconsol.lib not found at %INTEL_LIB_PATH%"
        echo "  Linker may fail - ensure Intel oneAPI is properly installed"
    )
    
    REM Copy other commonly needed Intel Fortran libraries
    REM Note: We use DLL versions (libifcoremd.lib) not static (libifcoremt.lib)
    REM But we may need libmmt.lib or libmd.lib for math functions
    for %%l in (libifport.lib libmmt.lib libmd.lib) do (
        set "LIB_FOUND=0"
        if exist "%INTEL_LIB_PATH%\%%l" (
            set "LIB_SOURCE=%INTEL_LIB_PATH%\%%l"
            set "LIB_FOUND=1"
        ) else if exist "%INTEL_LIB_PATH%\intel64\%%l" (
            set "LIB_SOURCE=%INTEL_LIB_PATH%\intel64\%%l"
            set "LIB_FOUND=1"
        )
        if "!LIB_FOUND!"=="1" (
            if not exist "%CWPROOT_WIN%\lib\%%l" (
                copy /Y "!LIB_SOURCE!" "%CWPROOT_WIN%\lib\%%l" >nul 2>&1
            )
        )
    )
)
echo.

REM Verify Fortran compiler
echo [3/3] Verifying Fortran compiler...
where ifx.exe >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo "  [OK] Intel ifx compiler found"
    where ifx.exe
) else (
    where ifort.exe >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        echo "  [OK] Intel ifort compiler found (legacy)"
        where ifort.exe
    ) else (
        echo "  [ERROR] No Intel Fortran compiler found in PATH"
        echo "  Please ensure Intel oneAPI is installed and setvars.bat was called"
        exit /b 1
    )
)
echo.

REM Add GNU Make to PATH
set "PATH=%MAKEPATH%;%PATH%"

REM Verify GNU Make
where make.exe >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR: GNU Make not found in PATH"
    echo "Please ensure make.exe is available at %MAKEPATH%"
    exit /b 1
)

REM ========================================
REM Create Required Directories
REM ========================================
echo Creating required directories...
if not exist "%CWPROOT_WIN%\bin" mkdir "%CWPROOT_WIN%\bin"
if not exist "%CWPROOT_WIN%\lib" mkdir "%CWPROOT_WIN%\lib"
if not exist "%CWPROOT_WIN%\include" mkdir "%CWPROOT_WIN%\include"
echo   Created: bin, lib, include
echo.

REM Counters
set /a TOTAL_DIRS=0
set /a BUILT_DIRS=0
set /a FAILED_DIRS=0

REM List of Fortran components to build
REM Note: These are the 5 components requested in the todo list
REM Additional components (AzimVelan, Cxzco, Cxzcs, Raytrace3D) can be added later
set "DIRS=Cshot Cwell Triso Vplusz Vzest"

echo ========================================
echo Starting Build Process
echo ========================================
echo.

for %%D in (%DIRS%) do (
    set /a TOTAL_DIRS+=1
    echo.
    echo ----------------------------------------
    echo Building: %%D
    echo ----------------------------------------
    
    if exist "%FORTRAN_DIR%\%%D\Makefile" (
        cd /d "%FORTRAN_DIR%\%%D"
        
        echo Building library and programs...
        REM Run make with Windows settings (use CWPROOT_MAKE for Make)
        make CWPROOT=%CWPROOT_MAKE% INSTALL 2>&1
        
        if !ERRORLEVEL! EQU 0 (
            echo "[PASS] %%D built successfully"
            set /a BUILT_DIRS+=1
            
            REM List built executables
            if exist "%CWPROOT_WIN%\bin\*.exe" (
                echo "  Built executables:"
                for %%F in ("%CWPROOT_WIN%\bin\*.exe") do (
                    set "FNAME=%%~nF"
                    echo | findstr /C:"%%D" >nul 2>&1
                    if !ERRORLEVEL! EQU 0 (
                        echo "    - %%~nF.exe"
                    )
                )
            )
        ) else (
            echo "[FAIL] %%D build failed"
            set /a FAILED_DIRS+=1
            echo "  Check the error messages above for details"
        )
    ) else (
        echo "[SKIP] No Makefile found in %%D"
        set /a FAILED_DIRS+=1
    )
)

echo.
echo ========================================
echo Build Summary
echo ========================================
echo Total components: %TOTAL_DIRS%
echo Successfully built: %BUILT_DIRS%
echo Failed/Skipped: %FAILED_DIRS%
echo ========================================
echo.

REM List all Fortran executables in bin directory
echo Checking for built Fortran executables...
set "FOUND_ANY=0"
set "EXE_COUNT=0"

REM Check for Cshot executables
if exist "%CWPROOT_WIN%\bin\cshot1.exe" (
    if !FOUND_ANY! EQU 0 (
        echo "Found Fortran executables:"
        set "FOUND_ANY=1"
    )
    echo "  - cshot1.exe"
    set /a EXE_COUNT+=1
)
if exist "%CWPROOT_WIN%\bin\cshot2.exe" (
    if !FOUND_ANY! EQU 0 (
        echo "Found Fortran executables:"
        set "FOUND_ANY=1"
    )
    echo "  - cshot2.exe"
    set /a EXE_COUNT+=1
)
if exist "%CWPROOT_WIN%\bin\cshotplot.exe" (
    if !FOUND_ANY! EQU 0 (
        echo "Found Fortran executables:"
        set "FOUND_ANY=1"
    )
    echo "  - cshotplot.exe"
    set /a EXE_COUNT+=1
)

REM Check for Cwell executables
if exist "%CWPROOT_WIN%\bin\cwell.exe" (
    if !FOUND_ANY! EQU 0 (
        echo "Found Fortran executables:"
        set "FOUND_ANY=1"
    )
    echo "  - cwell.exe"
    set /a EXE_COUNT+=1
)

REM Check for Triso executables
if exist "%CWPROOT_WIN%\bin\triso.exe" (
    if !FOUND_ANY! EQU 0 (
        echo "Found Fortran executables:"
        set "FOUND_ANY=1"
    )
    echo "  - triso.exe"
    set /a EXE_COUNT+=1
)
if exist "%CWPROOT_WIN%\bin\triview.exe" (
    if !FOUND_ANY! EQU 0 (
        echo "Found Fortran executables:"
        set "FOUND_ANY=1"
    )
    echo "  - triview.exe"
    set /a EXE_COUNT+=1
)
if exist "%CWPROOT_WIN%\bin\trisoplot.exe" (
    if !FOUND_ANY! EQU 0 (
        echo "Found Fortran executables:"
        set "FOUND_ANY=1"
    )
    echo "  - trisoplot.exe"
    set /a EXE_COUNT+=1
)

REM Check for Vplusz executables
if exist "%CWPROOT_WIN%\bin\vplusz.exe" (
    if !FOUND_ANY! EQU 0 (
        echo "Found Fortran executables:"
        set "FOUND_ANY=1"
    )
    echo "  - vplusz.exe"
    set /a EXE_COUNT+=1
)

REM Check for Vzest executables
if exist "%CWPROOT_WIN%\bin\vzest.exe" (
    if !FOUND_ANY! EQU 0 (
        echo "Found Fortran executables:"
        set "FOUND_ANY=1"
    )
    echo "  - vzest.exe"
    set /a EXE_COUNT+=1
)

if !FOUND_ANY! EQU 0 (
    echo "  No Fortran executables found in %CWPROOT_WIN%\bin"
) else (
    echo "  Total executables found: !EXE_COUNT!"
)

echo.
echo ========================================
echo Build Complete
echo ========================================

cd /d "%CWPROOT_WIN%"

endlocal
