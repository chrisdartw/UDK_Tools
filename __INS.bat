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


call wget --no-check-certificate --continue https://acpica.org/sites/acpica/files/iasl-win-20221020.zip
call 7za x -y -o"Tool" "iasl-win-20221020.zip"

call wget --no-check-certificate --continue https://www.nasm.us/pub/nasm/releasebuilds/2.16.01/win64/nasm-2.16.01-win64.zip
call 7za x -y "nasm-2.16.01-win64.zip"
xcopy /i /e /y nasm-2.16.01 Tool
rmdir /s /q    nasm-2.16.01

call wget --no-check-certificate --continue -O edk2-libc.zip https://github.com/tianocore/edk2-libc/archive/master.zip
call 7za x -y "edk2-libc.zip"
xcopy /i /e /y edk2-libc-master UDK2022
rmdir /s /q    edk2-libc-master

call wget --no-check-certificate --continue https://github.com/tianocore/edk2/archive/refs/tags/edk2-stable202211.zip
call 7za x -y "edk2-stable202211.zip"
xcopy /i /e /y edk2-edk2-stable202211 UDK2022
rmdir /s /q    edk2-edk2-stable202211


rem --------------------------------------------------------------------------------

call wget --no-check-certificate --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-BaseTools-Source-C-BrotliCompress-brotli.zip
call 7za x -y -oUDK2022 "submodule-BaseTools-Source-C-BrotliCompress-brotli.zip"

call wget --no-check-certificate --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-CryptoPkg-Library-OpensslLib-openssl.zip
call 7za x -y -oUDK2022 "submodule-CryptoPkg-Library-OpensslLib-openssl.zip"

call wget --no-check-certificate --continue https://github.com/tianocore/edk2/releases/download/edk2-stable202111/submodule-MdeModulePkg-Library-BrotliCustomDecompressLib-brotli.zip
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

xcopy /i /e /y code UDK2022

call wget --no-check-certificate --continue https://www.python.org/ftp/python/3.10.9/python-3.10.9-amd64.exe


rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

endlocal
if not "%~1"=="skip" pause
exit /b 0

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

