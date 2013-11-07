solution "luadata"
	-- Configurations and platforms
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	
	-- Start project
	startproject "luadata-test"
	
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
	
	-- On Visual Studio 2012, the _VARIADIC_MAX is too low.
	configuration "vs2012"
		defines { "_VARIADIC_MAX=10" }
	
	-- Output folders
	configuration { "x32", "Debug" }
		targetdir "bin/x32/Debug"
	configuration { "x32", "Release" }
		targetdir "bin/x32/Release"
	configuration { "x64", "Debug" }
		targetdir "bin/x64/Debug"
	configuration { "x64", "Release" }
		targetdir "bin/x64/Release"
	
	-- Google Test dependency
	gtest = "3rd-party/gtest-1.7.0"
	project "gtest"
		kind "StaticLib"
		language "C++"
		
		-- Project files
		files { gtest .. "/**.h", gtest .. "/**.cc" }
		excludes { gtest .. "/src/gtest-all.cc" }
		includedirs { gtest, gtest .. "/include" }
	
	-- Test project
	project "luadata-test"
		kind "ConsoleApp"
		language "C++"
		
		-- Project files
		files { "test/**.h", "test/**.cc" }
		debugdir "test/resources"
		
		-- Lua-data dependency
		includedirs { "include", gtest .. "/include" }
		links { "luadata", "gtest" }
	
	-- Link with the library.
	dofile "library.lua"
