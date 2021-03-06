#define LUADATA_LIB

#include "luadataimpl.h"

namespace luadata { namespace impl {;

luadataimpl::luadataimpl() :
	L(luaL_newstate()),
	_cacheStateIndex(0)
{
	// Load Lua libraries.
	luaL_openlibs(L);

	// Initialize the list of global values.
	_globalKeys = tablekeys();
}

luadataimpl::~luadataimpl() {
	lua_close(L);
}

bool luadataimpl::loadfile(const std::string &path) {
	// Load the file in the state.
	if(luaL_loadfile(L, path.c_str())) {
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return false;
	}

	// Add to the loaded files list, if needed.
	if(std::find_if(_loadedFiles.begin(), _loadedFiles.end(),
		[&path](const std::pair<std::string, time_t> &other) { return other.first == path; }) == _loadedFiles.end()) {
		_loadedFiles.push_back(std::pair<std::string, time_t>(path, getmodtime(path)));
	}

	return processloadedchunk();
}

bool luadataimpl::loadcode(const std::string &code) {
	if(luaL_loadstring(L, code.c_str())) {
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return false;
	}

	return processloadedchunk();
}

void luadataimpl::hotreload(bool force) {
	for(auto &file : _loadedFiles) {
		// Check if the file was modified.
		time_t lastmod = getmodtime(file.first);
		if(force || lastmod != file.second) {
			// The file has been modified, let's reload it.
			loadfile(file.first);
			file.second = lastmod;
		}
	}
}

bool luadataimpl::processloadedchunk() {
	// There should be the chunk at the top of the stack now.
	if(lua_gettop(L) != 1 || !lua_isfunction(L, 1))
		return false;

	// Runs the script directly.
	if(lua_pcall(L, 0, 0, 0))
		return false;

	// Invalidates the cache.
	_cacheStateIndex++;

	return true;
}

inline void luadataimpl::getpath(const luapath &valuepath) {
	// Gets the first element of the path to the front.
	lua_getglobal(L, valuepath.path[0].key.name.c_str());

	// While there's a function on the top, call it.
	while(lua_isfunction(L, -1)) {
		callfunction(valuepath.path[0].args);
	}

	// If we still have path elements, dig in.
	for(unsigned int i = 1; i < valuepath.path.size(); ++ i) {
		// If the next value is a table, let's put the right key to the top of the stack.
		if(lua_istable(L, -1)) {
			// Extracts the value.
			if (valuepath.path[i].key.type == luakey::p_name)
				lua_pushstring(L, valuepath.path[i].key.name.c_str());
			else
				lua_pushinteger(L, valuepath.path[i].key.index);
			lua_gettable(L, -2);

			// If the value is a function, run it.
			while(lua_isfunction(L, -1)) {
				callfunction(valuepath.path[i].args);
			}
		}
		else {
			// Otherwise, there's a problem with this path.
			lua_pushnil(L);
		}
	}
}

inline void luadataimpl::callfunction(const std::vector<luaarg> &args) {
	for(luaarg arg : args) {
		switch(arg.type) {
		case luaarg::a_boolean:
			lua_pushboolean(L, arg.simple_content.b);
			break;
		case luaarg::a_double:
			lua_pushnumber(L, arg.simple_content.d);
			break;
		case luaarg::a_integer:
			lua_pushinteger(L, arg.simple_content.n);
			break;
		case luaarg::a_string:
			lua_pushstring(L, arg.string_content.c_str());
			break;
		}
	}

	lua_pcall(L, static_cast<int>(args.size()), 1, 0);
}

double luadataimpl::getdoublefromstack(const double &defaultValue) {
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
		return 0.0;
	case LUA_TNIL:
		return defaultValue;
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:
	case LUA_TTHREAD:
	default:
		return 0.0;
	}
}

std::ptrdiff_t luadataimpl::getintfromstack(const std::ptrdiff_t &defaultValue) {
	// Get the value type.
	int type = lua_type(L, -1);

	// Loads the value with data depending on the type.
	switch(type) {
	case LUA_TBOOLEAN:
		return lua_toboolean(L, -1) != 0 ? 1 : 0;
	case LUA_TNUMBER:
		return (std::ptrdiff_t)lua_tonumber(L, -1);
	case LUA_TSTRING:
		return 0;
	case LUA_TTABLE:
		return 0;
	case LUA_TNIL:
		return defaultValue;
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:
	case LUA_TTHREAD:
	default:
		return 0;
	}
}

std::string luadataimpl::getstringfromstack(const std::string &defaultValue) {
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
	case LUA_TNIL:
		return defaultValue;
	case LUA_TTABLE:
		return "table";
	case LUA_TLIGHTUSERDATA:
		return "lightuserdata";
	case LUA_TUSERDATA:
		return "userdata";
	case LUA_TTHREAD:
		return "thread";
	default:
		return 0;
	}
}

bool luadataimpl::getboolfromstack(const bool &defaultValue) {
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
		return false;
	case LUA_TNIL:
		return defaultValue;
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:
	case LUA_TTHREAD:
	default:
		return false;
	}
}

double luadataimpl::retrievedouble(const luapath &valuepath, const double &defaultValue) {
	getpath(valuepath);
	double value = getdoublefromstack(defaultValue);
	lua_pop(L, (int)valuepath.path.size());
	return value;
}

double luadataimpl::retrievedouble(const luapath &valuepath) {
	return retrievedouble(valuepath, 0.0);
}

std::ptrdiff_t luadataimpl::retrieveint(const luapath &valuepath, const std::ptrdiff_t &defaultValue) {
	getpath(valuepath);
	std::ptrdiff_t value = getintfromstack(defaultValue);
	lua_pop(L, (int)valuepath.path.size());
	return value;
}

std::ptrdiff_t luadataimpl::retrieveint(const luapath &valuepath) {
	return retrieveint(valuepath, 0);
}

std::string luadataimpl::retrievestring(const luapath &valuepath, const std::string &defaultValue) {
	getpath(valuepath);
	std::string value = getstringfromstack(defaultValue);
	lua_pop(L, (int)valuepath.path.size());
	return value;
}

std::string luadataimpl::retrievestring(const luapath &valuepath) {
	return retrievestring(valuepath, "nil");
}

bool luadataimpl::retrievebool(const luapath &valuepath, const bool &defaultValue) {
	getpath(valuepath);
	bool value = getboolfromstack(defaultValue);
	lua_pop(L, (int)valuepath.path.size());
	return value;
}

bool luadataimpl::retrievebool(const luapath &valuepath) {
	return retrievebool(valuepath, false);
}

luatype luadataimpl::type(const luapath &valuepath) {
	// Gets the value at the front of the stack.
	getpath(valuepath);

	// Gets its type.
	int luaType = lua_type(L, -1);
	luatype type = luatype::lua_nil;
	switch(luaType) {
	case LUA_TBOOLEAN:	type = luatype::lua_boolean;	break;
	case LUA_TNUMBER:	type = luatype::lua_number;		break;
	case LUA_TSTRING:	type = luatype::lua_string;		break;
	case LUA_TTABLE:	type = luatype::lua_table;		break;
	case LUA_TFUNCTION:	type = luatype::lua_function;	break;
	case LUA_TNIL:		type = luatype::lua_nil;		break;
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:	type = luatype::lua_userdata;	break;
	case LUA_TTHREAD:	type = luatype::lua_thread;		break;
	}

	// Clears the stack.
	lua_pop(L, (int)valuepath.path.size());

	// And returns the value.
	return type;
}

std::ptrdiff_t luadataimpl::tablelen(const luapath &valuepath) {
	// Gets the value at the front of the stack.
	getpath(valuepath);

	// Gets the table length.
	std::ptrdiff_t len = 0;
	if(lua_istable(L, -1)) {
		len = luaL_len(L, -1);
	}

	// Clears the stack.
	lua_pop(L, (int)valuepath.path.size());

	// And returns the length.
	return len;
}

std::vector<luakey> luadataimpl::tablekeys() {
	// Gets the value at the front of the stack.
	lua_pushglobaltable(L);

	// Fetch the keys.
	std::vector<luakey> allKeys = fetchtablekeys();

	// Clears the stack.
	lua_pop(L, 1);

	// Sort the keys and get the difference from the base set.
	std::sort(allKeys.begin(), allKeys.end(), luakeycomparator());
	std::vector<luakey> keys(allKeys.size());
	std::vector<luakey>::iterator it = std::set_difference(allKeys.begin(), allKeys.end(), _globalKeys.begin(), _globalKeys.end(), keys.begin(), luakeycomparator());
	keys.resize(it - keys.begin());

	return keys;
}

std::vector<luakey> luadataimpl::tablekeys(const luapath &valuepath) {
	// Gets the value at the front of the stack.
	getpath(valuepath);

	// Fetch the keys.
	std::vector<luakey> keys = fetchtablekeys();

	// Clears the stack.
	lua_pop(L, (int)valuepath.path.size());

	return keys;
}

void luadataimpl::updatekeyscache(luapath &valuepath) {
	std::vector<luakey> keys = this->tablekeys(valuepath);
	valuepath.keys_cache = std::make_shared<std::vector<std::pair<luakey, luavalue>>>();

	std::transform(keys.begin(), keys.end(), std::back_inserter(*valuepath.keys_cache), [this, &valuepath](luakey &key){
		std::vector<luapathelement> path(valuepath.path.begin(), valuepath.path.end());
		path.push_back(key);
		return std::pair<luakey, luavalue>(key, luavalue(luapath(path), this));
	});

	valuepath.keys_cache_state = _cacheStateIndex;
}

std::vector<std::pair<luakey, luavalue>>::const_iterator luadataimpl::tablebegin(luapath &valuepath) {
	// Update the cache if necessary.
	if (valuepath.keys_cache == nullptr || valuepath.keys_cache_state != _cacheStateIndex) {
		updatekeyscache(valuepath);
	}

	return valuepath.keys_cache->begin();
}

std::vector<std::pair<luakey, luavalue>>::const_iterator luadataimpl::tableend(luapath &valuepath) {
	// Update the cache if necessary.
	if (valuepath.keys_cache == nullptr || valuepath.keys_cache_state != _cacheStateIndex) {
		updatekeyscache(valuepath);
	}

	return valuepath.keys_cache->end();
}

void luadataimpl::updateglobalkeyscache() {
	std::vector<luakey> keys = this->tablekeys();
	_globalKeysCache = std::make_shared<std::vector<std::pair<luakey, luavalue>>>();

	std::transform(keys.begin(), keys.end(), std::back_inserter(*_globalKeysCache), [this](luakey &key){
		return std::pair<luakey, luavalue>(key, luavalue(luapath(luapathelement(key)), this));
	});

	_globalKeysCacheState = _cacheStateIndex;
}

std::vector<std::pair<luakey, luavalue>>::const_iterator luadataimpl::tablebegin() {
	// Update the cache if necessary.
	if (_globalKeysCache == nullptr || _globalKeysCacheState != _cacheStateIndex) {
		updateglobalkeyscache();
	}

	return _globalKeysCache->begin();
}

std::vector<std::pair<luakey, luavalue>>::const_iterator luadataimpl::tableend() {
	// Update the cache if necessary.
	if (_globalKeysCache == nullptr || _globalKeysCacheState != _cacheStateIndex) {
		updateglobalkeyscache();
	}

	return _globalKeysCache->end();
}

std::vector<luakey> luadataimpl::fetchtablekeys() {
	// Gets the table keys.
	std::vector<luakey> keys;
	if(lua_istable(L, -1)) {
		lua_pushnil(L);
		while(lua_next(L, -2)) {
			if(lua_type(L, -2) == LUA_TSTRING)
				keys.push_back(luakey(lua_tostring(L, -2)));
			else {
				keys.push_back(luakey(static_cast<int>(lua_tointeger(L, -2))));
			}
			lua_pop(L, 1);
		}
	}

	// And returns the keys.
	return keys;
}

time_t getmodtime(const std::string &filepath) {
	struct stat st = {0};
	int ret = stat(filepath.c_str(), &st);
	if(ret == -1) return 0;
	return st.st_mtime;
}

}} // namespaces
