CFLAGS = -Zi
CC = cl

compiler.exe: compiler.o parser.o summationPhase.o stringConversionPhase.o codeGenerationPhase.o varDecSubPhase.o tempAssignSubPhase.o nodes.o scanner.o token.o obfuscator.o
	$(CC) $(CFLAGS) -Focompiler.exe compiler.o parser.o summationPhase.o stringConversionPhase.o codeGenerationPhase.o varDecSubPhase.o tempAssignSubPhase.o nodes.o scanner.o token.o obfuscator.o

compiler.o: compiler.h compiler.cpp parser.o summationPhase.o stringConversionPhase.o codeGenerationPhase.o varDecSubPhase.o tempAssignSubPhase.o obfuscator.o
	$(CC) $(CFLAGS) -c -Focompiler.o compiler.cpp

obfuscator.o: obfuscator.h obfuscator.cpp
	$(CC) $(CFLAGS) -c -Foobfuscator.o obfuscator.cpp

tempAssignSubPhase.o: tempAssignSubPhase.h tempAssignSubPhase.cpp phases.h nodes.h
	$(CC) $(CFLAGS) -c -FotempAssignSubPhase.o tempAssignSubPhase.cpp

varDecSubPhase.o: varDecSubPhase.h varDecSubPhase.cpp phases.h nodes.h
	$(CC) $(CFLAGS) -c -FovarDecSubPhase.o varDecSubPhase.cpp

codeGenerationPhase.o: codeGenerationPhase.h codeGenerationPhase.cpp varDecSubPhase.h tempAssignSubPhase.h phases.h nodes.h
	$(CC) $(CFLAGS) -c -FocodeGenerationPhase.o codeGenerationPhase.cpp

stringConversionPhase.o: stringConversionPhase.h stringConversionPhase.cpp phases.h nodes.h
	$(CC) $(CFLAGS) -c -FostringConversionPhase.o stringConversionPhase.cpp

summationPhase.o: summationPhase.h summationPhase.cpp phases.h nodes.h
	$(CC) $(CFLAGS) -c -FosummationPhase.o summationPhase.cpp

parser.out: parser.o scanner.o token.o nodes.o
	$(CC) $(CFLAGS) -Foparser.out parser.o scanner.o token.o nodes.o

parser.o: parser.h parser.cpp scanner.o token.o
	$(CC) $(CFLAGS) -c -Foparser.o parser.cpp

nodes.o: nodes.h nodes.cpp phases.h
	$(CC) $(CFLAGS) -c -Fonodes.o nodes.cpp

scanner.out: scanner.o token.o
	$(CC) $(CFLAGS) -Foscanner.out scanner.o token.o

scanner.o: scanner.h scanner.cpp token.h
	$(CC) $(CFLAGS) -c -Foscanner.o scanner.cpp

token.o: token.h token.cpp
	$(CC) $(CFLAGS) -c -Fotoken.o token.cpp

clean:
	del /f *.o *.ilk *.pdb *.exe Sample*.cpp Example*.cpp