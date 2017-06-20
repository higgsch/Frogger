fc %1 %2 >nul
if %errorlevel%==1 goto Differ
:Same
set differ=0
goto End
:Differ
set differ=1
:End