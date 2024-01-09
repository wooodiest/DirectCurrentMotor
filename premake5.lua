include "Dependencies.lua"

workspace "DirectCurrentMotor"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    flags { "MultiProcessorCompile" }
    startproject "DirectCurrentMotor"

    filter "system:windows"
    buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Luha/vendor/GLFW"
	include "Luha/vendor/Glad"
    include "Luha/vendor/imgui"
    include "Luha/vendor/yaml"
group ""

include "Luha"
include "DirectCurrentMotor"