#ifndef _LUADATAIMPL_H_
#define _LUADATAIMPL_H_

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "luadata.h"

namespace luadata { namespace impl {;

/**
 * Implementation of the luadata library.
 */
class luadataimpl {
	/** Lua state pointer. */
	lua_State *L;

	/** Lists of loaded files. */
	std::vector<std::pair<std::string, time_t>> _loadedFiles;

public:
	/** The constructor creates a new Lua state. */
	luadataimpl();

	/** The destructor releases the Lua state. */
	~luadataimpl();

	/** Loads a Lua file (source code or precompiled). */
	bool loadfile(const std::string &path);

	/** Loads a string containing Lua code. */
	bool loadcode(const std::string &code);

	/** Do the hotreloading if needed. */
	void hotreload();

	/** Gets the value of a Lua variable. */
	double retrievedouble(const luapath &valuepath);
	int retrieveint(const luapath &valuepath);
	std::string retrievestring(const luapath &valuepath);
	bool retrievebool(const luapath &valuepath);

	/** Gets the type of a Lua variable. */
	luatype type(const luapath &valuepath);

	/** Get the length of a Lua table, or 0 if it's not a table. */
	std::size_t tablelen(const luapath &valuepath);

	/** Gets a list of keys of a Lua associative table. */
	std::vector<luakey> tablekeys(const luapath &valuepath);

private:
	/** Process a loaded chunk. */
	bool processloadedchunk();

	/** Puts a value from a path at the front of the stack, returns the number of pushed elements. */
	inline void getpath(const luapath &valuepath);

	/** Calls the function at the top of the stack with the given arguments. */
	inline void callfunction(const std::vector<luaarg> &args);

	/** Gets a value from the top of the stack. */
	double getdoublefromstack();
	int getintfromstack();
	std::string getstringfromstack();
	bool getboolfromstack();
};

/** Last modification date of the file. */
time_t getmodtime(const std::string &filepath);

}} // namespaces

#endif // _LUADATAIMPL_H_
