CFLAGS = -Zi
CC = cl

clean:
	del /f *.o *.cpp *.cpp *.ilk *.pdb *.exe

compileSource: $(fname).o
	$(CC) $(CFLAGS) -Fo$(fname).exe $(fname).o

$(fname).o: $(fname).cpp
	$(CC) $(CFLAGS) -c -Fo$(fname).o $(fname).cpp

$(fname).cpp: $(fname).fgr
	..\Debug\Frogger.exe $(fname).fgr $(fname).cpp