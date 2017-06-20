echo off

call Compilation\runCompileTest.bat v3\StringLength "Pass - StringLength" "Fail - StringLength"

pause

call _clean.lnk>nul

pause