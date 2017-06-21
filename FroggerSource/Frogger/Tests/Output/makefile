CFLAGS = -Zi
CC = cl

clean:
	del /f /s *.o *.cpp *.cpp *.ilk *.pdb *.exe *.txt

compileSource: $(fname).o
	$(CC) $(CFLAGS) -Fo$(fname).exe $(fname).o

$(fname).o: $(fname).cpp
	$(CC) $(CFLAGS) -c -Fo$(fname).o $(fname).cpp

$(fname).cpp: ..\..\Samples\$(fname).fgr
	..\..\Debug\Frogger.exe /q /if ..\..\Samples\$(fname).fgr /o $(fname).cpp>nul

syntaxCheck: ..\InputCode\$(fname).fgr
	..\..\Debug\Frogger.exe /q /if ..\InputCode\$(fname).fgr /o $(fname).cpp>$(fname).txt

syntaxCheck_PF: ..\InputCode\$(fname)\$(fname).fgr
	..\..\Debug\Frogger.exe /q /ip ..\InputCode\$(fname) /o $(fname).cpp>$(fname).txt