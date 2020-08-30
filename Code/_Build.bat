@echo off
setlocal enableextensions enabledelayedexpansion

rem --------------------------------------------------------------------------------
rem -- Environment Settings
rem --------------------------------------------------------------------------------

if not defined VCINSTALLDIR call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" amd64
chdir /d "%~dp0"

rem if exist BaseTools\Bin\Win32\GenFds.exe rename BaseTools\Bin\Win32\GenFds.exe GenFds.labz

set PYTHON_HOME=c:\python27
Set PYTHON_FREEZER_PATH=%PYTHON_HOME%\Scripts
set IASL_PREFIX=%cd%\..\Tool\
set NASM_PREFIX=%cd%\..\Tool\
call edksetup.bat Reconfig VS2015
set WINSDK81_PREFIX=%WINSDK81x86_PREFIX%

cmd

endlocal
if not "%~1"=="skip" pause
exit /b 0