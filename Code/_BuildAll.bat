@echo off
setlocal enableextensions enabledelayedexpansion

rem --------------------------------------------------------------------------------
rem -- Environment Settings
rem --------------------------------------------------------------------------------

if not defined VCINSTALLDIR call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" amd64
chdir /d "%~dp0"

if exist BaseTools\Bin\Win32\GenFds.exe rename BaseTools\Bin\Win32\GenFds.exe GenFds.labz

set PYTHON_HOME=c:\python27
set IASL_PREFIX=%cd%\..\Tool\
set NASM_PREFIX=%cd%\..\Tool\
call edksetup.bat --nt32 X64 Reconfig

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

pause
call :GetLocalTimeCount LOCAL_TIME1

rd Build /s /q > Log.txt

call build -p AppPkg\AppPkg.dsc -t VS2015x86 -b DEBUG   -a X64      >> Log.txt
call build -p AppPkg\AppPkg.dsc -t VS2015x86 -b DEBUG   -a IA32     >> Log.txt
call build -p AppPkg\AppPkg.dsc -t VS2015x86 -b RELEASE -a X64      >> Log.txt
call build -p AppPkg\AppPkg.dsc -t VS2015x86 -b RELEASE -a IA32     >> Log.txt

set p=
set p=%p% "LongRunShellTool"
set p=%p% "EntryPointTest1"
set p=%p% "EntryPointTest2"
set p=%p% "EntryPointTest3"
set p=%p% "VbiosTest"
set p=%p% "OpenGc3Test"
set p=%p% "ShellLoader"
set p=%p% "UdfWalker"
set p=%p% "QualifyByDateTime"

for %%i in (%p%) do (
  copy /y .\build\AppPkg\DEBUG_VS2015x86\IA32\%%~i.efi   .\AppPkg\Applications\%%~i\%%~i_DEBUG_IA32.efi   > NUL
  copy /y .\build\AppPkg\DEBUG_VS2015x86\X64\%%~i.efi    .\AppPkg\Applications\%%~i\%%~i_DEBUG_X64.efi    > NUL
  copy /y .\build\AppPkg\RELEASE_VS2015x86\IA32\%%~i.efi .\AppPkg\Applications\%%~i\%%~i_RELEASE_IA32.efi > NUL
  copy /y .\build\AppPkg\RELEASE_VS2015x86\X64\%%~i.efi  .\AppPkg\Applications\%%~i\%%~i_RELEASE_X64.efi  > NUL
)

call build -p ShellPkg\ShellPkg.dsc -t VS2015x86 -b DEBUG   -a X64  >> Log.txt
call build -p ShellPkg\ShellPkg.dsc -t VS2015x86 -b DEBUG   -a IA32 >> Log.txt
call build -p ShellPkg\ShellPkg.dsc -t VS2015x86 -b RELEASE -a X64  >> Log.txt
call build -p ShellPkg\ShellPkg.dsc -t VS2015x86 -b RELEASE -a IA32 >> Log.txt

set p=
set p=%p% "Shell"

for %%i in (%p%) do (
  copy /y .\build\Shell\DEBUG_VS2015x86\IA32\%%~i.efi   .\ShellPkg\Application\%%~i\%%~i_DEBUG_IA32.efi   > NUL
  copy /y .\build\Shell\DEBUG_VS2015x86\X64\%%~i.efi    .\ShellPkg\Application\%%~i\%%~i_DEBUG_X64.efi    > NUL
  copy /y .\build\Shell\RELEASE_VS2015x86\IA32\%%~i.efi .\ShellPkg\Application\%%~i\%%~i_RELEASE_IA32.efi > NUL
  copy /y .\build\Shell\RELEASE_VS2015x86\X64\%%~i.efi  .\ShellPkg\Application\%%~i\%%~i_RELEASE_X64.efi  > NUL
)

call build -p Nt32Pkg\Nt32Pkg.dsc -t VS2015x86 -b DEBUG   -a X64    >> Log.txt
call build -p Nt32Pkg\Nt32Pkg.dsc -t VS2015x86 -b DEBUG   -a IA32   >> Log.txt
call build -p Nt32Pkg\Nt32Pkg.dsc -t VS2015x86 -b RELEASE -a X64    >> Log.txt
call build -p Nt32Pkg\Nt32Pkg.dsc -t VS2015x86 -b RELEASE -a IA32   >> Log.txt


rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

rem call build run                -t VS2015x86 -b DEBUG   -a X64
rem call build run                -t VS2015x86 -b DEBUG   -a IA32
rem call build run                -t VS2015x86 -b RELEASE -a X64
rem call build run                -t VS2015x86 -b RELEASE -a IA32

rem .\Build\NT32X64\DEBUG_VS2015x86\X64\
rem .\Build\NT32IA32\DEBUG_VS2015x86\IA32\
rem .\Build\NT32IA32\RELEASE_VS2015x86\IA32\
rem .\Build\NT32X64\RELEASE_VS2015x86\X64\

rem .\Build\AppPkg\DEBUG_VS2015x86\X64\
rem .\Build\AppPkg\DEBUG_VS2015x86\IA32\
rem .\Build\AppPkg\RELEASE_VS2015x86\IA32\
rem .\Build\AppPkg\RELEASE_VS2015x86\X64\

rem .\EdkShellBinPkg\Bin\X64\Apps\
rem .\EdkShellBinPkg\Bin\Ia32\Apps\



rem copy /y .\Build\AppPkg\DEBUG_VS2015x86\X64\ShellLoader.efi    .\Build\NT32X64\DEBUG_VS2015x86\X64\
rem copy /y .\Build\AppPkg\DEBUG_VS2015x86\IA32\ShellLoader.efi   .\Build\NT32IA32\DEBUG_VS2015x86\IA32\
rem copy /y .\Build\AppPkg\RELEASE_VS2015x86\X64\ShellLoader.efi  .\Build\NT32X64\RELEASE_VS2015x86\X64\
rem copy /y .\Build\AppPkg\RELEASE_VS2015x86\IA32\ShellLoader.efi .\Build\NT32IA32\RELEASE_VS2015x86\IA32\


rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

call :GetLocalTimeCount LOCAL_TIME1

endlocal
if not "%~1"=="skip" pause
exit /b 0


:GetLocalTimeCount
  for /f %%x in ('wmic path win32_localtime get /format:list ^| findstr "="') do (set _%%x)
  set /a z=(14-100%_Month%%%100)/12, y=10000%_Year%%%10000-z
  set /a ut=y*365+y/4-y/100+y/400+(153*(100%_Month%%%100+12*z-3)+2)/5+%_Day%-719469
  set /a ut=ut*86400+100%_Hour%%%100*3600+100%_Minute%%%100*60+100%_Second%%%100
  if not defined %~1 set "%~1=%ut%" & exit /b 0
  set param=%~1
  call set /a _elapsed=%ut% - %%!param!%%
  set "%~1="
  set /a _H=%_elapsed%/3600
  set /a _M=(%_elapsed%%%3600)/60+100
  set /a _S=((%_elapsed%%%3600)%%60)/1+100
  set /a _T= %_H%*3600 + %_M:~-2%*60 + %_S:~-2%
  echo elapsed time  %_H%:%_M:~-2%:%_S:~-2% (%_T% sec total)
  exit /b 0

exit /b 0
