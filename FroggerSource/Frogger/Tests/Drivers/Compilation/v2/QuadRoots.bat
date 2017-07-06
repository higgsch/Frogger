(call echo 1
 call echo -1
 call echo -6) | call QuadRoots.exe>v2\QuadRoots.txt
(call echo 5
 call echo 2
 call echo 1) | call QuadRoots.exe>v2\QuadRoots1.txt
(call echo 0
 call echo -1
 call echo -6) | call QuadRoots.exe>v2\QuadRoots2.txt

cd ..\Drivers
call runMatchTest.bat ..\Output\v2\QuadRoots1.txt ..\ExpectedOutput\v2\QuadRoots1.txt %11
call runMatchTest.bat ..\Output\v2\QuadRoots2.txt ..\ExpectedOutput\v2\QuadRoots2.txt %12
cd ..\Output