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

enum loadfilemode {
	automatic,
	source,
	binary
};

class LUADATA_API luavalue {
	impl::luavalueimpl *_pimpl;

public:
	~luavalue();

	operator double() const;
	operator int() const;
	operator std::string() const;
	operator bool() const;

	//luavalue& operator[](const std::string& valuename) const;

	luatype type() const;

private:
	luavalue(impl::luavalueimpl *impl);
	luavalue(const luavalue&);
	luavalue& operator=(const luavalue&);

	friend class impl::luadataimpl;
	friend class luadata;
};

/**
 * Access data stored in LUA files or preparsed LUA files.
 */
class LUADATA_API luadata {
	impl::luadataimpl *_pimpl;

public:
	luadata();
	~luadata();

	bool loadfile(const std::string &name, const std::string &path, loadfilemode mode = automatic);
	bool loadfile(const std::string &path, loadfilemode mode = automatic);

	luavalue operator[](const std::string& valuename) const;

private:
	luadata(const luadata&);
	const luadata& operator=(const luadata&);
};

} // namespace luadata

#endif // _LUADATA_H_