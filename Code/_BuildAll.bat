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

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

pause
call :GetLocalTimeCount LOCAL_TIME1

if exist Build rd Build /s /q > Log.txt

call build -p AppPkg\AppPkg.dsc             -t VS2019 -b DEBUG   -a X64  >> Log.txt
call build -p AppPkg\AppPkg.dsc             -t VS2019 -b DEBUG   -a IA32 >> Log.txt
call build -p AppPkg\AppPkg.dsc             -t VS2019 -b RELEASE -a X64  >> Log.txt
call build -p AppPkg\AppPkg.dsc             -t VS2019 -b RELEASE -a IA32 >> Log.txt

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
  copy /y .\build\AppPkg\DEBUG_VS2019\IA32\%%~i.efi   .\AppPkg\Applications\%%~i\%%~i_DEBUG_IA32.efi   > NUL
  copy /y .\build\AppPkg\DEBUG_VS2019\X64\%%~i.efi    .\AppPkg\Applications\%%~i\%%~i_DEBUG_X64.efi    > NUL
  copy /y .\build\AppPkg\RELEASE_VS2019\IA32\%%~i.efi .\AppPkg\Applications\%%~i\%%~i_RELEASE_IA32.efi > NUL
  copy /y .\build\AppPkg\RELEASE_VS2019\X64\%%~i.efi  .\AppPkg\Applications\%%~i\%%~i_RELEASE_X64.efi  > NUL
)

call build -p ShellPkg\ShellPkg.dsc         -t VS2019 -b DEBUG   -a X64  >> Log.txt
call build -p ShellPkg\ShellPkg.dsc         -t VS2019 -b DEBUG   -a IA32 >> Log.txt
call build -p ShellPkg\ShellPkg.dsc         -t VS2019 -b RELEASE -a X64  >> Log.txt
call build -p ShellPkg\ShellPkg.dsc         -t VS2019 -b RELEASE -a IA32 >> Log.txt

set p=
set p=%p% "Shell"

for %%i in (%p%) do (
  copy /y .\build\Shell\DEBUG_VS2019\IA32\%%~i.efi   .\ShellPkg\Application\%%~i\%%~i_DEBUG_IA32.efi   > NUL
  copy /y .\build\Shell\DEBUG_VS2019\X64\%%~i.efi    .\ShellPkg\Application\%%~i\%%~i_DEBUG_X64.efi    > NUL
  copy /y .\build\Shell\RELEASE_VS2019\IA32\%%~i.efi .\ShellPkg\Application\%%~i\%%~i_RELEASE_IA32.efi > NUL
  copy /y .\build\Shell\RELEASE_VS2019\X64\%%~i.efi  .\ShellPkg\Application\%%~i\%%~i_RELEASE_X64.efi  > NUL
)

  copy /y   .\build\Shell\DEBUG_VS2019\IA32\ShellPkg\Application\Shell\EA4BB293-2D7F-4456-A681-1F22F42CD0BC\OUTPUT\Shell.efi .\ShellPkg\Application\Shell\Shell_2nd_DEBUG_IA32.efi   > NUL
  copy /y    .\build\Shell\DEBUG_VS2019\X64\ShellPkg\Application\Shell\EA4BB293-2D7F-4456-A681-1F22F42CD0BC\OUTPUT\Shell.efi .\ShellPkg\Application\Shell\Shell_2nd_DEBUG_X64.efi    > NUL
  copy /y .\build\Shell\RELEASE_VS2019\IA32\ShellPkg\Application\Shell\EA4BB293-2D7F-4456-A681-1F22F42CD0BC\OUTPUT\Shell.efi .\ShellPkg\Application\Shell\Shell_2nd_RELEASE_IA32.efi > NUL
  copy /y  .\build\Shell\RELEASE_VS2019\X64\ShellPkg\Application\Shell\EA4BB293-2D7F-4456-A681-1F22F42CD0BC\OUTPUT\Shell.efi .\ShellPkg\Application\Shell\Shell_2nd_RELEASE_X64.efi  > NUL

call build -p EmulatorPkg\EmulatorPkg.dsc   -t VS2019 -b DEBUG   -a X64  >> Log.txt
call build -p EmulatorPkg\EmulatorPkg.dsc   -t VS2019 -b DEBUG   -a IA32 >> Log.txt
call build -p EmulatorPkg\EmulatorPkg.dsc   -t VS2019 -b RELEASE -a X64  >> Log.txt
call build -p EmulatorPkg\EmulatorPkg.dsc   -t VS2019 -b RELEASE -a IA32 >> Log.txt


rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

rem pushd .\Build\EmulatorX64\DEBUG_VS2019\X64\
rem WinHost.exe
rem popd

rem call :RunWinHost   .\Build\EmulatorIA32\DEBUG_VS2019\IA32\WinHost.exe
rem call :RunWinHost    .\Build\EmulatorX64\DEBUG_VS2019\X64\WinHost.exe
rem call :RunWinHost .\Build\EmulatorIA32\RELEASE_VS2019\IA32\WinHost.exe
rem call :RunWinHost  .\Build\EmulatorX64\RELEASE_VS2019\X64\WinHost.exe

rem   .\Build\AppPkg\DEBUG_VS2019\X64\
rem   .\Build\AppPkg\DEBUG_VS2019\IA32\
rem .\Build\AppPkg\RELEASE_VS2019\X64\
rem .\Build\AppPkg\RELEASE_VS2019\IA32\

rem .\EdkShellBinPkg\Bin\X64\Apps\
rem .\EdkShellBinPkg\Bin\Ia32\Apps\

rem copy /y   .\Build\AppPkg\DEBUG_VS2019\X64\ShellLoader.efi     .\Build\EmulatorX64\DEBUG_VS2019\X64\
rem copy /y   .\Build\AppPkg\DEBUG_VS2019\IA32\ShellLoader.efi   .\Build\EmulatorIA32\DEBUG_VS2019\IA32\
rem copy /y .\Build\AppPkg\RELEASE_VS2019\X64\ShellLoader.efi   .\Build\EmulatorX64\RELEASE_VS2019\X64\
rem copy /y .\Build\AppPkg\RELEASE_VS2019\IA32\ShellLoader.efi .\Build\EmulatorIA32\RELEASE_VS2019\IA32\

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

:RunWinHost
  pushd %~dp1
  call WinHost.exe
  popd
  exit /b 0

exit /b 0
