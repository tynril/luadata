solution "lua-data"
	-- Configurations and platforms
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	
	-- Global configuration
	location "_projects"
	objdir "_build/obj"
	
	-- Common configuration for 'Debug'
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetsuffix "_d"
	
	-- Common configuration for 'Release'
	configuration "Release"
		defines { "NDEBUG" }
		flags { "OptimizeSpeed" }
	
	-- Common platform-specific configurations
	configuration { "x32", "Debug" }
		targetdir "_build/bin/x32/Debug"
	configuration { "x32", "Release" }
		targetdir "_build/bin/x32/Release"
	configuration { "x64", "Debug" }
		targetdir "_build/bin/x64/Debug"
	configuration { "x64", "Release" }
		targetdir "_build/bin/x64/Release"
	
	-- Library project
	project "lua-data"
		kind "StaticLib"
		language "C++"
		
		-- Project files
		includedirs { "include" }
		files { "include/**.h", "src/**.cc" }
		
		-- Lua dependency
		includedirs { "3rd-party/include" }
		links { "lua52" }
		configuration { "windows", "x32" }
			libdirs { "3rd-party/lib/win32" }
		configuration { "windows", "x64" }
			libdirs { "3rd-party/lib/win64" }
