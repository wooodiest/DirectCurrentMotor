project "DirectCurrentMotor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
    {
        "src/**.h",
		"src/**.cpp"
	}

	includedirs
    {
		"src",
		"%{wks.location}/Luha/src",
		"%{wks.location}/Luha/vendor",
		"%{wks.location}/Luha/vendor/spdlog/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImPlot}",
		"%{IncludeDir.yaml_cpp}"
	}

	links
	{
		"Luha"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
        systemversion "latest"

		filter "configurations:Debug"
		defines "LH_DEBUG"
		runtime "Debug"
		symbols "On"

		filter "configurations:Release"
		defines "LH_RELEASE"
		runtime "Release"
		optimize "on"
		symbols "On"

		filter "configurations:Dist"
		defines "LH_DIST"
		runtime "Release"
		optimize "on"
		symbols "Off"
