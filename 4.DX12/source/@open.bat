@echo off

pushd "%~dp0"
call @clean.bat
popd

::----------------------------------------------------
:: �v���W�F�N�g�t�@�C���𐶐�
::----------------------------------------------------
bin\premake5.exe vs2019 --file=bin/premake5.lua

::----------------------------------------------------
:: �\�����[�V�����t�@�C��������
:: �����Ō����邱�ƂŃt�@�C�������ω����Ă��Ή��\
::----------------------------------------------------
cd "%~dp0\.build"
setlocal enabledelayedexpansion

for /f "usebackq" %%i in (`dir /b *.sln`) do set SOLUTION_FILE=%%i
if not exist "%SOLUTION_FILE%" (
	echo �v���W�F�N�g�t�@�C����������܂���Bmake.bat�����s���Ă��������B
	goto error
)

:: �ŐV��VisualStudio����N�����邽�߂ɋN������o�[�W������VisualStudio2019������
set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio\Installer
for /f "usebackq delims=" %%i in (`vswhere.exe -version 16.0^,17.0 -property productPath`) do (
	set DEVENV_EXE=%%i
)

:: �\�����[�V�������N��
start /min "" "%DEVENV_EXE%" "%SOLUTION_FILE%"

exit

:error
pause
