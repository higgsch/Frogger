call runATest %1 %2
if %differ%==1 goto Fail
:Pass
echo Pass - %3
goto End
:Fail
echo #####Fail - %3###########################################
:End