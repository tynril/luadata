#ifndef _LUADATA_H_
#define _LUADATA_H_

#include <string>

#include "luadataconf.h"

namespace luadata {;

// Implementation classes forward declaration
namespace impl {
	class luadataimpl;
	struct luavalueimpl;
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

public:
	~luavalue();

	/** Gets the value as a double. */
	operator double() const;

	/** Gets the value as an integer. */
	operator int() const;

	/** Gets the value as a string. */
	operator std::string() const;

	/** Gets the value as a boolean. */
	operator bool() const;

	//luavalue& operator[](const std::string& valuename) const;

	/** Gets the Lua type of the value. */
	luatype type() const;

private:
	/** Construction is done by the implementation. */
	luavalue(impl::luavalueimpl *impl);

	/** Copy is forbidden. */
	luavalue(const luavalue&);
	luavalue& operator=(const luavalue&);

	friend class impl::luadataimpl;
	friend class luadata;
};

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

	/** Access a value defined in one of the loaded files. */
	luavalue operator[](const std::string& valuename) const;

private:
	/** Copy is disabled. */
	luadata(const luadata&);

	/** Copy is disabled. */
	const luadata& operator=(const luadata&);
};

} // namespace luadata

#endif // _LUADATA_H_