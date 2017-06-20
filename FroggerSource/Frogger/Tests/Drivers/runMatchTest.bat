call runATest %1 %2
if %differ%==1 goto Fail
:Pass
echo %3
goto End
:Fail
echo %4
:End