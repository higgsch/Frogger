call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\vsvars32.bat"

:GenObjectFile
  cl -Zi -c -Fo%1.o %1.cpp 2>nul
  @if %ERRORLEVEL% == 0 (
    goto GenExecutableFile
  )
  @if ERRORLEVEL != 0 (
    goto Bad
  )

:GenExecutableFile
  cl -Zi -Fe%1.exe %1.o 2>nul
  @if %ERRORLEVEL% == 0 (
    goto Good
  )
  @if ERRORLEVEL != 0 (
    goto Bad
  )

:Good
  move %1.exe %2
  goto End

:Bad
  goto End

:End