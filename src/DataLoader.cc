#include "DataLoader.h"

lua_State* DataLoader::L = 0;

bool DataLoader::initialize()
{
    // Prevents multiple call to initialize.
    if(L != NULL) throw;

    // Creates a state for LUA execution.
	L = luaL_newstate();
    luaL_openlibs(L);

    // Preparing the root table for data.
    Data::_data = new DataTable("_root_");

    // Process a single file.
    return loadFile("assets/data/world/biomes.lua", "biomes");
}

bool DataLoader::loadFile(std::string filePath, std::string globalName)
{
    // Loads the LUA file.
    int loadStatus = luaL_loadfile(L, filePath.c_str());
    if(loadStatus) {
        printf("Couldn't load file: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }

    // Executes this file.
    int callStatus = lua_pcall(L, 0, 0, 0);
    if(callStatus) {
        printf("Couldn't call file: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }

    // Gets the global that it should contain.
    lua_getglobal(L, globalName.c_str());

    // Ensure it's a table.
    if(!lua_istable(L, -1)) {
        printf("Unable to find global %s in loaded file.\n", globalName.c_str());
        lua_pop(L, 1);
        return false;
    }

    // Gets a data table from this table.
    DataTable* fileTable = new DataTable(globalName);
    fillTable(fileTable);
    lua_pop(L, 1);

    // Add it to the global data manager.
    Data::_data->add(fileTable);

    return true;
}

void DataLoader::fillTable(DataTable* table)
{
    // This method expects the table to be on the top of the LUA stack.
    lua_pushnil(L);

    // Iterate on this table.
    while(lua_next(L, -2)) {
        // Fetching the key.
        int key_type = lua_type(L, -2);
        if(key_type != LUA_TSTRING) {
            printf("Key of a non-string type (%s).\n", lua_typename(L, key_type));
            lua_pop(L, 1);
            continue;
        }
        std::string key_name = lua_tostring(L, -2);

        // Skipping unsupported value types
        int val_type = lua_type(L, -1);
        if(val_type != LUA_TBOOLEAN &&
           val_type != LUA_TNUMBER &&
           val_type != LUA_TSTRING &&
           val_type != LUA_TTABLE) {
            // Unsupported value type.
            printf("Unsupported value type (%s).\n", lua_typename(L, val_type));
            lua_pop(L, 1);
            continue;
        }

        switch(val_type) {
            case LUA_TBOOLEAN: {
                bool value = lua_toboolean(L, -1) != 0;
                table->add(new DataValue<bool_d>(key_name, value));
                break;
            }

            case LUA_TNUMBER: {
                double value = lua_tonumber(L, -1);
				table->add(new DataValue<DataNumber>(key_name, DataNumber(value)));
                break;
            }

            case LUA_TSTRING: {
                std::string value = lua_tostring(L, -1);
                table->add(new DataValue<string_d>(key_name, value));
                break;
            }

            case LUA_TTABLE: {
                DataTable* subTable = new DataTable(key_name);
                fillTable(subTable);
                table->add(subTable);
                break;
            }

            default:
                throw;
                break;
        }
        lua_pop(L, 1);
    }
}

void DataLoader::dispose()
{
    // Clear the data in memory.
    disposeTable(Data::_data);

    // Closes the LUA state.
    if(L != NULL) lua_close(L);
    L = NULL;
}

void DataLoader::disposeTable(DataTable* table)
{
    // Dispose of sub-tables.
    std::map<const std::string, const DataNode*>::iterator it;
    for(it = table->m_content.begin(); it != table->m_content.end(); ++ it) {
        const DataTable* subTable = dynamic_cast<const DataTable*>(it->second);
        if(subTable != NULL) {
            disposeTable(const_cast<DataTable*>(subTable));
        }
    }

    // Deletes this table.
    delete table;
}

void DataLoader::stackDump(std::string txt) {
    printf("%s > ", txt.c_str());
      int i;
      int top = lua_gettop(L);
      for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        printf("[%d]", i);
        switch (t) {
    
          case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;
    
          case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
    
          case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;
    
          default:  /* other values */
            printf("%s", lua_typename(L, t));
            break;
    
        }
        printf("  ");  /* put a separator */
      }
      printf("\n");  /* end the listing */
    }