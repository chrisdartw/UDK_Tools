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

call 7za x -y -o"Tool" "iasl-win-20190816.zip"

call 7za x -y "nasm-2.14.02-win64.zip"
xcopy /i /e /y nasm-2.14.02 Tool
rmdir /s /q    nasm-2.14.02

call 7za x -y "edk2-libc-master.zip"
xcopy /i /e /y edk2-libc-master UDK2019
rmdir /s /q    edk2-libc-master

call 7za x -y "edk2-edk2-stable201908.zip"
xcopy /i /e /y edk2-edk2-stable201908 UDK2019
rmdir /s /q    edk2-edk2-stable201908

call 7za x -y -o"UDK2019\BaseTools\Bin" "edk2-BaseTools-win32-master.zip"
rmdir /s /q    UDK2019\BaseTools\Bin\Win32
rename UDK2019\BaseTools\Bin\edk2-BaseTools-win32-master Win32

xcopy /i /e /y code UDK2019

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

endlocal
if not "%~1"=="skip" pause
exit /b 0

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
