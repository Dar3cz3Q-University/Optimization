workspace "Optimization"
    architecture "x64"
    startproject "Optimization"

    configurations {
        "Debug",
        "Release",
        "Report"
    }

    filter { "system:windows", "action:vs2022" }
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

    filter "action:cmake"
        require "cmake"

    OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    group "Dependencies"
        include "Dependencies/googletest/googletest/premake5.lua"
    group ""

    include "Optimization/premake5.lua"
    include "Optimization_Tests/premake5.lua"
