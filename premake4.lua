solution "luadata"
	-- Configurations and platforms
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	
	-- Global configuration
	location "projects"
	objdir "obj"
	
	-- Common configuration for 'Debug'
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetsuffix "_d"
	
	-- Common configuration for 'Release'
	configuration "Release"
		defines { "NDEBUG" }
		flags { "OptimizeSpeed" }
	
	-- Output folders
	configuration { "x32", "Debug" }
		targetdir "bin/x32/Debug"
	configuration { "x32", "Release" }
		targetdir "bin/x32/Release"
	configuration { "x64", "Debug" }
		targetdir "bin/x64/Debug"
	configuration { "x64", "Release" }
		targetdir "bin/x64/Release"
	
	-- Test project
	project "libluadata-test"
		kind "ConsoleApp"
		language "C++"
		
		-- Project files
		files { "test/**.h", "test/**.cc" }
		debugdir "test"
		
		-- Lua-data dependency
		includedirs { "include" }
		links { "libluadata" }
	
	-- Link with the library.
	dofile "library.lua"
