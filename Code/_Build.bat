@echo off
setlocal enableextensions enabledelayedexpansion

if not defined VCINSTALLDIR call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" amd64
chdir /d "%~dp0"

IF EXIST BaseTools\Bin\Win32\GenFds.exe rename BaseTools\Bin\Win32\GenFds.exe GenFds.labz

Set PYTHON_HOME=c:\python27
Set PYTHON_FREEZER_PATH=%PYTHON_HOME%\Scripts
Set IASL_PREFIX=%cd%\..\Tool\
Set NASM_PREFIX=%cd%\..\Tool\
call edksetup.bat Reconfig VS2015

cmd

endlocal
if not "%~1"=="skip" pause
exit /b 0