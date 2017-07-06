call Compilation\makeCompile_PF.bat %1 %2>nul
cd ..\Output
call ..\Drivers\Compilation\%1\%2.bat %3 %4
cd ..\Drivers
call runMatchTest.bat ..\Output\%1\%2.txt ..\ExpectedOutput\%1\%2.txt %3 %4