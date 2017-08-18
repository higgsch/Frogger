(call echo 0
 call echo 0
 call echo 0
 call echo 0) | call CalcDistance.exe>v5\CalcDistance.txt
(call echo 0
 call echo 0
 call echo 3
 call echo 4) | call CalcDistance.exe>v5\CalcDistance1.txt
(call echo 1
 call echo 2
 call echo 3
 call echo 4) | call CalcDistance.exe>v5\CalcDistance2.txt

cd ..\Drivers
call runMatchTest.bat ..\Output\v5\CalcDistance1.txt ..\ExpectedOutput\v5\CalcDistance1.txt %11
call runMatchTest.bat ..\Output\v5\CalcDistance2.txt ..\ExpectedOutput\v5\CalcDistance2.txt %12

cd ..\Output