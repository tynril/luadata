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

	luavalue get(const std::string &valuename);

private:
	/** Loads a binary file and put it in a chunk with the given name. */
	bool loadbinaryfile(const std::string &path);

	/** Loads a source file and put it in a chunk with the given name. */
	bool loadsourcefile(const std::string &path);

	/** Gets a luavalue from the top of the stack. */
	luavalueimpl * getfromstack();
	
	/** Static function used by lua_dump to  */
	static int luawriter(lua_State *L, const void *chunk, size_t size, void *userChunk);
};

/**
 * Abstract implementation of a Lua value.
 */
struct luavalueimpl {
	/** Get the underlying value as a double. */
	virtual double getdouble() const = 0;

	/** Get the underlying value as an integer. */
	virtual int getint() const = 0;

	/** Get the underlying value as a string. */
	virtual std::string getstring() const = 0;

	/** Get the underlying value as a boolean. */
	virtual bool getbool() const = 0;

	/** Get the type of the underlying value. */
	virtual luatype type() const = 0;
};

/** nil value */
struct nilluavalueimpl : public luavalueimpl {
	double getdouble() const { return 0.0; }
	int getint() const { return 0; }
	std::string getstring() const { return "nil"; }
	bool getbool() const { return false; }
	luatype type() const { return nil; }
};

/** boolean value */
struct boolluavalueimpl : public luavalueimpl {
	bool _v;
	boolluavalueimpl(bool v) : _v(v) {}
	double getdouble() const { return _v ? 1.0 : 0.0; }
	int getint() const { return _v ? 1 : 0; }
	std::string getstring() const { return _v ? "true" : "false"; }
	bool getbool() const { return _v; }
	luatype type() const { return boolean; }
};

/** number value */
struct numberluavalueimpl : public luavalueimpl {
	double _v;
	numberluavalueimpl(double v) : _v(v) {}
	double getdouble() const { return _v; }
	int getint() const { return (int)_v; }
	std::string getstring() const { return std::to_string(_v); }
	bool getbool() const { return _v != 0.0; }
	luatype type() const { return number; }
};

/** string value */
struct stringluavalueimpl : public luavalueimpl {
	std::string _v;
	stringluavalueimpl(std::string v) : _v(v) {}
	double getdouble() const { return 0.0; }
	int getint() const { return 0; }
	std::string getstring() const { return _v; }
	bool getbool() const { return !_v.empty(); }
	luatype type() const { return string; }
};

}} // namespaces

#endif // _LUADATAIMPL_H_
