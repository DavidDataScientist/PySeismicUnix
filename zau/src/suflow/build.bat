@echo off
REM Build script for suflow.exe
REM Requires Visual Studio 2022 environment

echo Building suflow.exe...

REM Check for cl.exe
where cl.exe >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR: cl.exe not found. Please run from VS Developer Command Prompt"
    echo "Or run: call \"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat\""
    exit /b 1
)

REM Compile
cl /nologo /W3 /O2 /MD /Fe:suflow.exe suflow.c
if %ERRORLEVEL% NEQ 0 (
    echo "[FAIL] Build failed"
    exit /b 1
)

echo "[PASS] suflow.exe built successfully"

REM Install to bin directory
if not exist ..\..\..\bin mkdir ..\..\..\bin
copy /Y suflow.exe ..\..\..\bin\suflow.exe >nul
echo "Installed to bin\suflow.exe"

