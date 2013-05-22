#ifndef _LUADATA_H_
#define _LUADATA_H_

#include <string>

#include "luadataconf.h"

namespace luadata {;

// Implementation classes forward declaration
class luadataimpl;
class luavalueimpl;

LUADATA_API enum luatype {
	nil,
	boolean,
	number,
	string,
	userdata,
	function,
	thread,
	table
};

class LUADATA_API luavalue {
	luavalueimpl *_pimpl;

public:
	operator float() const;
	operator double() const;
	operator int() const;
	operator std::string() const;
	operator bool() const;

	luavalue& operator[](const std::string& valuename) const;

	luatype type() const;
};

/**
 * Access data stored in LUA files or preparsed LUA files.
 */
class LUADATA_API luadata {
	luadataimpl *_pimpl;

public:
	luadata(const std::string &file);

	luavalue& operator[](const std::string& valuename) const;

private:
	luadata(const luadata&);
	const luadata& operator=(const luadata&);
};

static luavalue __val__;

} // namespace luadata

#endif // _LUADATA_H_