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
	
	-- Lua dependency files
	includedirs { lua .. "/src" }
	files { lua .. "/src/*.h", lua .. "/src/*.hpp", lua .. "/src/*.c" }
	excludes { lua .. "/src/lua.c" }
	vpaths {
		["lua"] = lua,
		["src"] = "src",
		["include"] = "include"
	}
	
	-- Disable some deprecation warnings on Visual Studio
	configuration "vs*"
		defines{ "_CRT_SECURE_NO_WARNINGS" }
	
	-- Enabling the C++11 standard on Make
	configuration "gmake"
		buildoptions { "-std=c++0x" }
