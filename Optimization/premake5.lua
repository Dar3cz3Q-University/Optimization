project "Optimization"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir ("%{_WORKING_DIR}/Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("%{_WORKING_DIR}/Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
   staticruntime "off"

   flags { "MultiProcessorCompile" }

   pchheader "pch.h"
   pchsource "src/pch.cpp"

   files { "src/**.h", "src/**.cpp" }

   includedirs {
       "src",
       "src/optimization",
       "src/utils",
       "src/utils/file_reader",
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

    filter "configurations:Report"
       defines { "NDEBUG", "ENABLE_SAVING_RESULTS" }
       runtime "Release"
       optimize "On"
       symbols "On"
