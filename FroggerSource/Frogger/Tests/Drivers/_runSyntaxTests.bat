echo off

call SyntaxChecking\runSyntaxTest.bat Arithmetic_Test Pass "Fail - Arithmetic Test"
call SyntaxChecking\runSyntaxTest.bat Command_Test Pass "Fail - Command Test"
call SyntaxChecking\runSyntaxTest.bat Function_Test Pass "Fail - Function Test"
call SyntaxChecking\runSyntaxTest.bat If_Test Pass "Fail - If Test"
call SyntaxChecking\runSyntaxTest.bat Syntax_Test Pass "Fail - Syntax Test"
call SyntaxChecking\runSyntaxTest.bat Temp_Test Pass "Fail Temp Test"


call ..\_clean.lnk>nul

pause