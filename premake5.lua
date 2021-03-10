workspace "Hydro"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}

externalproject "SDL2"
	location "SDK/SDL2/VisualC/SDL"
	filename "SDL"
	kind "SharedLib"
	language "C++"
	
externalproject "SDL2main"
	location "SDK/SDL2/VisualC/SDLmain"
	filename "SDLmain"
	kind "SharedLib"
	language "C++"
	dependson
	{
		"SDL2",
	}
	
externalproject "SDL2_image"
	location "SDK/SDL2_image/VisualC"
	filename "SDL_image"
	kind "SharedLib"
	language "C++"
	dependson
	{
		"SDL2",
	}
	
externalproject "assimp"
	location "SDK/AssImp/build/code"
	filename "assimp"
	kind "SharedLib"
	language "C++"

project "Hydro"
	location "Hydro"
	kind "StaticLib"
	language "C++"
	
	targetdir ("Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("Build/temp/%{prj.name}/%{cfg.buildcfg}/")
	
	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/",
		"SDK/include",
		"SDK/SDL2/include",
		"SDK/SDL2_image/",
		"C:/VulkanSDK/1.2.141.2/Include",
		"SDK/AssImp/include",
		"SDK/AssImp/build/include",
		"SDK/glew/include",
	}
	
	libdirs
	{
		"Build/SDL2/%{cfg.buildcfg}/",
		"Build/SDL2main/%{cfg.buildcfg}/",
		"Build/SDL2_image/%{cfg.buildcfg}/",
		"C:/VulkanSDK/1.2.141.2/Lib/",
		"Build/assimp/%{cfg.buildcfg}/",
		"SDK/glew/lib/Release/x64",
	}
	
	dependson
	{
		"SDL2",
		"SDL2main",
		"SDL2_image",
		"assimp",
	}
	
	flags
	{
		"MultiProcessorCompile",
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"HYDRO_PLATFORM_WIN32",
		}
		
	filter "configurations:Debug"
		defines "HYDRO_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "HYDRO_RELEASE"
		optimize "On"
		
		flags
		{
			"LinkTimeOptimization",
		}
		
project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	dpiawareness "HighPerMonitor"
	
	targetdir ("Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("Build/temp//%{prj.name}/%{cfg.buildcfg}/%{prj.name}") 
	
	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/",
		"Hydro/",
		"SDK/include/",
		"SDK/SDL2/include/",
		"SDK/SDL2_image/",
		"C:/VulkanSDK/1.2.141.2/Include/",
		"SDK/AssImp/include",
		"SDK/AssImp/build/include",
		"SDK/glew/include",
	}
	
	libdirs
	{
		"Build/Hydro/%{cfg.buildcfg}/",
		"Build/SDL2/%{cfg.buildcfg}/",
		"Build/SDL2main/%{cfg.buildcfg}/",
		"Build/SDL2_image/%{cfg.buildcfg}/",
		"C:/VulkanSDK/1.2.141.2/Lib/",
		"Build/assimp/%{cfg.buildcfg}/",
		"SDK/glew/lib/Release/x64",
	}
	
	links
	{
		"Hydro.lib",
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"vulkan-1.lib",
		"assimp.lib",
		"glew32.lib",
		"opengl32.lib",
	}
	
	dependson
	{
		"Hydro",
		"SDL2",
		"SDL2main",
		"SDL2_image",
		"assimp",
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"HYDRO_PLATFORM_WIN32",
		}
		
		postbuildcommands
		{
			"echo D|xcopy \"$(SolutionDir)Build\\SDL2\\$(Configuration)\\*.*\" \"$(TargetDir)\" /y /E",
			"echo D|xcopy \"$(SolutionDir)Build\\SDL2_image\\$(Configuration)\\*.*\" \"$(TargetDir)\" /y /E",
			"echo D|xcopy \"$(SolutionDir)Build\\assimp\\$(Configuration)\\*.*\" \"$(TargetDir)\" /y /E",
			"echo D|xcopy \"$(SolutionDir)SDK\\glew\\bin\\Release\\$(Platform)\\glew32.dll\" \"$(TargetDir)\" /y /E",
			"echo D|xcopy \"$(SolutionDir)Resources\\*.*\" \"$(TargetDir)Resources\" /y /E",
		}
		
	filter "configurations:Debug"
		defines "HYDRO_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "HYDRO_RELEASE"
		optimize "On"
		
		flags
		{
			"LinkTimeOptimization"
		}