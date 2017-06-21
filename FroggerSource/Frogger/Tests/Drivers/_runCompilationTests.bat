echo off

call Compilation\runCompileTest.bat v1\HelloWorld_simple "Pass - HelloWorld_simple" "Fail - HelloWorld_simple"
call Compilation\runCompileTest.bat v1\HelloWorld_full "Pass - HelloWorld_full" "Fail - HelloWorld_full"
call Compilation\runCompileTest.bat v1\NestedArithmetic "Pass - NestedArithmetic" "Fail - NestedArithmetic"
call Compilation\runCompileTest.bat v1\Headache "Pass - Headache" "Fail - Headache"
call Compilation\runCompileTest.bat v1\Headache_obfuscated "Pass - Headache_obfuscated" "Fail - Headache_obfuscated"
call Compilation\runCompileTest.bat v1\RectArea "Pass - RectArea" "Fail - RectArea"
call Compilation\runCompileTest.bat v1\RectArea_obfuscated "Pass - RectArea_obfuscated" "Fail - RectArea_obfuscated"

call Compilation\runCompileTest.bat v2\If_simple "Pass - If_Simple" "Fail - If_Simple"
call Compilation\runCompileTest.bat v2\QuadRoots "Pass - QuadRoots" "Fail - QuadRoots"
call Compilation\runCompileTest.bat v2\GuessMyNumber "Pass - GuessMyNumber" "Fail - GuessMyNumber"

call Compilation\runCompileTest.bat v3\File_HelloWorld "Pass - File_HelloWorld" "Fail - File_HelloWorld"
call Compilation\runCompileTest.bat v3\CopyHelloFile "Pass - CopyHelloFile" "Fail - CopyHelloFile"
call Compilation\runCompileTest.bat v3\CopyFile "Pass - CopyFile" "Fail - CopyFile"
call Compilation\runCompileTest.bat v3\DisplayArgList "Pass - DisplayArgList" "Fail - DisplayArgList"
call Compilation\runCompileTest.bat v3\StringLength "Pass - StringLength" "Fail - StringLength"

pause

call _clean.lnk>nul