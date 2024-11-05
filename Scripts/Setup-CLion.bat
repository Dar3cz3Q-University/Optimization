@echo off

setlocal enabledelayedexpansion

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --scripts=Vendor/premake-cmake --file=premake5.lua cmake
pushd Optimization

if not exist Optimization.cmake (
    echo Plik 'Optimization.cmake' nie istnieje!
    exit /b 1
)

> temp.cmake (
    for /f "usebackq delims=" %%i in ("Optimization.cmake") do (
        set "line=%%i"
        echo !line! | findstr /L /C:"/NOLOGO" /C:"/DEBUG" > nul
        if errorlevel 1 (
            echo !line!
        )
    )
)

move /Y temp.cmake Optimization.cmake

popd
popd
pause
