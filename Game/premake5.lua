-- GAME

project "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"

    targetdir   "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
    objdir      "%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}"

    includedirs {
        "src/",
        "../Engine/include",
        "../Engine/src",
        "vendor/include"
    }

    --libdirs { }

    links {
        "Engine"
    }

    files {
        "src/**.h",
        "src/**.cpp"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"