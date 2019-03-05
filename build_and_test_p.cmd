@Echo Off
SETLOCAL enabledelayedexpansion

REM Arma 3 tools folder path
SET TOOLS_PATH=D:\Program Files (x86)\Steam\steamapps\common\DayZ Tools\Bin
SET DAYZ_CLIENT_PATH=D:\Program Files (x86)\Steam\steamapps\common\DayZ
SET DAYZ_SERVER_PATH=D:\Program Files (x86)\Steam\steamapps\common\DayZServer
SET SERVER_PROFILE_NAME=EpochServer

SET WORKSHOP_PATH=%DAYZ_CLIENT_PATH%\!Workshop\

REM set output folder name
SET MOD_NAME=@EpochTest

REM path to compiled mod
SET CLIENT_PATH=%DAYZ_CLIENT_PATH%\%MOD_NAME%
SET SERVER_PATH=%DAYZ_SERVER_PATH%\%MOD_NAME%

REM SET OTHER_MODS=%WORKSHOP_PATH%@RPCFramework;%WORKSHOP_PATH%@Permissions-Framework;%WORKSHOP_PATH%@Community-Online-Tools;
SET OTHER_MODS=
SET PBO_PREFIX=emt
REM path to sources
SET MAIN_PATH=%CD%
REM path to private sources 

SET AddonBuilder=%TOOLS_PATH%\AddonBuilder
SET INCLUDES_PATH="%MAIN_PATH%\includes.txt"
SET PDRIVE=P:\%PBO_PREFIX%

REM Kill server if running
taskkill /F /IM DayZServer_x64.exe
REM Kill client if running
taskkill /F /IM DayZ_x64.exe

REM packing mod
for /d %%D in ("%PDRIVE%\*") do (
	REM check if folder had a config.cpp file
	if exist "%%~fD\config.cpp" (
		echo Building... %%~nxD
		START "AddonBuilder" /min /WAIT /d "%AddonBuilder%" "%AddonBuilder%\AddonBuilder.exe" "%PDRIVE%\%%~nxD" "%CLIENT_PATH%\addons" -clear -prefix="%PBO_PREFIX%\%%~nxD" -project=P:\ -include=%INCLUDES_PATH%
	)
)

if %errorlevel% neq 0 (
		echo "Some pbos failed to pack properly, check AddonBuilder logs"
		exit /B %errorlevel%
)

REM copy mod to server
xcopy /s/e/y/q/i/d "%CLIENT_PATH%" "%SERVER_PATH%"

REM start server with mod enabled
set /p start_server="Start Server: Y or N?"
if "%start_server%" == "Y" (
	START "StartServer" "%DAYZ_SERVER_PATH%\DayZServer_x64.exe" -config=serverDZ.cfg "-mod=%MOD_NAME%;%OTHER_MODS%" "-profiles=%SERVER_PROFILE_NAME%" -scrAllowFileWrite
)

REM start client with mod enabled and join server
set /p start_client="Start Client: Y or N?"
if "%start_client%" == "Y" (
	START "StartClient" "%DAYZ_CLIENT_PATH%\DayZ_x64.exe" "-mod=%MOD_NAME%;%OTHER_MODS%" -connect=127.0.0.1 -port=2302
)
