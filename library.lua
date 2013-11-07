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

-- liblua dependency
project "lua"
	language "C++"
	kind "StaticLib"
	
	defines { "_CRT_SECURE_NO_WARNINGS" }
	files { lua .. "/src/*.h", lua .. "/src/*.hpp", lua .. "/src/*.c" }
	vpaths { ["*"] = lua }
	excludes { lua .. "/src/lua.c" }
