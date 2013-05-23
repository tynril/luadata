#ifndef _LUADATAIMPL_H_
#define _LUADATAIMPL_H_

#include <iostream>
#include <cstdint>
#include <vector>

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
	std::vector<luachunk> _chunks;

public:
	/** The constructor creates a new Lua state. */
	luadataimpl();

	/** The destructor releases the Lua state. */
	~luadataimpl();

	/** Loads a file (source or binary). */
	bool loadfile(const std::string &path, loadfilemode mode = automatic);

	/** Saves a binary file with all the loaded data. */
	bool savefile(const std::string &path);

	/** Get a future value bound to this data implementation. */
	luavalue get(const std::string &valuename);

	/** Gets the real value from a global Lua variable. */
	template<typename T> inline T retrieve(const std::string &valuename);

	/** Check if a global Lua variable is nil. */
	inline bool isnil(const std::string &valuename);

private:
	/** Loads a binary file and put it in a chunk with the given name. */
	bool loadbinaryfile(const std::string &path);

	/** Loads a source file and put it in a chunk with the given name. */
	bool loadsourcefile(const std::string &path);

	/** Gets a luavalue from the top of the stack. */
	template<typename T> inline T getfromstack();
	
	/** Static function used by lua_dump to  */
	static int luawriter(lua_State *L, const void *chunk, size_t size, void *userChunk);
};

/**
 * Abstract implementation of a Lua value.
 */
class luavalueimpl {
	const std::string _name;
	luadataimpl *_data;

public:
	/** Builds a new value promise, bound to the given data implementation. */
	luavalueimpl(const std::string &name, luadataimpl *data);

	/** Get the underlying value as a double. */
	double getdouble() const;

	/** Get the underlying value as an integer. */
	int getint() const;

	/** Get the underlying value as a string. */
	std::string getstring() const;

	/** Get the underlying value as a boolean. */
	bool getbool() const;

	/** Check if the value is a nil value. */
	bool isnil() const;

	/** Get the type of the underlying value. */
	luatype type() const;

private:

};

}} // namespaces

#endif // _LUADATAIMPL_H_
