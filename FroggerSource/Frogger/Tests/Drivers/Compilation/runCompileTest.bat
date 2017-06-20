call Compilation\makeCompile.bat %1>nul
cd ..\Output
call ..\Drivers\Compilation\%1.bat %2 %3
cd ..\Drivers
call runMatchTest.bat ..\Output\%1.txt ..\ExpectedOutput\%1.txt %2 %3