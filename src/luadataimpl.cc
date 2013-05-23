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
	// If the mode is automatic, it depends on file extension.
	if(mode == automatic) {
		if(path.rfind(".lua") == path.length() - 4)
			mode = source;
		else
			mode = binary;
	}

	// Load in the right mode.
	if(mode == binary)
		return loadbinaryfile(path);
	else
		return loadsourcefile(path);
}

bool luadataimpl::savefile(const std::string &path) {
	// Not yet implemented.
	return false;
}

bool luadataimpl::loadbinaryfile(const std::string &path) {
	// Not yet implemented.
	return false;
}

bool luadataimpl::loadsourcefile(const std::string &path) {
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
	_chunks.push_back(std::move(chunk));

	return true;
}

luavalue luadataimpl::get(const std::string &valuename) {
	return luavalue(new luavalueimpl(valuename, this));
}

template<typename T>
inline T luadataimpl::retrieve(const std::string &valuename) {
	// Put the value at the top of the stack.
	lua_getglobal(L, valuename.c_str());

	// Read it.
	T value = getfromstack<T>();

	// Removes the value from the stack.
	lua_pop(L, 1);

	// And returns it.
	return value;
}

template<>
inline double luadataimpl::getfromstack() {
	// Get the value type.
	int type = lua_type(L, -1);

	// Loads the value with data depending on the type.
	switch(type) {
	case LUA_TBOOLEAN:
		return lua_toboolean(L, -1) != 0 ? 1.0 : 0.0;
	case LUA_TNUMBER:
		return lua_tonumber(L, -1);
	case LUA_TSTRING:
		return 0.0;
	case LUA_TTABLE:
		throw std::runtime_error("Not yet implemented.");
	case LUA_TFUNCTION:
		// Call the function and gets its 1st result.
		lua_pcall(L, 0, 1, 0);
		return getfromstack<double>();
	case LUA_TNIL:
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:
	case LUA_TTHREAD:
	default:
		return 0.0;
	}
}

template<>
inline int luadataimpl::getfromstack() {
	// Get the value type.
	int type = lua_type(L, -1);

	// Loads the value with data depending on the type.
	switch(type) {
	case LUA_TBOOLEAN:
		return lua_toboolean(L, -1) != 0 ? 1 : 0;
	case LUA_TNUMBER:
		return (int)lua_tonumber(L, -1);
	case LUA_TSTRING:
		return 0;
	case LUA_TTABLE:
		throw std::runtime_error("Not yet implemented.");
	case LUA_TFUNCTION:
		// Call the function and gets its 1st result.
		lua_pcall(L, 0, 1, 0);
		return getfromstack<int>();
	case LUA_TNIL:
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:
	case LUA_TTHREAD:
	default:
		return 0;
	}
}

template<>
inline std::string luadataimpl::getfromstack() {
	// Get the value type.
	int type = lua_type(L, -1);

	// Loads the value with data depending on the type.
	switch(type) {
	case LUA_TBOOLEAN:
		return lua_toboolean(L, -1) != 0 ? "true" : "false";
	case LUA_TNUMBER:
		return std::to_string(lua_tonumber(L, -1));
	case LUA_TSTRING:
		return lua_tostring(L, -1);
	case LUA_TTABLE:
		throw std::runtime_error("Not yet implemented.");
	case LUA_TFUNCTION:
		// Call the function and gets its 1st result.
		lua_pcall(L, 0, 1, 0);
		return getfromstack<std::string>();
	case LUA_TNIL:
		return "nil";
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:
	case LUA_TTHREAD:
	default:
		return 0;
	}
}

template<>
inline bool luadataimpl::getfromstack() {
	// Get the value type.
	int type = lua_type(L, -1);

	// Loads the value with data depending on the type.
	switch(type) {
	case LUA_TBOOLEAN:
		return lua_toboolean(L, -1) != 0;
	case LUA_TNUMBER:
		return lua_tonumber(L, -1) != 0;
	case LUA_TSTRING:
		return !std::string(lua_tostring(L, -1)).empty();
	case LUA_TTABLE:
		throw std::runtime_error("Not yet implemented.");
	case LUA_TFUNCTION:
		// Call the function and gets its 1st result.
		lua_pcall(L, 0, 1, 0);
		return getfromstack<bool>();
	case LUA_TNIL:
		return false;
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:
	case LUA_TTHREAD:
	default:
		return 0;
	}
}

inline bool luadataimpl::isnil(const std::string &valuename) {
	// Put the value at the top of the stack.
	lua_getglobal(L, valuename.c_str());

	// Check if it's nil.
	bool isnil = lua_isnil(L, -1);

	// Removes it from the stack.
	lua_pop(L, 1);

	return isnil;
}

int luadataimpl::luawriter(lua_State *L, const void *chunk, std::size_t size, void *userChunkPtr) {
	luachunk *userChunk = static_cast<luachunk*>(userChunkPtr);
	const uint8_t *newData = static_cast<const uint8_t*>(chunk);
	userChunk->insert(userChunk->end(), newData, newData + size);
	return 0;
}

luavalueimpl::luavalueimpl(const std::string &name, luadataimpl *data) :
	_name(name),
	_data(data) {
}

double luavalueimpl::getdouble() const {
	return _data->retrieve<double>(_name);
}

int luavalueimpl::getint() const {
	return _data->retrieve<int>(_name);
}

std::string luavalueimpl::getstring() const {
	return _data->retrieve<std::string>(_name);
}

bool luavalueimpl::getbool() const {
	return _data->retrieve<bool>(_name);
}

bool luavalueimpl::isnil() const {
	return _data->isnil(_name);
}

luatype luavalueimpl::type() const {
	return nil;
}

}} // namespaces
