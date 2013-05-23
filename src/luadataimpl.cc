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
	return luavalue(new luavalueimpl(luapath(1, luapathelement(valuename)), this));
}

template<typename T>
inline T luadataimpl::retrieve(const luapath &valuepath) {
	// Gets the value at the front of the stack.
	getpath(valuepath);

	// Read it.
	T value = getfromstack<T>();

	// Clears the stack.
	clearstack();

	// And returns the value.
	return value;
}

inline void luadataimpl::getpath(const luapath &valuepath) {
	// Gets the first element of the path to the front.
	lua_getglobal(L, valuepath[0].key.c_str());

	// If we still have path elements, and if the value is a table, dig in.
	for(unsigned int i = 1; i < valuepath.size() && lua_istable(L, -1); ++ i) {
		if(valuepath[i].type == key_t)
			lua_pushstring(L, valuepath[i].key.c_str());
		else
			lua_pushinteger(L, valuepath[i].index + 1);
		lua_gettable(L, -2);
	}
}

inline void luadataimpl::clearstack() {
	lua_pop(L, lua_gettop(L));
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
	case LUA_TFUNCTION:
		lua_pcall(L, 0, 1, 0);
		return getfromstack<double>();
	case LUA_TTABLE:
		return luaL_len(L, -1);
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
	case LUA_TFUNCTION:
		lua_pcall(L, 0, 1, 0);
		return getfromstack<int>();
	case LUA_TTABLE:
		return luaL_len(L, -1);
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
	case LUA_TFUNCTION:
		lua_pcall(L, 0, 1, 0);
		return getfromstack<std::string>();
	case LUA_TNIL:
		return "nil";
	case LUA_TTABLE:
		return "table";
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
		return luaL_len(L, -1) != 0;
	case LUA_TFUNCTION:
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

inline luatype luadataimpl::type(const luapath &valuepath) {
	// Gets the value at the front of the stack.
	getpath(valuepath);

	// Gets its type.
	int luaType = lua_type(L, 1);
	luatype type = nil;
	switch(luaType) {
	case LUA_TBOOLEAN:	type = boolean;		break;
	case LUA_TNUMBER:	type = number;		break;
	case LUA_TSTRING:	type = string;		break;
	case LUA_TTABLE:	type = table;		break;
	case LUA_TFUNCTION:	type = function;	break;
	case LUA_TNIL:		type = nil;			break;
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:	type = userdata;	break;
	case LUA_TTHREAD:	type = thread;		break;
	}

	// Clears the stack.
	clearstack();

	// And returns the value.
	return type;
}

inline std::size_t luadataimpl::tablelen(const luapath &valuepath) {
	// Gets the value at the front of the stack.
	getpath(valuepath);

	// Gets the table length.
	std::size_t len = 0;
	if(lua_istable(L, -1)) {
		len = luaL_len(L, -1);
	}

	// Clears the stack.
	clearstack();

	// And returns the length.
	return len;
}

inline std::vector<std::string> luadataimpl::tablekeys(const luapath &valuepath) {
	// Gets the value at the front of the stack.
	getpath(valuepath);

	// Gets the table keys.
	std::vector<std::string> keys;
	if(lua_istable(L, -1)) {
		lua_pushnil(L);
		while(lua_next(L, -2)) {
			keys.push_back(lua_tostring(L, -2));
			lua_pop(L, 1);
		}
	}

	// Clears the stack.
	clearstack();

	// And returns the keys.
	return keys;
}

int luadataimpl::luawriter(lua_State *L, const void *chunk, std::size_t size, void *userChunkPtr) {
	luachunk *userChunk = static_cast<luachunk*>(userChunkPtr);
	const uint8_t *newData = static_cast<const uint8_t*>(chunk);
	userChunk->insert(userChunk->end(), newData, newData + size);
	return 0;
}

luavalueimpl::luavalueimpl(const luapath &valuepath, luadataimpl *data) :
	_valuepath(valuepath),
	_data(data) {
}

double luavalueimpl::getdouble() const {
	return _data->retrieve<double>(_valuepath);
}

int luavalueimpl::getint() const {
	return _data->retrieve<int>(_valuepath);
}

std::string luavalueimpl::getstring() const {
	return _data->retrieve<std::string>(_valuepath);
}

bool luavalueimpl::getbool() const {
	return _data->retrieve<bool>(_valuepath);
}

std::size_t luavalueimpl::tablelen() const {
	return _data->tablelen(_valuepath);
}

std::vector<std::string> luavalueimpl::tablekeys() const {
	return _data->tablekeys(_valuepath);
}

luavalue luavalueimpl::tableval(const std::string &keyval) const {
	luapath appendedPath(_valuepath);
	appendedPath.push_back(luapathelement(keyval));
	return luavalue(new luavalueimpl(appendedPath, _data));
}

luavalue luavalueimpl::tableval(const int &indexval) const {
	luapath appendedPath(_valuepath);
	appendedPath.push_back(luapathelement(indexval));
	return luavalue(new luavalueimpl(appendedPath, _data));
}

luatype luavalueimpl::type() const {
	return _data->type(_valuepath);
}

}} // namespaces
