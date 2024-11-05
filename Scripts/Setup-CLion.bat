@echo off

setlocal enabledelayedexpansion

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --scripts=Vendor/premake-cmake --file=premake5.lua cmake
pushd Optimization

echo Running some manual changes to CMake files. Please do not close this window!

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

> temp.cmake (
    for /f "usebackq delims=" %%i in ("Optimization.cmake") do (
        set "line=%%i"
        echo !line! | findstr /L /C:"target_precompile_headers(\"Optimization\" PUBLIC src/pch.h)" > nul
        if not errorlevel 1 (
            echo target_precompile_headers^("Optimization" PUBLIC Optimization/src/pch.h^)
        ) else (
            echo !line!
        )
    )
)
move /Y temp.cmake Optimization.cmake

popd
pushd Optimization_Tests

if not exist Optimization_Tests.cmake (
    echo Plik 'Optimization_Tests.cmake' nie istnieje!
    exit /b 1
)

> temp.cmake (
    for /f "usebackq delims=" %%i in ("Optimization_Tests.cmake") do (
        set "line=%%i"
        echo !line! | findstr /L /C:"/NOLOGO" /C:"$<$<CONFIG:Release>:/DEBUG>" /C:"$<$<CONFIG:Debug>:/DEBUG>" > nul
        if errorlevel 1 (
            echo !line!
        )
    )
)
move /Y temp.cmake Optimization_Tests.cmake

> temp.cmake (
    for /f "usebackq delims=" %%i in ("Optimization_Tests.cmake") do (
        set "line=%%i"
        echo !line! | findstr /L /C:"target_precompile_headers(\"Optimization_Tests\" PUBLIC ../Optimization/src/pch.h)" > nul
        if not errorlevel 1 (
            echo target_precompile_headers^("Optimization_Tests" PUBLIC Optimization/src/pch.h^)
        ) else (
            echo !line!
        )
    )
)
move /Y temp.cmake Optimization_Tests.cmake

endlocal

popd
popd

echo Should be done now. You can close the window

pause
