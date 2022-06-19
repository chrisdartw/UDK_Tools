@echo off
setlocal enableextensions enabledelayedexpansion

rem --------------------------------------------------------------------------------
rem -- Environment Settings
rem --------------------------------------------------------------------------------

rem if not defined VCINSTALLDIR call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat"
if not defined VCINSTALLDIR call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsamd64_x86.bat"
rem if not defined VCINSTALLDIR call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars32.bat"
rem if not defined VCINSTALLDIR call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsx86_amd64.bat"
chdir /d "%~dp0"

set CLANG_BIN=%VCINSTALLDIR%Tools\Llvm\x64\bin
set NASM_PREFIX=%cd%\..\Tool\
set IASL_PREFIX=%cd%\..\Tool\
set PYTHON_HOME=%LOCALAPPDATA%\Programs\Python\Python38\
call edksetup.bat Reconfig VS2019

cmd

endlocal
if not "%~1"=="skip" pause
exit /b 0