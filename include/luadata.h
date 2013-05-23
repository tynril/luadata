#ifndef _LUADATA_H_
#define _LUADATA_H_

#include <string>

#include "luadataconf.h"

namespace luadata {;

// Implementation classes forward declaration
namespace impl {
	class luadataimpl;
	class luavalueimpl;
}

enum luatype {
	nil,
	boolean,
	number,
	string,
	userdata,
	function,
	thread,
	table
};

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
class LUADATA_API luavalue {
	impl::luavalueimpl *_pimpl;
	unsigned int *_refCount;

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

	/** Gets the Lua type of the value. */
	luatype type() const;

private:
	/** Construction is done by the implementation. */
	luavalue(impl::luavalueimpl *impl);

	friend class impl::luadataimpl;
	friend class luadata;
	friend void swap(luavalue& lhs, luavalue& rhs);
};

/** Swap method for luavalues. */
void swap(luavalue& lhs, luavalue& rhs);

/**
 * This is the central class providing all features of the
 * lua-data library.
 */
class LUADATA_API luadata {
	impl::luadataimpl *_pimpl;

public:
	/** Creates a Lua data provider. */
	luadata();

	/** Destroys the Lua data provider. */
	~luadata();

	/** Loads a data file. */
	bool loadfile(const std::string &path, loadfilemode mode = automatic);

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