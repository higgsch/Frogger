IF NOT DEFINED VSDevEnv (
SET VSDevEnv="true"
echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\vsvars32.bat"
)
cd "C:\Users\Wayne\Desktop\Notes\Personal\Frogger\FroggerSource\Frogger\Tests\Output"

if exist ..\..\Debug\Frogger.exe (
	nmake makefile compileSource_PF fversion=%1 fname=%2 2>nul
) else (
	echo "Frogger is not compiled"
)

cd "C:\Users\Wayne\Desktop\Notes\Personal\Frogger\FroggerSource\Frogger\Tests\Drivers"