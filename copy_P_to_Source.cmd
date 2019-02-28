@Echo Off
SETLOCAL enabledelayedexpansion

REM path to sources
SET MAIN_PATH=%CD%
SET SIDE_PATH=D:\Current Projects\GitHub\dayZ_SA_Epoch_internal

REM prefix path
SET PBO_PREFIX=emt
SET PDRIVE=P:\%PBO_PREFIX%

REM move files to p drive if folder has a config.cpp file
for /d %%D in ("%MAIN_PATH%\*") do (
	if exist "%%~fD\config.cpp" (
		if exist "%PDRIVE%\%%~nxD\config.cpp" (	
			REM xcopy /s/e/y/q/i/d %PDRIVE%\%%~nxD "%%~fD"
			echo Copying %PDRIVE%\%%~nxD to "%%~fD"
		)
	)
)
for /d %%D in ("%SIDE_PATH%\*") do (
	REM check if folder had a config.cpp file
	if exist "%%~fD\config.cpp" (
		if exist "%PDRIVE%\%%~nxD\config.cpp" (		
			REM xcopy /s/e/y/q/i/d %PDRIVE%\%%~nxD "%%~fD"
			echo Copying %PDRIVE%\%%~nxD to "%%~fD"
		)
	)
)