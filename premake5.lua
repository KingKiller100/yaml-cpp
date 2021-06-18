project "YAML"
	kind "StaticLib"
	language "C++"
    characterset ("default")
	staticruntime "on"
	
	targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")
	
	files
	{
		"include/**.h",
		"src/**.h",
		"src/**.cpp",
	}
	
	includedirs
	{
		"include",
	}
	
	filter "system:windows"
		systemversion "latest"
		cppdialect "c++latest"
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Test"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Profile"
		runtime "Release"
		optimize "Speed"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "Full"
		
