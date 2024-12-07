project "Optimization_Tests"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir ("%{_WORKING_DIR}/Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("%{_WORKING_DIR}/Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
   staticruntime "off"

   flags { "MultiProcessorCompile" }

   pchheader "pch.h"
   pchsource "../Optimization/src/pch.cpp"

   files { 
    "tests/**.cpp",

    "../Optimization/src/**.h",
    "../Optimization/src/**.cpp"
   }

   removefiles {
       "../Optimization/src/main.cpp"
   }

   includedirs {
    "../Dependencies/googletest/googletest/include",

    "../Optimization/src",
    "../Optimization/src/optimization",
    "../Optimization/src/utils",
    "../Optimization/src/utils/file_reader",
    "../Optimization/src/utils/file_reader/read_data_types"
   }

   links {
    "GoogleTest"
   }

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "NDEBUG" }
       runtime "Release"
       optimize "On"
       symbols "On"
