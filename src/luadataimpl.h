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

/** Union structure for path elements. */
enum luapathelementtype {
	index_t,
	key_t
};
struct luapathelement {
	luapathelement(std::string k) :
		type(key_t), key(k) {}
	luapathelement(int i) :
		type(index_t), index(i) {}
	luapathelementtype type;
	int index;
	std::string key;
};

/** Type definition for a value path. */
typedef std::vector<luapathelement> luapath;

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

	/** Gets the value of a Lua variable. */
	template<typename T> inline T retrieve(const luapath &valuepath);

	/** Gets the type of a Lua variable. */
	inline luatype type(const luapath &valuepath);

	/** Get the length of a Lua table, or 0 if it's not a table. */
	inline std::size_t tablelen(const luapath &valuepath);

	/** Gets a list of keys of a Lua associative table. */
	inline std::vector<std::string> tablekeys(const luapath &valuepath);

private:
	/** Loads a binary file and put it in a chunk with the given name. */
	bool loadbinaryfile(const std::string &path);

	/** Loads a source file and put it in a chunk with the given name. */
	bool loadsourcefile(const std::string &path);

	/** Puts a value from a path at the front of the stack. */
	inline void getpath(const luapath &valuepath);

	/** Clears anything still on the stack. */
	inline void clearstack();

	/** Gets a luavalue from the top of the stack. */
	template<typename T> inline T getfromstack();
	
	/** Static function used by lua_dump to  */
	static int luawriter(lua_State *L, const void *chunk, size_t size, void *userChunk);
};

/**
 * Abstract implementation of a Lua value.
 */
class luavalueimpl {
	const luapath _valuepath;
	luadataimpl *_data;

public:
	/** Builds a new value promise, bound to the given data implementation. */
	luavalueimpl(const luapath &valuepath, luadataimpl *data);

	/** Get the underlying value as a double. */
	double getdouble() const;

	/** Get the underlying value as an integer. */
	int getint() const;

	/** Get the underlying value as a string. */
	std::string getstring() const;

	/** Get the underlying value as a boolean. */
	bool getbool() const;

	/** Length of the table (if it's a table). */
	std::size_t tablelen() const;

	/** Keys of the associative table. */
	std::vector<std::string> tablekeys() const;

	/** Value of an associative table. */
	luavalue tableval(const std::string &keyval) const;

	/** Value of a non-associative table. */
	luavalue tableval(const int &keyval) const;

	/** Get the type of the underlying value. */
	luatype type() const;
};

}} // namespaces

#endif // _LUADATAIMPL_H_
