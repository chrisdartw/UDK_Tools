@echo off
setlocal enableextensions enabledelayedexpansion

rem --------------------------------------------------------------------------------
rem -- Environment Settings
rem --------------------------------------------------------------------------------

if not defined VCINSTALLDIR call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
chdir /d "%~dp0"

rem if exist BaseTools\Bin\Win32\GenFds.exe rename BaseTools\Bin\Win32\GenFds.exe GenFds.labz

set PYTHON_HOME=c:\python27
Set PYTHON_FREEZER_PATH=%PYTHON_HOME%\Scripts
set IASL_PREFIX=%cd%\..\Tool\
set NASM_PREFIX=%cd%\..\Tool\
call edksetup.bat Reconfig VS2015
set WINSDK81_PREFIX=%WINSDK81x86_PREFIX%

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

pause
call :GetLocalTimeCount LOCAL_TIME1


call edksetup.bat ForceRebuild VS2015


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
