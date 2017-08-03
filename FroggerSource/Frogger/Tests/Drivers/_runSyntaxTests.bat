echo off

call SyntaxChecking\runSyntaxTest.bat if Arithmetic_Test 
call SyntaxChecking\runSyntaxTest.bat if Command_Test 
call SyntaxChecking\runSyntaxTest.bat if Function_Test 
call SyntaxChecking\runSyntaxTest.bat if If_Test 
call SyntaxChecking\runSyntaxTest.bat if Syntax_Test 
call SyntaxChecking\runSyntaxTest.bat if Temp_Test 
call SyntaxChecking\runSyntaxTest.bat if Recursive

call SyntaxChecking\runSyntaxTest.bat pf Stand_Alone 
call SyntaxChecking\runSyntaxTest.bat pf Stand_Alone_With_SCF 
call SyntaxChecking\runSyntaxTest.bat pf Single_UDF 
call SyntaxChecking\runSyntaxTest.bat pf Multi_UDF 
call SyntaxChecking\runSyntaxTest.bat pf Overload


pause

call ..\_clean.lnk>nul