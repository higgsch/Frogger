(call echo 3
 call echo 5) | call RectArea.exe>v1\RectArea.txt
(call echo 5
 call echo 3) | call RectArea.exe>v1\RectArea1.txt
(call echo 0
 call echo 5) | call RectArea.exe>v1\RectArea2.txt
(call echo -2
 call echo 5) | call RectArea.exe>v1\RectArea3.txt

cd ..\Drivers
call runMatchTest.bat ..\Output\v1\RectArea1.txt ..\ExpectedOutput\v1\RectArea1.txt %11
call runMatchTest.bat ..\Output\v1\RectArea2.txt ..\ExpectedOutput\v1\RectArea2.txt %12
call runMatchTest.bat ..\Output\v1\RectArea3.txt ..\ExpectedOutput\v1\RectArea3.txt %13
cd ..\Output