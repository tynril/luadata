#ifndef _LUADATA_H_
#define _LUADATA_H_

#include <string>
#include <vector>
#include <memory>

namespace luadata {;

// Implementation classes forward declaration
namespace impl {
	/** Implementation of the luadata system. */
	class luadataimpl;
}

/** Lua types that a luavalue can contain. */
enum class luatype {
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

// Comparator forward declaration.
struct luakeycomparator;

/** Structure for Lua tables keys. */
class luakey {
	enum {
		p_undefined,
		p_name,
		p_index
	} type;
	std::string name;
	int index;

public:
	luakey(const char * k) : type(p_name), name(k) {}
	luakey(std::string k) : type(p_name), name(k) {}
	luakey(int i) : type(p_index), index(i) {}
	luakey() : type(p_undefined) {}

	const std::string str() const {
		return type == p_name ? name : std::to_string(index);
	}

	friend class impl::luadataimpl;
	friend struct luakeycomparator;
};

/** Comparison utility for lua keys. */
struct luakeycomparator {
	bool operator()(const luakey& lhs, const luakey& rhs) const {
		// Indexed keys are sorted before named keys.
		if(lhs.type != rhs.type) {
			if(lhs.type == luakey::p_index) return true;
			return false;
		}

		// Indexed keys sort.
		if(lhs.type == luakey::p_index)
			return lhs.index < rhs.index;

		// Named keys sort.
		if(lhs.type == luakey::p_name)
			return lhs.name < rhs.name;

		// Unreachable default (or undefined keys).
		return false;
	}
};

/** Print utility for a Lua key. */
std::ostream& operator<<(std::ostream& os, const luakey& key);

// luavalue forward declaration
class luavalue;

/** More implementation details. */
namespace impl {
	/** Structure for path elements. */
	struct luapathelement {
		luakey key;
		std::vector<luaarg> args;

		luapathelement(luakey k) : key(k) {}
	};

	/** Structure for a value path. */
	struct luapath {
		std::vector<luapathelement> path;
		std::shared_ptr<std::vector<std::pair<luakey, luavalue>>> keys_cache;
		int keys_cache_state;

		explicit luapath(luapathelement element) :
			path(1, element),
			keys_cache(nullptr)
		{}

		explicit luapath(std::vector<luapathelement> path) :
			path(path),
			keys_cache(nullptr)
		{}
	};
}

/**
 * Represents a single value retrieved from Lua.
 */
class luavalue {
	impl::luadataimpl *_pimpl;
	impl::luapath _valuepath;

public:
	luavalue(const luavalue &other);
	luavalue(luavalue &&other);
	~luavalue();

	/** Assignation. */
	luavalue& operator=(const luavalue& rhs);

#if defined(LUADATA_LIB) || defined(LUADATA_IMPLICIT_CAST)
	/** Gets the value with an implicit casting. */
	operator double() const;
	operator std::ptrdiff_t() const;
	operator std::string() const;
	operator bool() const;
#endif

	/** Gets the value with an explicit casting. */
	double asdouble() const;
	double asdouble(const double &defaultValue) const;
	std::ptrdiff_t asint() const;
	std::ptrdiff_t asint(const std::ptrdiff_t &defaultValue) const;
	std::string asstring() const;
	std::string asstring(const std::string &defaultValue) const;
	bool asbool() const;
	bool asbool(const bool &defaultValue) const;

	/** Gets the length of the (unassociative) table, or 0
	    if it is an associative table or another value. */
	std::ptrdiff_t tablelen() const;

	/** Gets the list of the keys in the associative table,
	    or an empty vector for another value. */
	std::vector<luakey> tablekeys() const;

	/** Gets the value at the given table key. */
	luavalue operator[](const luakey &key) const;

	/** Iterator functions for tables. */
	std::vector<std::pair<luakey, luavalue>>::const_iterator begin();
	std::vector<std::pair<luakey, luavalue>>::const_iterator end();

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
	luavalue(const impl::luapath &valuepath, impl::luadataimpl *impl);

	friend class luadata;
	friend class impl::luadataimpl;
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

	/** Moves a Lua data provider. */
	luadata(luadata &&other);

	/** Destroys the Lua data provider. */
	~luadata();

	/** Loads a data file. */
	bool loadfile(const std::string &path);

	/** Loads a string containing Lua source code. */
	bool loadcode(const std::string &luacode);

	/** Call this function to process hot-reloading if needed. */
	void hotreload(bool force = false);

	/** Gets the list of the keys in the data tree. */
	std::vector<luakey> keys() const;

	/** Get a value in the data tree. */
	luavalue operator[](const luakey& name) const;

	/** Iterator over the keys of the data tree. */
	std::vector<std::pair<luakey, luavalue>>::const_iterator begin();
	std::vector<std::pair<luakey, luavalue>>::const_iterator end();

private:
	/** Copy is disabled. */
	luadata(const luadata&);

	/** Copy is disabled. */
	const luadata& operator=(const luadata&);
};

} // namespace luadata

#endif // _LUADATA_H_
