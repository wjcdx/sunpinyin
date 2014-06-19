::install XHua input method
@echo off

set SrcDir=%~dp0
set SrcDir=%SrcDir:~0,-1%

set /p TgtDir=�����밲װĿ¼��
set TgtDir=%TgtDir%\XHua

if exist "%TgtDir%" (
  rd /s /q "%TgtDir%"
)

echo ���� %SrcDir% ����װĿ¼��%TgtDir%
xcopy /e /y "%SrcDir%" "%TgtDir%\"

cd /d "%TgtDir%"
if %PROCESSOR_ARCHITECTURE% == x86 (
  echo 32λϵͳ����װ32λ���뷨
  call :Reg4x86
) else (
  echo 64λϵͳ����װ32/64λ���뷨
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
echo ��װ��ɣ�
pause