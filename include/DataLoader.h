#ifndef __luadata_dataloader_h__
#define __luadata_dataloader_h__

#include <string>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "Data.h"

class DataTable;

class DataLoader
{
private:
    static lua_State* L;

public:
	static bool initialize();
	static void dispose();

private:
    static bool loadFile(std::string, std::string);
    static void fillTable(DataTable*);
    static void disposeTable(DataTable*);

    static void stackDump(std::string);
};

#endif /* __mapgen_data_dataloader_h__ */