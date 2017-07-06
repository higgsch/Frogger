call SyntaxChecking\makeCheckSyntax_PF.bat %1>nul
call runMatchTest.bat ..\Output\%1.txt ..\ExpectedOutput\%1.txt %2 %3