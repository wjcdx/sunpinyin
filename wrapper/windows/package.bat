::make a install package
@echo off
set PkgDir=XHua

call :RstDir %PkgDir%
copy install.bat %PkgDir%

::install dlls
set BinDir=%PkgDir%
copy Release\TextService-x86.dll "%BinDir%"
copy x64\Release\TextService-x86_64.dll "%BinDir%"

::install dictionaries
set DictDstDir=%PkgDir%\dict\xh
set DictSrcDir=..\..\src\dict
call :RstDir %DictDstDir%
copy "%DictSrcDir%\dict3_sc.bin.xh" "%DictDstDir%\dict3_sc.bin"
copy "%DictSrcDir%\lm_sc.t3g.xh" "%DictDstDir%\lm_sc.t3g"

::install configurations
set ConfDstDir=%PkgDir%\config
set ConfSrcDir=TextService\Config
call :RstDir %ConfDstDir%
copy "%ConfSrcDir%\configs.dat" "%ConfDstDir%"

goto Done

:RstDir
set Dir=%1
if exist "%Dir%" (
  rd /s /q "%Dir%"
)
md %Dir%
goto :eof

:Done
pause