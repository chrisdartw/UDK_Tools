@echo off
setlocal enableextensions enabledelayedexpansion

rem :: CHECK ADMIN RIGHTS
rem "%windir%\system32\reg.exe" query "HKU\S-1-5-19" 1>nul 2>nul || (
rem   powershell.exe Start-Process -FilePath '"%~fs0"' -Verb runAs
rem   goto :eof
rem )

rem
rem Switch to working folder
rem
chdir /d "%~dp0"

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

call 7za x -y "nasm-2.14.02-win64.zip"
rename nasm-2.14.02 Tool
call 7za x -y -o"Tool" "iasl-win-20181213.zip"

call 7za x -y "edk2-vUDK2018.zip"
rename edk2-vUDK2018 UDK2018

call 7za x -y -o"UDK2018\BaseTools\Bin" "edk2-BaseTools-win32-master.zip"
rename UDK2018\BaseTools\Bin\edk2-BaseTools-win32-master Win32

xcopy /i /e /y code UDK2018

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

endlocal
if not "%~1"=="skip" pause
exit /b 0

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
