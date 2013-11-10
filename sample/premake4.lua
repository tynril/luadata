-- Path to the luadata library.
luadata = "../"

-- Defining the solution.
solution "luadata-sample"
	-- Configurations and platforms
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	
	-- Start project
	startproject "luadata-sample"
	
	-- Global configuration
	location "projects"
	objdir "bin"
	
	-- Common configuration for 'Debug'
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetsuffix "_d"
	
	-- Common configuration for 'Release'
	configuration "Release"
		defines { "NDEBUG" }
		optimize "Full"
	
	-- Output folders
	configuration { "x32", "Debug" }
		targetdir "bin/x32/Debug"
	configuration { "x32", "Release" }
		targetdir "bin/x32/Release"
	configuration { "x64", "Debug" }
		targetdir "bin/x64/Debug"
	configuration { "x64", "Release" }
		targetdir "bin/x64/Release"
		
	-- Sample project
	project "luadata-sample"
		kind "ConsoleApp"
		language "C++"
		
		-- Project files
		files { "src/**.h", "src/**.cc" }
		debugdir "assets"
		
		-- Lua-data dependency
		includedirs { luadata .. "include" }
		links { "luadata" }
		
		configuration "gmake"
			-- Enabling the C++11 standard on Make
			buildoptions { "-std=c++11" }
	
	-- Link with the library.
	dofile(luadata .. "library.lua")
