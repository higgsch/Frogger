echo off

call Compilation\runCompileTest.bat if v1 HelloWorld_simple
call Compilation\runCompileTest.bat if v1 HelloWorld_full
call Compilation\runCompileTest.bat if v1 NestedArithmetic
call Compilation\runCompileTest.bat if v1 Headache
call Compilation\runCompileTest.bat if v1 Headache_obfuscated
call Compilation\runCompileTest.bat if v1 RectArea
call Compilation\runCompileTest.bat if v1 RectArea_obfuscated

call Compilation\runCompileTest.bat if v2 If_simple
call Compilation\runCompileTest.bat if v2 QuadRoots
echo "call Compilation\runCompileTest.bat if v2 GuessMyNumber"

call Compilation\runCompileTest.bat if v3 File_HelloWorld
call Compilation\runCompileTest.bat if v3 CopyHelloFile
call Compilation\runCompileTest.bat if v3 CopyFile
call Compilation\runCompileTest.bat if v3 DisplayArgList
call Compilation\runCompileTest.bat if v3 StringLength

call Compilation\runCompileTest.bat pf v4 Months
call Compilation\runCompileTest.bat pf v4 Print

pause

call _clean.lnk>nul