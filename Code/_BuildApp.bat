@echo off
setlocal enableextensions enabledelayedexpansion

if not defined VCINSTALLDIR call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" amd64
chdir /d "%~dp0"

IF EXIST BaseTools\Bin\Win32\GenFds.exe rename BaseTools\Bin\Win32\GenFds.exe GenFds.labz

Set PYTHON_HOME=c:\python27
Set IASL_PREFIX=%cd%\..\Tool\
Set NASM_PREFIX=%cd%\..\Tool\
call edksetup.bat --nt32 X64 Reconfig

call build -p AppPkg\AppPkg.dsc -t VS2015x86 -b DEBUG   -a X64
call build -p AppPkg\AppPkg.dsc -t VS2015x86 -b DEBUG   -a IA32
call build -p AppPkg\AppPkg.dsc -t VS2015x86 -b RELEASE -a X64
call build -p AppPkg\AppPkg.dsc -t VS2015x86 -b RELEASE -a IA32

set p=
set p=%p% "LongRunShellTool"
set p=%p% "EntryPointTest1"
set p=%p% "EntryPointTest2"
set p=%p% "EntryPointTest3"
set p=%p% "VbiosTest"
set p=%p% "OpenGc3Test"

for %%i in (%p%) do (
  copy /y .\build\AppPkg\DEBUG_VS2015x86\IA32\%%~i.efi   .\AppPkg\Applications\%%~i\%%~i_DEBUG_IA32.efi   > NUL
  copy /y .\build\AppPkg\DEBUG_VS2015x86\X64\%%~i.efi    .\AppPkg\Applications\%%~i\%%~i_DEBUG_X64.efi    > NUL
  copy /y .\build\AppPkg\RELEASE_VS2015x86\IA32\%%~i.efi .\AppPkg\Applications\%%~i\%%~i_RELEASE_IA32.efi > NUL
  copy /y .\build\AppPkg\RELEASE_VS2015x86\X64\%%~i.efi  .\AppPkg\Applications\%%~i\%%~i_RELEASE_X64.efi  > NUL
)

endlocal
if not "%~1"=="skip" pause
exit /b 0