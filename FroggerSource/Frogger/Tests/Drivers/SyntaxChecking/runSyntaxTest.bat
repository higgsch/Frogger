call SyntaxChecking\makeCheckSyntax.bat %1 %2>nul
call runMatchTest.bat ..\Output\%2.txt ..\ExpectedOutput\%2.txt %2