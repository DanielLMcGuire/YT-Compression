@echo off
echo Begin init
powershell "rm -r -Force ./bin"
powershell "mkdir ./bin"
cls
echo If this fails, run from VS2022 Developer Command Prompt
cl /EHsc /Fe:bin\ytcomp.exe src\YTCompression.cpp /std:c++17
