#ifndef _LUADATA_H_
#define _LUADATA_H_

#include <string>
#include <vector>

namespace luadata {;

// Implementation classes forward declaration
namespace impl {
	class luadataimpl;
}

/** Lua types that a luavalue can contain. */
enum luatype {
	lua_nil,
	lua_boolean,
	lua_number,
	lua_string,
	lua_userdata,
	lua_function,
	lua_thread,
	lua_table
};

/** Structure that contains an argument to be passed to a Lua function. */
class luaarg {
	/** Underlying type of the argument. */
	enum {
		a_undefined,
		a_boolean,
		a_double,
		a_integer,
		a_string
	} type;

	/** Union to store non-string content. */
	union {
		bool b;
		double d;
		int n;
	} simple_content;

	/** String member to store string content. */
	std::string string_content;

public:
	/** Implicit construction from a boolean value. */
	luaarg(const bool &b) : type(a_boolean)	 { simple_content.b = b; }

	/** Implicit construction from a double value. */
	luaarg(const double &d) : type(a_double) { simple_content.d = d; }

	/** Implicit construction from an integer value. */
	luaarg(const int &n) : type(a_integer)   { simple_content.n = n; }

	/** Implicit construction from a string value. */
	luaarg(const std::string &s) : type(a_string), string_content(s) {}

	friend class impl::luadataimpl;
};

/** Structure for Lua tables keys. */
class luakey {
	enum {
		p_name,
		p_index
	} type;
	std::string name;
	int index;

public:
	luakey(const char * k) : type(p_name), name(k) {}
	luakey(std::string k) : type(p_name), name(k) {}
	luakey(int i) : type(p_index), index(i) {}

	friend class luadata;
	friend class impl::luadataimpl;
	friend std::ostream& operator<<(std::ostream& os, const luakey& key);
};

/** Print utility for a Lua key. */
std::ostream& operator<<(std::ostream& os, const luakey& key);

/** Structure for path elements. */
class luapathelement {
	luakey key;
	std::vector<luaarg> args;

	luapathelement(luakey k) : key(k) {}

	friend class luavalue;
	friend class luadata;
	friend class impl::luadataimpl;
};

/** Structure for a value path. */
typedef std::vector<luapathelement> luapath;

/**
 * The different modes to open a data file.
 */
enum loadfilemode {
	/** Choose mode by looking at the extension (lua for source, anything else for data). */
	automatic,

	/** Loads the file as Lua source. */
	source,

	/** Loads the file as data binary. */
	binary
};

/**
 * Represents a single value retrieved from Lua.
 */
class luavalue {
	impl::luadataimpl *_pimpl;
	luapath _valuepath;

public:
	luavalue(const luavalue &other);
	~luavalue();

	/** Assignation. */
	luavalue& operator=(const luavalue& rhs);

#if (defined LUADATA_LIB || defined LUADATA_IMPLICIT_CAST)
	/** Gets the value with an implicit casting. */
	operator double() const;
	operator int() const;
	operator std::string() const;
	operator bool() const;
#endif

	/** Gets the value with an explicit casting. */
	double asdouble() const;
	int asint() const;
	std::string asstring() const;
	bool asbool() const;

	/** Gets the length of the (unassociative) table, or 0
	    if it is an associative table or another value. */
	std::size_t tablelen() const;

	/** Gets the list of the keys in the associative table,
	    or an empty vector for another value. */
	std::vector<luakey> tablekeys() const;

	/** Gets the value at the given table key. */
	luavalue operator[](const luakey &key) const;

	/** Gets the value returned by the function. */
	luavalue operator()() const;
	luavalue operator()(luaarg arg0) const;
	luavalue operator()(luaarg arg0, luaarg arg1) const;
	luavalue operator()(luaarg arg0, luaarg arg1, luaarg arg2) const;
	luavalue operator()(luaarg arg0, luaarg arg1, luaarg arg2, luaarg arg3) const;
	luavalue operator()(luaarg arg0, luaarg arg1, luaarg arg2, luaarg arg3, luaarg arg4) const;
	luavalue operator()(const std::vector<luaarg> &args) const;

	/** Gets the Lua type of the value. */
	luatype type() const;

private:
	/** Construction is done by the implementation. */
	luavalue(const luapath &valuepath, impl::luadataimpl *impl);

	friend class impl::luadataimpl;
	friend class luadata;
	friend void swap(luavalue& lhs, luavalue& rhs);
};

/** Print a luavalue, including tables and stuff. */
std::ostream& operator<<(std::ostream& os, const luavalue& val);

/** Swap method for luavalues. */
void swap(luavalue& lhs, luavalue& rhs);

/**
 * This is the central class providing all features of the
 * lua-data library.
 */
class luadata {
	impl::luadataimpl *_pimpl;

public:
	/** Creates a Lua data provider. */
	luadata();

	/** Destroys the Lua data provider. */
	~luadata();

	/** Loads a data file. */
	bool loadfile(const std::string &path, loadfilemode mode = automatic);

	/** Loads a string containing Lua source code. */
	bool loadcode(const std::string &luacode);

	/** Saves every loaded data files to a binary data file. */
	bool savefile(const std::string &path);

	/** Get a value in the data tree. */
	luavalue operator[](const std::string& name) const;

private:
	/** Copy is disabled. */
	luadata(const luadata&);

	/** Copy is disabled. */
	const luadata& operator=(const luadata&);
};

} // namespace luadata

#endif // _LUADATA_H_
