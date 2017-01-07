IF NOT DEFINED VSDevEnv (
SET VSDevEnv="true"
echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\vsvars32.bat"

cd "C:\Users\Wayne\Desktop\Notes\Personal\Frogger"
)

call makeclean.bat
nmake makefile compiler.exe