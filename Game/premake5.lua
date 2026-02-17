-- GAME

project "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"

    targetdir   "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
    objdir      "%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}"
    
    -- Relative paths for Assets
    debugdir "%{cfg.targetdir}"

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

    postbuildcommands {
        "{COPYDIR} %{wks.location}/Assets %{cfg.targetdir}/Assets",
        "{COPY} %{wks.location}/Engine/vendor/bin/assimp.dll %{cfg.targetdir}",
        "{COPY} %{wks.location}/Engine/vendor/bin/assimp.pdb %{cfg.targetdir}"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"