call CopyFile.exe makefile_if v3\CopyFile.txt
call CopyFile.exe >v3\CopyFile1.txt
call CopyFile.exe makefile >v3\CopyFile2.txt
call CopyFile.exe makefile v3\CopyFile.txt error.txt >v3\CopyFile3.txt

cd ..\Drivers
call runMatchTest.bat ..\Output\v3\CopyFile1.txt ..\ExpectedOutput\v3\CopyFile_Usage.txt %11
call runMatchTest.bat ..\Output\v3\CopyFile2.txt ..\ExpectedOutput\v3\CopyFile_Usage.txt %12
call runMatchTest.bat ..\Output\v3\CopyFile3.txt ..\ExpectedOutput\v3\CopyFile_Usage.txt %13

cd ..\Output