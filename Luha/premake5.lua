project "Luha"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "lhpch.h"
	pchsource "src/lhpch.cpp"

	files
    {
        "src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/implot/**.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE",
		"_USE_MATH_DEFINES"
	}

	includedirs
    {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImPlot}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.OBJ_Loader}"
	}

	links 
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui",
		"yaml-cpp"
	}

	filter "system:windows"
        systemversion "latest"
		defines
        { "GLFW_INCLUDE_NONE" }

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