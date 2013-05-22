solution "lua-data"
	-- Configurations and platforms
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	
	-- Path to lua sources
	lua = "3rd-party/lua-5.2.2"
	
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
	
	-- Static lua-data library
	project "lua-data-static"
		kind "StaticLib"
		language "C++"
		
		-- Project files
		includedirs { "include" }
		files { "include/**.h", "src/**.cc" }
		
		-- Lua dependency
		includedirs { lua .. "/src" }
		links { "lua-static" }
	
	-- Shared lua-data library
	project "lua-data-shared"
		kind "SharedLib"
		language "C++"
		
		-- Project files
		includedirs { "include" }
		files { "include/**.h", "src/**.cc" }
		
		-- Lua dependency
		includedirs { lua .. "/src" }
		links { "lua-shared" }
	
	-- Lua dependency (static library)
	project "lua-static"
		language "C++"
		kind "StaticLib"
		
		files { lua .. "/src/*.h", lua .. "/src/*.hpp", lua .. "/src/*.c" }
		excludes { lua .. "/src/lua.c" }
	
	-- Lua dependency (shared library)
	project "lua-shared"
		language "C++"
		kind "SharedLib"
		
		defines { "LUA_BUILD_AS_DLL" }
		files { lua .. "/src/*.h", lua .. "/src/*.hpp", lua .. "/src/*.c" }
		excludes { lua .. "/src/lua.c" }
