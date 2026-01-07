@echo off
REM Build all libraries and SU main utilities (without Fortran)
REM This script builds all required libraries and SU programs using GNU Make and MSVC
REM
REM Usage: build.bat
REM
REM Builds:
REM   - Libraries: libcwp, libpar, libsu, libwin32compat, libcomp, libtriang, libtrielas, libtetra, libreflect
REM   - All SU main programs (amplitudes, filters, data_compression, etc.)
REM   - CWP main utilities (ctrlstrip, fcat, etc.)
REM   - Triangle utilities (gbbeam, trimodel, triray, etc.)
REM   - Elastic triangle utilities (elaray, elamodel, etc.)
REM   - Tetrahedral utilities (tetramod, sutetraray)
REM   - Reflection utilities (sureflpsvsh)
REM   - Third Party components (bison2su, etc.)
REM
REM Does NOT build:
REM   - Fortran components (use build_fortran.bat)
REM   - X11-dependent components (Xtcwp, Xmcwp, xplot, xtri, su/graphics/xplot)
REM     These require X11 windowing system which is not available on Windows

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
echo Building Libraries and SU Programs
echo ========================================
echo.
echo CWPROOT = %CWPROOT_WIN%
echo.

REM Find Visual Studio 2022
set "VS2022_PATH="
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
    goto :found_vs
)
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Professional"
    goto :found_vs
)
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise"
    goto :found_vs
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community"
    goto :found_vs
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional"
    goto :found_vs
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS2022_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise"
    goto :found_vs
)

echo "ERROR: Visual Studio 2022 not found!"
exit /b 1

:found_vs
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

REM ========================================
REM Create Required Directories
REM ========================================
echo Creating required directories...
if not exist "%CWPROOT_WIN%\bin" mkdir "%CWPROOT_WIN%\bin"
if not exist "%CWPROOT_WIN%\lib" mkdir "%CWPROOT_WIN%\lib"
if not exist "%CWPROOT_WIN%\include" mkdir "%CWPROOT_WIN%\include"
echo   Created: bin, lib, include
echo.

REM ========================================
REM Build Libraries
REM ========================================
echo.
echo ========================================
echo Building Libraries
echo ========================================
echo.

set "LIB_SUCCESS_COUNT=0"
set "LIB_FAIL_COUNT=0"
set "FAILED_LIBS="

REM Build CWP library
echo Building CWP library (libcwp)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\cwp\lib\Makefile" (
    cd /d "%CWPROOT_WIN%\src\cwp\lib"
    make INSTALL 2>&1
    if errorlevel 1 (
        echo [FAIL] libcwp
        set /a LIB_FAIL_COUNT+=1
        set "FAILED_LIBS=!FAILED_LIBS! libcwp"
    ) else (
        echo [PASS] libcwp
        set /a LIB_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] libcwp - Makefile not found
    set /a LIB_FAIL_COUNT+=1
)

REM Build PAR library
echo.
echo Building PAR library (libpar)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\par\lib\Makefile" (
    cd /d "%CWPROOT_WIN%\src\par\lib"
    make INSTALL 2>&1
    if errorlevel 1 (
        echo [FAIL] libpar
        set /a LIB_FAIL_COUNT+=1
        set "FAILED_LIBS=!FAILED_LIBS! libpar"
    ) else (
        echo [PASS] libpar
        set /a LIB_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] libpar - Makefile not found
    set /a LIB_FAIL_COUNT+=1
)

REM Build SU library
echo.
echo Building SU library (libsu)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\su\lib\Makefile" (
    cd /d "%CWPROOT_WIN%\src\su\lib"
    make INSTALL 2>&1
    if errorlevel 1 (
        echo [FAIL] libsu
        set /a LIB_FAIL_COUNT+=1
        set "FAILED_LIBS=!FAILED_LIBS! libsu"
    ) else (
        echo [PASS] libsu
        set /a LIB_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] libsu - Makefile not found
    set /a LIB_FAIL_COUNT+=1
)

REM Build win32compat library
echo.
echo Building win32compat library (libwin32compat)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\win32_compat\Makefile" (
    cd /d "%CWPROOT_WIN%\src\win32_compat"
    make 2>&1
    if errorlevel 1 (
        echo [FAIL] libwin32compat
        set /a LIB_FAIL_COUNT+=1
        set "FAILED_LIBS=!FAILED_LIBS! libwin32compat"
    ) else (
        echo [PASS] libwin32compat
        set /a LIB_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] libwin32compat - Makefile not found
)

REM Build comp library (DCT compression)
echo.
echo Building comp library (libcomp)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\comp\dct\lib\Makefile" (
    cd /d "%CWPROOT_WIN%\src\comp\dct\lib"
    make 2>&1
    if errorlevel 1 (
        echo [FAIL] libcomp
        set /a LIB_FAIL_COUNT+=1
        set "FAILED_LIBS=!FAILED_LIBS! libcomp"
    ) else (
        echo [PASS] libcomp
        set /a LIB_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] libcomp - Makefile not found
    set /a LIB_FAIL_COUNT+=1
)

REM Build triang library (Triangle processing)
echo.
echo Building triang library (libtriang)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\tri\lib\Makefile" (
    cd /d "%CWPROOT_WIN%\src\tri\lib"
    make 2>&1
    if errorlevel 1 (
        echo [FAIL] libtriang
        set /a LIB_FAIL_COUNT+=1
        set "FAILED_LIBS=!FAILED_LIBS! libtriang"
    ) else (
        echo [PASS] libtriang
        set /a LIB_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] libtriang - Makefile not found
    set /a LIB_FAIL_COUNT+=1
)

REM Build trielas library (Elastic triangle)
echo.
echo Building trielas library (libtrielas)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\Trielas\lib\Makefile" (
    cd /d "%CWPROOT_WIN%\src\Trielas\lib"
    make 2>&1
    if errorlevel 1 (
        echo [FAIL] libtrielas
        set /a LIB_FAIL_COUNT+=1
        set "FAILED_LIBS=!FAILED_LIBS! libtrielas"
    ) else (
        echo [PASS] libtrielas
        set /a LIB_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] libtrielas - Makefile not found
    set /a LIB_FAIL_COUNT+=1
)

REM Build tetra library (Tetrahedral processing)
echo.
echo Building tetra library (libtetra)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\tetra\lib\Makefile" (
    cd /d "%CWPROOT_WIN%\src\tetra\lib"
    make 2>&1
    if errorlevel 1 (
        echo [FAIL] libtetra
        set /a LIB_FAIL_COUNT+=1
        set "FAILED_LIBS=!FAILED_LIBS! libtetra"
    ) else (
        echo [PASS] libtetra
        set /a LIB_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] libtetra - Makefile not found
    set /a LIB_FAIL_COUNT+=1
)

REM Build reflect library (Reflection coefficients)
echo.
echo Building reflect library (libreflect)...
echo ----------------------------------------
if exist "%CWPROOT_WIN%\src\Refl\lib\Makefile" (
    cd /d "%CWPROOT_WIN%\src\Refl\lib"
    make 2>&1
    if errorlevel 1 (
        echo [FAIL] libreflect
        set /a LIB_FAIL_COUNT+=1
        set "FAILED_LIBS=!FAILED_LIBS! libreflect"
    ) else (
        echo [PASS] libreflect
        set /a LIB_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] libreflect - Makefile not found
    set /a LIB_FAIL_COUNT+=1
)

REM ========================================
REM Build SU Main Programs
REM ========================================
echo.
echo ========================================
echo Building SU Main Programs
echo ========================================
echo.

REM List of subdirectories to build
set "SUBDIRS=amplitudes attributes_parameter_estimation convolution_correlation data_compression data_conversion datuming decon_shaping dip_moveout filters headers interp_extrap migration_inversion multicomponent noise operations picking stacking statics stretching_moveout_resamp supromax synthetics_waveforms_testpatterns tapering transforms velocity_analysis well_logs windowing_sorting_muting"

set "SU_SUCCESS_COUNT=0"
set "SU_FAIL_COUNT=0"
set "FAILED_DIRS="

for %%d in (%SUBDIRS%) do (
    echo.
    echo ----------------------------------------
    echo Building: %%d
    echo ----------------------------------------
    
    cd /d "%CWPROOT_WIN%\src\su\main\%%d" 2>nul
    if errorlevel 1 (
        echo [SKIP] Directory not found: %%d
    ) else (
        REM Clean first
        if exist *.exe del /F /Q *.exe 2>nul
        if exist *.obj del /F /Q *.obj 2>nul
        if exist INSTALL del /F /Q INSTALL 2>nul
        
        REM Build
        make -f Makefile INSTALL 2>&1
        
        if errorlevel 1 (
            echo [FAIL] %%d
            set /a SU_FAIL_COUNT+=1
            set "FAILED_DIRS=!FAILED_DIRS! %%d"
        ) else (
            echo [PASS] %%d
            set /a SU_SUCCESS_COUNT+=1
        )
    )
)

REM ========================================
REM Build CWP Main Utilities
REM ========================================
echo.
echo ========================================
echo Building CWP Main Utilities
echo ========================================
echo.

set "CWP_SUCCESS_COUNT=0"
set "CWP_FAIL_COUNT=0"

if exist "%CWPROOT_WIN%\src\cwp\main\Makefile" (
    cd /d "%CWPROOT_WIN%\src\cwp\main"
    echo "Building CWP utilities (ctrlstrip, fcat, isatty, etc.)..."
    echo ----------------------------------------
    REM Clean first
    if exist *.exe del /F /Q *.exe 2>nul
    if exist *.obj del /F /Q *.obj 2>nul
    if exist INSTALL del /F /Q INSTALL 2>nul
    
    REM Build
    make -f Makefile INSTALL 2>&1
    
    if errorlevel 1 (
        echo [FAIL] CWP main utilities
        set /a CWP_FAIL_COUNT+=1
    ) else (
        echo [PASS] CWP main utilities
        set /a CWP_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] CWP main utilities - Makefile not found
    set /a CWP_FAIL_COUNT+=1
)

REM ========================================
REM Build Triangle Utilities
REM ========================================
echo.
echo ========================================
echo Building Triangle Utilities
echo ========================================
echo.

set "TRI_SUCCESS_COUNT=0"
set "TRI_FAIL_COUNT=0"

if exist "%CWPROOT_WIN%\src\tri\main\Makefile" (
    cd /d "%CWPROOT_WIN%\src\tri\main"
    echo "Building Triangle utilities (gbbeam, trimodel, triray, etc.)..."
    echo ----------------------------------------
    REM Clean first
    if exist *.exe del /F /Q *.exe 2>nul
    if exist *.obj del /F /Q *.obj 2>nul
    if exist INSTALL del /F /Q INSTALL 2>nul
    
    REM Build
    make -f Makefile INSTALL 2>&1
    
    if errorlevel 1 (
        echo [FAIL] Triangle utilities
        set /a TRI_FAIL_COUNT+=1
    ) else (
        echo [PASS] Triangle utilities
        set /a TRI_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] Triangle utilities - Makefile not found
    set /a TRI_FAIL_COUNT+=1
)

REM ========================================
REM Build Elastic Triangle (Trielas) Utilities
REM ========================================
echo.
echo ========================================
echo Building Elastic Triangle Utilities
echo ========================================
echo.

set "TRIELAS_SUCCESS_COUNT=0"
set "TRIELAS_FAIL_COUNT=0"

if exist "%CWPROOT_WIN%\src\Trielas\main\Makefile" (
    cd /d "%CWPROOT_WIN%\src\Trielas\main"
    echo "Building Elastic Triangle utilities (elaray, elamodel, etc.)..."
    echo ----------------------------------------
    REM Clean first
    if exist *.exe del /F /Q *.exe 2>nul
    if exist *.obj del /F /Q *.obj 2>nul
    if exist INSTALL del /F /Q INSTALL 2>nul
    
    REM Build
    make -f Makefile INSTALL 2>&1
    
    if errorlevel 1 (
        echo [FAIL] Elastic Triangle utilities
        set /a TRIELAS_FAIL_COUNT+=1
    ) else (
        echo [PASS] Elastic Triangle utilities
        set /a TRIELAS_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] Elastic Triangle utilities - Makefile not found
    set /a TRIELAS_FAIL_COUNT+=1
)

REM ========================================
REM Build Tetrahedral Utilities
REM ========================================
echo.
echo ========================================
echo Building Tetrahedral Utilities
echo ========================================
echo.

set "TETRA_SUCCESS_COUNT=0"
set "TETRA_FAIL_COUNT=0"

if exist "%CWPROOT_WIN%\src\tetra\main\Makefile" (
    cd /d "%CWPROOT_WIN%\src\tetra\main"
    echo "Building Tetrahedral utilities (tetramod, sutetraray)..."
    echo ----------------------------------------
    REM Clean first
    if exist *.exe del /F /Q *.exe 2>nul
    if exist *.obj del /F /Q *.obj 2>nul
    if exist INSTALL del /F /Q INSTALL 2>nul
    
    REM Build
    make -f Makefile INSTALL 2>&1
    
    if errorlevel 1 (
        echo [FAIL] Tetrahedral utilities
        set /a TETRA_FAIL_COUNT+=1
    ) else (
        echo [PASS] Tetrahedral utilities
        set /a TETRA_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] Tetrahedral utilities - Makefile not found
    set /a TETRA_FAIL_COUNT+=1
)

REM ========================================
REM Build Reflection Utilities
REM ========================================
echo.
echo ========================================
echo Building Reflection Utilities
echo ========================================
echo.

set "REFL_SUCCESS_COUNT=0"
set "REFL_FAIL_COUNT=0"

if exist "%CWPROOT_WIN%\src\Refl\main\Makefile" (
    cd /d "%CWPROOT_WIN%\src\Refl\main"
    echo "Building Reflection utilities (sureflpsvsh)..."
    echo ----------------------------------------
    REM Clean first
    if exist *.exe del /F /Q *.exe 2>nul
    if exist *.obj del /F /Q *.obj 2>nul
    if exist INSTALL del /F /Q INSTALL 2>nul
    
    REM Build
    make -f Makefile INSTALL 2>&1
    
    if errorlevel 1 (
        echo [FAIL] Reflection utilities
        set /a REFL_FAIL_COUNT+=1
    ) else (
        echo [PASS] Reflection utilities
        set /a REFL_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] Reflection utilities - Makefile not found
    set /a REFL_FAIL_COUNT+=1
)

REM ========================================
REM Build Third Party Components
REM ========================================
echo.
echo ========================================
echo Building Third Party Components
echo ========================================
echo.

set "THIRD_PARTY_SUCCESS_COUNT=0"
set "THIRD_PARTY_FAIL_COUNT=0"

if exist "%CWPROOT_WIN%\src\Third_Party\Makefile" (
    cd /d "%CWPROOT_WIN%\src\Third_Party"
    echo "Building Third Party utilities (bison2su, etc.)..."
    echo ----------------------------------------
    REM Clean first
    if exist *.exe del /F /Q *.exe 2>nul
    if exist *.obj del /F /Q *.obj 2>nul
    if exist INSTALL del /F /Q INSTALL 2>nul
    
    REM Build
    make -f Makefile INSTALL 2>&1
    
    if errorlevel 1 (
        echo [FAIL] Third Party components
        set /a THIRD_PARTY_FAIL_COUNT+=1
    ) else (
        echo [PASS] Third Party components
        set /a THIRD_PARTY_SUCCESS_COUNT+=1
    )
) else (
    echo [SKIP] Third Party components - Makefile not found
    set /a THIRD_PARTY_FAIL_COUNT+=1
)

REM ========================================
REM Build Summary
REM ========================================
echo.
echo ========================================
echo Build Summary
echo ========================================
echo.
echo Libraries:
echo   Successful: %LIB_SUCCESS_COUNT%
echo   Failed: %LIB_FAIL_COUNT%
if not "!FAILED_LIBS!"=="" (
    echo   Failed libraries: !FAILED_LIBS!
)
echo.
echo SU Programs:
echo   Successful: %SU_SUCCESS_COUNT%
echo   Failed: %SU_FAIL_COUNT%
if not "!FAILED_DIRS!"=="" (
    echo   Failed directories: !FAILED_DIRS!
)
echo.
echo CWP Utilities:
echo   Successful: %CWP_SUCCESS_COUNT%
echo   Failed: %CWP_FAIL_COUNT%
echo.
echo Triangle Utilities:
echo   Successful: %TRI_SUCCESS_COUNT%
echo   Failed: %TRI_FAIL_COUNT%
echo.
echo Elastic Triangle Utilities:
echo   Successful: %TRIELAS_SUCCESS_COUNT%
echo   Failed: %TRIELAS_FAIL_COUNT%
echo.
echo Tetrahedral Utilities:
echo   Successful: %TETRA_SUCCESS_COUNT%
echo   Failed: %TETRA_FAIL_COUNT%
echo.
echo Reflection Utilities:
echo   Successful: %REFL_SUCCESS_COUNT%
echo   Failed: %REFL_FAIL_COUNT%
echo.
echo Third Party Components:
echo   Successful: %THIRD_PARTY_SUCCESS_COUNT%
echo   Failed: %THIRD_PARTY_FAIL_COUNT%

REM List libraries in lib directory
echo.
echo Libraries in lib directory:
cd /d "%CWPROOT_WIN%\lib"
dir /B *.lib *.a 2>nul

REM Count executables in bin
cd /d "%CWPROOT_WIN%\bin"
set "EXE_COUNT=0"
for %%f in (*.exe) do set /a EXE_COUNT+=1
echo.
echo Total executables in bin: %EXE_COUNT%

echo.
echo ========================================
echo Build Complete
echo ========================================

cd /d "%CWPROOT_WIN%"
endlocal

