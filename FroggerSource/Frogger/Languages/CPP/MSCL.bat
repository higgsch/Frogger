call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\vsvars32.bat"
cl -Zi -c -Fo%1.o %1.cpp 2>nul
cl -Zi -Fe%1.exe %1.o 2>nul
move %1.exe %2