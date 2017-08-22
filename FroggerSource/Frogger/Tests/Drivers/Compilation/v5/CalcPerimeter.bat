(call echo 0
 call echo 0
 call echo 0
 call echo 1
call echo 0
 call echo 1
 call echo 1
 call echo 1
call echo 1
 call echo 1
 call echo 1
 call echo 0
call echo 1
 call echo 0
 call echo 0
 call echo 0) | call CalcPerimeter.exe>v5\CalcPerimeter.txt

(call echo 0
 call echo -3
 call echo 6
 call echo 0
call echo 6
 call echo 0
 call echo 0
 call echo 3
call echo 0
 call echo 3
 call echo -6
 call echo 0
call echo -6
 call echo 0
 call echo 0
 call echo -3) | call CalcPerimeter.exe>v5\CalcPerimeter1.txt

(call echo -7.75
 call echo 1.86
 call echo -5.81
 call echo -0.49
call echo -5.81
 call echo -0.49
 call echo 5.74
 call echo -8.41
call echo 5.74
 call echo -8.41
 call echo -2.07
 call echo -0.10
call echo -2.07
 call echo -0.10
 call echo -7.75
 call echo 1.86) | call CalcPerimeter.exe>v5\CalcPerimeter2.txt

cd ..\Drivers
call runMatchTest.bat ..\Output\v5\CalcPerimeter1.txt ..\ExpectedOutput\v5\CalcPerimeter1.txt %11
call runMatchTest.bat ..\Output\v5\CalcPerimeter2.txt ..\ExpectedOutput\v5\CalcPerimeter2.txt %12

cd ..\Output