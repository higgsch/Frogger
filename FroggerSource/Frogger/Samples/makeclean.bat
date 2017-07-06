IF NOT DEFINED VSDevEnv (
SET VSDevEnv="true"
echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\vsvars32.bat"

cd "C:\Users\Wayne\Desktop\Notes\Personal\Frogger\FroggerSource\Frogger\Samples"
)
nmake makefile clean
cd v1
nmake /f ..\makefile clean
cd ..\v2
nmake /f ..\makefile clean
cd ..\v3
nmake /f ..\makefile clean
cd ..\v4
nmake /f ..\makefile clean
