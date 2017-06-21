echo off

call SyntaxChecking\runSyntaxTest.bat Arithmetic_Test Pass "Fail - Arithmetic Test"
call SyntaxChecking\runSyntaxTest.bat Command_Test Pass "Fail - Command Test"
call SyntaxChecking\runSyntaxTest.bat Function_Test Pass "Fail - Function Test"
call SyntaxChecking\runSyntaxTest.bat If_Test Pass "Fail - If Test"
call SyntaxChecking\runSyntaxTest.bat Syntax_Test Pass "Fail - Syntax Test"
call SyntaxChecking\runSyntaxTest.bat Temp_Test Pass "Fail Temp Test"

call SyntaxChecking\runSyntaxTest_PF.bat Stand_Alone Pass "Fail - Stand Alone"
call SyntaxChecking\runSyntaxTest_PF.bat Stand_Alone_With_SCF Pass "Fail - Stand Alone with SCF"
call SyntaxChecking\runSyntaxTest_PF.bat Single_UDF Pass "Fail - Single UDF"
call SyntaxChecking\runSyntaxTest_PF.bat Multi_UDF Pass "Fail - Multi UDF"

pause

call ..\_clean.lnk>nul