call DisplayArgList.exe This is "a test." >v3\DisplayArgList.txt
call DisplayArgList.exe >v3\DisplayArgList1.txt
call DisplayArgList.exe "This is another test." >v3\DisplayArgList2.txt

cd ..\Drivers
call runMatchTest.bat ..\Output\v3\DisplayArgList1.txt ..\ExpectedOutput\v3\DisplayArgList1.txt %11
call runMatchTest.bat ..\Output\v3\DisplayArgList2.txt ..\ExpectedOutput\v3\DisplayArgList2.txt %12
cd ..\Output