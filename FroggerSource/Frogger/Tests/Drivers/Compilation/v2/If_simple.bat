call echo 1 | call If_simple.exe>v2\If_simple.txt
call echo 2 | call If_simple.exe>v2\If_simple1.txt
call echo 3 | call If_simple.exe>v2\If_simple2.txt
call echo 0 | call If_simple.exe>v2\If_simple3.txt

cd ..\Drivers
call runMatchTest.bat ..\Output\v2\If_simple1.txt ..\ExpectedOutput\v2\If_simple1.txt %11 %21
call runMatchTest.bat ..\Output\v2\If_simple2.txt ..\ExpectedOutput\v2\If_simple2.txt %12 %22
call runMatchTest.bat ..\Output\v2\If_simple3.txt ..\ExpectedOutput\v2\If_simple3.txt %13 %23
cd ..\Output