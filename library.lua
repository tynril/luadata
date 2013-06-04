-- Path to lua sources
lua = "3rd-party/lua-5.2.2"

-- libluadata library
project "libluadata"
	kind "StaticLib"
	language "C++"
	
	-- Project files
	includedirs { "include" }
	files { "include/**.h", "src/**.h", "src/**.cc" }
	
	-- Lua dependency
	includedirs { lua .. "/src" }
	links { "liblua" }

-- liblua dependency
project "liblua"
	language "C++"
	kind "StaticLib"
	
	defines { "_CRT_SECURE_NO_WARNINGS" }
	files { lua .. "/src/*.h", lua .. "/src/*.hpp", lua .. "/src/*.c" }
	vpaths { ["*"] = lua }
	excludes { lua .. "/src/lua.c" }