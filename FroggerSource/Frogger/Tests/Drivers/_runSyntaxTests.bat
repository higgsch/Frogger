echo off

call SyntaxChecking\runSyntaxTest.bat Arithmetic_Test "Pass - Arithmetic Test" "Fail - Arithmetic Test"
call SyntaxChecking\runSyntaxTest.bat Command_Test "Pass - Command Test" "Fail - Command Test"
call SyntaxChecking\runSyntaxTest.bat Function_Test "Pass - Function Test" "Fail - Function Test"
call SyntaxChecking\runSyntaxTest.bat If_Test "Pass - If Test" "Fail - If Test"
call SyntaxChecking\runSyntaxTest.bat Syntax_Test "Pass - Syntax Test" "Fail - Syntax Test"
call SyntaxChecking\runSyntaxTest.bat Temp_Test "Pass - Temp Test" "Fail Temp - Test"

call SyntaxChecking\runSyntaxTest_PF.bat Stand_Alone "Pass - Stand Alone Function Test" "Fail - Stand Alone Function Test"
call SyntaxChecking\runSyntaxTest_PF.bat Stand_Alone_With_SCF "Pass - Stand Alone with SCF Test" "Fail - Stand Alone with SCF Test"
call SyntaxChecking\runSyntaxTest_PF.bat Single_UDF "Pass - Single UDF Test" "Fail - Single UDF Test"
call SyntaxChecking\runSyntaxTest_PF.bat Multi_UDF "Pass - Multi UDF Test" "Fail - Multi UDF Test"


pause

call ..\_clean.lnk>nul