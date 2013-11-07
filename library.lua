-- Path to lua sources
lua = "3rd-party/lua-5.2.2"

-- Common configuration for x32/x64
configuration "x32"
	defines { "LUADATA_32" }
configuration "x64"
	defines { "LUADATA_64" }

-- libluadata library
project "luadata"
	kind "StaticLib"
	language "C++"
	
	-- Project files
	includedirs { "include" }
	files { "include/**.h", "src/**.h", "src/**.cc" }
	
	-- Lua dependency
	includedirs { lua .. "/src" }
	links { "lua" }
	
	-- Enabling the C++11 standard on Make
	configuration "gmake"
		buildoptions { "-std=c++11" }

-- liblua dependency
project "lua"
	language "C++"
	kind "StaticLib"
	
	-- Project files
	files { lua .. "/src/*.h", lua .. "/src/*.hpp", lua .. "/src/*.c" }
	excludes { lua .. "/src/lua.c" }
	vpaths { ["*"] = lua }
	
	-- Disable some warnings on Visual Studio
	configuration "vs*"
		defines { "_CRT_SECURE_NO_WARNINGS" }
