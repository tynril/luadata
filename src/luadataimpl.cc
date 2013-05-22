#define LUADATA_LIB

#include "luadataimpl.h"

namespace luadata { namespace impl {;

luadataimpl::luadataimpl() :
	L(luaL_newstate())
{
	// Load Lua libraries.
	luaL_openlibs(L);
}

luadataimpl::~luadataimpl() {
	lua_close(L);
}

bool luadataimpl::loadfile(const std::string &path, loadfilemode mode) {
	// The chunk will be named after the file.
	std::string filename = std::string(std::find_if( path.rbegin(), path.rend(), pathseparatormatch()).base(), path.end());
	std::string::const_reverse_iterator pivot = std::find(filename.rbegin(), filename.rend(), '.');
    std::string name = pivot == filename.rend() ? filename : std::string(filename.begin(), pivot.base() - 1);
	return loadfile(name, path, mode);
}

bool luadataimpl::loadfile(const std::string &name, const std::string &path, loadfilemode mode) {
	// If the mode is automatic, it depends on file extension.
	if(mode == automatic) {
		if(path.rfind(".lua") == path.length() - 4)
			mode = source;
		else if(path.rfind(".lda") == path.length() - 4)
			mode = binary;
		else
			return false;
	}

	// Load in the right mode.
	if(mode == binary)
		return loadbinaryfile(name, path);
	else
		return loadsourcefile(name, path);
}

bool luadataimpl::loadbinaryfile(const std::string &name, const std::string &path) {
	// Not yet implemented.
	return false;
}

bool luadataimpl::loadsourcefile(const std::string &name, const std::string &path) {
	// Load the file in the state.
	if(luaL_loadfile(L, path.c_str()))
		return false;

	// There should be the chunk at the top of the stack now.
	if(lua_gettop(L) != 1 || !lua_isfunction(L, 1))
		return false;

	// Dump the binary values to a new chunk.
	luachunk chunk;
	lua_dump(L, &luadataimpl::luawriter, &chunk);
	chunk.shrink_to_fit();

	// Runs the script directly.
	if(lua_pcall(L, 0, 0, 0))
		return false;

	// Moves the chunk to its storage.
	_chunks[name] = std::move(chunk);

	return true;
}

luavalue luadataimpl::get(const std::string &valuename) {
	// Get the value and its type.
	lua_getglobal(L, valuename.c_str());
	int type = lua_type(L, -1);

	// Creates the proper implementation.
	luavalueimpl *vimpl = nullptr;

	// Loads the value with data depending on the type.
	switch(type) {
	case LUA_TNIL:
		vimpl = new nilluavalueimpl();
		break;
	case LUA_TBOOLEAN:
		vimpl = new boolluavalueimpl(lua_toboolean(L, -1) != 0);
		break;
	case LUA_TLIGHTUSERDATA:
		throw std::runtime_error("Not yet implemented.");
		break;
	case LUA_TNUMBER:
		vimpl = new numberluavalueimpl(lua_tonumber(L, -1));
		break;
	case LUA_TSTRING:
		vimpl = new stringluavalueimpl(lua_tostring(L, -1));
		break;
	case LUA_TTABLE:
		throw std::runtime_error("Not yet implemented.");
		break;
	case LUA_TFUNCTION:
		throw std::runtime_error("Not yet implemented.");
		break;
	case LUA_TUSERDATA:
		throw std::runtime_error("Not yet implemented.");
		break;
	case LUA_TTHREAD:
		throw std::runtime_error("Not yet implemented.");
		break;
	}

	if(vimpl == nullptr) {
		throw std::runtime_error("Unable to find a representation for the underlying type.");
	}

	// Removes the value from the stack.
	lua_pop(L, 1);

	return luavalue(vimpl);
}

int luadataimpl::luawriter(lua_State *L, const void *chunk, std::size_t size, void *userChunkPtr) {
	luachunk *userChunk = static_cast<luachunk*>(userChunkPtr);
	const uint8_t *newData = static_cast<const uint8_t*>(chunk);
	userChunk->insert(userChunk->end(), newData, newData + size);
	return 0;
}

}} // namespaces
