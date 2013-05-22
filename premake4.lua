solution "lua-data"
	-- Configurations and platforms
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	
	-- Path to lua sources
	lua = "3rd-party/lua-5.2.2"
	
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
	
	-- Static lua-data library
	project "lua-data-static"
		kind "StaticLib"
		language "C++"
		
		-- Project files
		includedirs { "include" }
		files { "include/**.h", "src/**.h", "src/**.cc" }
		
		-- Lua dependency
		includedirs { lua .. "/src" }
		links { "lua-static" }
		
		-- Output folders
		configuration { "x32", "Debug" }
			targetdir "bin/x32/Debug"
		configuration { "x32", "Release" }
			targetdir "bin/x32/Release"
		configuration { "x64", "Debug" }
			targetdir "bin/x64/Debug"
		configuration { "x64", "Release" }
			targetdir "bin/x64/Release"
	
	-- Shared lua-data library
	project "lua-data-shared"
		kind "SharedLib"
		language "C++"
		
		-- Project files
		defines { "LUADATA_BUILD_AS_DLL" }
		includedirs { "include" }
		files { "include/**.h", "src/**.h", "src/**.cc" }
		
		-- Lua dependency
		includedirs { lua .. "/src" }
		links { "lua-shared" }
		
		-- Output folders
		configuration { "x32", "Debug" }
			targetdir "bin/x32/Debug"
		configuration { "x32", "Release" }
			targetdir "bin/x32/Release"
		configuration { "x64", "Debug" }
			targetdir "bin/x64/Debug"
		configuration { "x64", "Release" }
			targetdir "bin/x64/Release"
	
	-- Test project (statically linked)
	project "lua-data-test-static"
		kind "ConsoleApp"
		language "C++"
		
		-- Project files
		files { "test/**.h", "test/**.cc" }
		
		-- Lua-data dependency
		includedirs { "include" }
		links { "lua-data-static" }
		
		-- Output folders
		configuration { "x32", "Debug" }
			targetdir "bin/x32/Debug/test"
		configuration { "x32", "Release" }
			targetdir "bin/x32/Release/test"
		configuration { "x64", "Debug" }
			targetdir "bin/x64/Debug/test"
		configuration { "x64", "Release" }
			targetdir "bin/x64/Release/test"
	
	-- Test project (dynamically linked)
	project "lua-data-test-shared"
		kind "ConsoleApp"
		language "C++"
		
		-- Project files
		files { "test/**.h", "test/**.cc" }
		
		-- Lua-data dependency
		includedirs { "include" }
		links { "lua-data-shared" }
		
		-- Output folders
		configuration { "x32", "Debug" }
			targetdir "bin/x32/Debug/test"
		configuration { "x32", "Release" }
			targetdir "bin/x32/Release/test"
		configuration { "x64", "Debug" }
			targetdir "bin/x64/Debug/test"
		configuration { "x64", "Release" }
			targetdir "bin/x64/Release/test"
	
	-- Lua dependency (static library)
	project "lua-static"
		language "C++"
		kind "StaticLib"
		
		defines { "_CRT_SECURE_NO_WARNINGS" }
		files { lua .. "/src/*.h", lua .. "/src/*.hpp", lua .. "/src/*.c" }
		excludes { lua .. "/src/lua.c" }
		
		-- Output folders
		configuration { "x32", "Debug" }
			targetdir "bin/x32/Debug/3rd-party"
		configuration { "x32", "Release" }
			targetdir "bin/x32/Release/3rd-party"
		configuration { "x64", "Debug" }
			targetdir "bin/x64/Debug/3rd-party"
		configuration { "x64", "Release" }
			targetdir "bin/x64/Release/3rd-party"
	
	-- Lua dependency (shared library)
	project "lua-shared"
		language "C++"
		kind "SharedLib"
		
		defines { "_CRT_SECURE_NO_WARNINGS", "LUA_BUILD_AS_DLL" }
		files { lua .. "/src/*.h", lua .. "/src/*.hpp", lua .. "/src/*.c" }
		excludes { lua .. "/src/lua.c" }
		
		-- Output folders
		configuration { "x32", "Debug" }
			targetdir "bin/x32/Debug/3rd-party"
		configuration { "x32", "Release" }
			targetdir "bin/x32/Release/3rd-party"
		configuration { "x64", "Debug" }
			targetdir "bin/x64/Debug/3rd-party"
		configuration { "x64", "Release" }
			targetdir "bin/x64/Release/3rd-party"
