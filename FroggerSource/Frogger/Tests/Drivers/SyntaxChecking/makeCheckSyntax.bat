IF NOT DEFINED VSDevEnv (
SET VSDevEnv="true"
echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\vsvars32.bat"
echo Reload VS
)

cd "C:\Users\Wayne\Desktop\Notes\Personal\Frogger\FroggerSource\Frogger\Tests\Output"

set arg=%1

if exist ..\..\Debug\Frogger.exe (
	nmake makefile syntaxCheck fname=%arg% 2<nul
) else (
	echo "Frogger is not compiled"
)

cd "C:\Users\Wayne\Desktop\Notes\Personal\Frogger\FroggerSource\Frogger\Tests\Drivers"

