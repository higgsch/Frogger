call Compilation\makeCompile.bat %1 %2 %3>nul
cd ..\Output
call ..\Drivers\Compilation\%2\%3.bat %3
cd ..\Drivers
call runMatchTest.bat ..\Output\%2\%3.txt ..\ExpectedOutput\%2\%3.txt %3