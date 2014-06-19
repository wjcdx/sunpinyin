::install XHua input method
@echo off

set SrcDir=%~dp0
set SrcDir=%SrcDir:~0,-1%

set /p TgtDir=请输入安装目录：
set TgtDir=%TgtDir%\XHua

if exist "%TgtDir%" (
  rd /s /q "%TgtDir%"
)

echo 复制 %SrcDir% 到安装目录：%TgtDir%
xcopy /e /y "%SrcDir%" "%TgtDir%\"

cd /d "%TgtDir%"
if %PROCESSOR_ARCHITECTURE% == x86 (
  echo 32位系统，安装32位输入法
  call :Reg4x86
) else (
  echo 64位系统，安装32/64位输入法
  call :Reg4x86_64
)

reg add HKLM\Software\XHua /v Path /t REG_SZ /d "%TgtDir%" /f
goto :Done

:Reg4x86
regsvr32 /s TextService-x86.dll
goto :eof

:Reg4x86_64
C:\Windows\syswow64\regsvr32.exe /s TextService-x86.dll
C:\Windows\System32\regsvr32.exe /s TextService-x86_64.dll
goto :eof

:Done
echo 安装完成！
pause