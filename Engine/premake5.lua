-- ENGINE

project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"

    targetdir   "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
    objdir      "%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}"

    includedirs {
        "src/",
        "vendor/include"
    }

    libdirs {
        "vendor/lib"
    }

    links {
        "glfw3",
        "opengl32"
    }

    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.c"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"