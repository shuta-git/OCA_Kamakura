@echo off

pushd "%~dp0"
call @clean.bat
popd

::----------------------------------------------------
:: �v���W�F�N�g�t�@�C���𐶐�
::----------------------------------------------------
bin\premake5.exe vs2019 --file=bin/premake5.lua

if %ERRORLEVEL% neq 0 (
	echo.
	echo �G���[���������܂����BLua�X�N���v�g���m�F���Ă��������B
	pause
)
