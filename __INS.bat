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


call wget --continue https://acpica.org/sites/acpica/files/iasl-win-20200717.zip
call 7za x -y -o"Tool" "iasl-win-20200717.zip"

call wget --continue https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip
call 7za x -y "nasm-2.15.05-win64.zip"
xcopy /i /e /y nasm-2.15.05 Tool
rmdir /s /q    nasm-2.15.05

call wget --continue -O edk2-libc.zip https://github.com/tianocore/edk2-libc/archive/master.zip
call 7za x -y "edk2-libc.zip"
xcopy /i /e /y edk2-libc-master UDK2020
rmdir /s /q    edk2-libc-master

call wget --continue https://github.com/tianocore/edk2/archive/edk2-stable202005.zip
call 7za x -y "edk2-stable202005.zip"
xcopy /i /e /y edk2-edk2-stable202005 UDK2020
rmdir /s /q    edk2-edk2-stable202005

call wget --continue https://github.com/openssl/openssl/archive/OpenSSL_1_1_1g.zip
call 7za x -y "OpenSSL_1_1_1g.zip"
xcopy /i /e /y openssl-OpenSSL_1_1_1g UDK2020\CryptoPkg\Library\OpensslLib\openssl\
rmdir /s /q    openssl-OpenSSL_1_1_1g

call wget --continue -O "softfloat.zip" https://github.com/ucb-bar/berkeley-softfloat-3/archive/master.zip
call 7za x -y "softfloat.zip"
xcopy /i /e /y berkeley-softfloat-3-master UDK2020\ArmPkg\Library\ArmSoftFloatLib\berkeley-softfloat-3\
rmdir /s /q    berkeley-softfloat-3-master

call wget --continue https://git.cryptomilk.org/projects/cmocka.git/snapshot/cmocka-1.1.5.zip
call 7za x -y "cmocka-1.1.5.zip"
xcopy /i /e /y cmocka-1.1.5 UDK2020\UnitTestFrameworkPkg\Library\CmockaLib\cmocka\
rmdir /s /q    cmocka-1.1.5

call wget --continue https://github.com/kkos/oniguruma/archive/v6.9.5_rev1.zip
call 7za x -y "v6.9.5_rev1.zip"
xcopy /i /e /y oniguruma-6.9.5_rev1 UDK2020\MdeModulePkg\Universal\RegularExpressionDxe\oniguruma\
rmdir /s /q    oniguruma-6.9.5_rev1

call wget --continue https://github.com/google/brotli/archive/v1.0.7.zip
call 7za x -y "v1.0.7.zip"
xcopy /i /e /y brotli-1.0.7 UDK2020\MdeModulePkg\Library\BrotliCustomDecompressLib\brotli\
xcopy /i /e /y brotli-1.0.7 UDK2020\BaseTools\Source\C\BrotliCompress\brotli\
rmdir /s /q    brotli-1.0.7


xcopy /i /e /y code UDK2020


rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

endlocal
if not "%~1"=="skip" pause
exit /b 0

rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------

