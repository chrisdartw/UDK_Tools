@echo off

set prj=VsProject

if exist ".\Release\%prj%.exe"     copy /y ".\Release\%prj%.exe"     ".\Bin\%prj%_x32.exe"
if exist ".\x64\Release\%prj%.exe" copy /y ".\x64\Release\%prj%.exe" ".\Bin\%prj%_x64.exe"

if exist ".\Release" rd /s /q ".\Release"
if exist ".\Debug"   rd /s /q ".\Debug"
if exist ".\x64"     rd /s /q ".\x64"

if exist ".\%prj%\Release" rd /s /q ".\%prj%\Release"
if exist ".\%prj%\Debug"   rd /s /q ".\%prj%\Debug"
if exist ".\%prj%\x64"     rd /s /q ".\%prj%\x64"

if exist ".\.vs\%prj%\v16\ipch"             rd /s /q ".\.vs\%prj%\v16\ipch"
if exist ".\.vs\%prj%\v16\Browse.VC.db"     del      ".\.vs\%prj%\v16\Browse.VC.db"

if exist ".\%prj%.VC.db"                    del      ".\%prj%.VC.db"
if exist ".\%prj%.VC.VC.opendb"             del      ".\%prj%.VC.VC.opendb"
if exist ".\.vs\"                           rd /s /q ".\.vs\"

pause