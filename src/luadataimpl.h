#ifndef _LUADATAIMPL_H_
#define _LUADATAIMPL_H_

#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "luadata.h"

namespace luadata { namespace impl {;

/** Structure defining the path separator depending on the OS. */
struct pathseparatormatch {
	bool operator()(char ch) const {
#ifdef _WIN32
		return ch == '\\' || ch == '/';
#else
		return ch == '/';
#endif
	}
};

/** Type definition for binary chunk data. */
typedef std::vector<uint8_t> luachunk;

/**
 * Implementation of the luadata library.
 */
class luadataimpl {
	/** Lua state pointer. */
	lua_State *L;

	/** Map holding all loaded chunks. */
	std::unordered_map<std::string, luachunk> _chunks;

public:
	/** The constructor creates a new Lua state. */
	luadataimpl();

	/** The destructor releases the Lua state. */
	~luadataimpl();

	/** Loads a file (source or binary). */
	bool loadfile(const std::string &path);

	/** Loads a string containing Lua code. */
	bool loadcode(const std::string &code);

	/** Dump chunks in a binary format to a stream. */
	void dump(const std::string &name, std::ostream &out);

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
	
	/** Static function used by lua_dump to  */
	static int luawriter(lua_State *L, const void *chunk, size_t size, void *userChunk);
};

}} // namespaces

#endif // _LUADATAIMPL_H_
