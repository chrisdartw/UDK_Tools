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


call wget --no-check-certificate --continue https://acpica.org/sites/acpica/files/iasl-win-20220331.zip
call 7za x -y -o"Tool" "iasl-win-20220331.zip"

call wget --no-check-certificate --continue https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip
call 7za x -y "nasm-2.15.05-win64.zip"
xcopy /i /e /y nasm-2.15.05 Tool
rmdir /s /q    nasm-2.15.05

call wget --continue -O edk2-libc.zip https://github.com/tianocore/edk2-libc/archive/master.zip
call 7za x -y "edk2-libc.zip"
xcopy /i /e /y edk2-libc-master UDK2022
rmdir /s /q    edk2-libc-master

call wget --continue https://github.com/tianocore/edk2/archive/edk2-stable202205.zip
call 7za x -y "edk2-stable202205.zip"
xcopy /i /e /y edk2-edk2-stable202205 UDK2022
rmdir /s /q    edk2-edk2-stable202205

rem --------------------------------------------------------------------------------

call wget --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-BaseTools-Source-C-BrotliCompress-brotli.zip
call 7za x -y -oUDK2022 "submodule-BaseTools-Source-C-BrotliCompress-brotli.zip"

call wget --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-CryptoPkg-Library-OpensslLib-openssl.zip
call 7za x -y -oUDK2022 "submodule-CryptoPkg-Library-OpensslLib-openssl.zip"

call wget --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-MdeModulePkg-Library-BrotliCustomDecompressLib-brotli.zip
call 7za x -y -oUDK2022 "submodule-MdeModulePkg-Library-BrotliCustomDecompressLib-brotli.zip"

call wget --no-check-certificate --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-MdeModulePkg-Universal-RegularExpressionDxe-oniguruma.zip
call 7za x -y -oUDK2022 "submodule-MdeModulePkg-Universal-RegularExpressionDxe-oniguruma.zip"

call wget --no-check-certificate --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-RedfishPkg-Library-JsonLib-jansson.zip
call 7za x -y -oUDK2022 "submodule-RedfishPkg-Library-JsonLib-jansson.zip"

call wget --no-check-certificate --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-SoftFloat.zip
call 7za x -y -oUDK2022 "submodule-SoftFloat.zip"

call wget --no-check-certificate --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-UnitTestFrameworkPkg-Library-CmockaLib-cmocka.zip
call 7za x -y -oUDK2022 "submodule-UnitTestFrameworkPkg-Library-CmockaLib-cmocka.zip"

rem --------------------------------------------------------------------------------

call wget --no-check-certificate --continue https://www.python.org/ftp/python/3.8.10/python-3.8.10-amd64.exe

xcopy /i /e /y code UDK2022


rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

endlocal
if not "%~1"=="skip" pause
exit /b 0

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

