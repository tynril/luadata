#ifndef _LUADATAIMPL_H_
#define _LUADATAIMPL_H_

#include <iostream>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace luadata {

class luadataimpl {
public:
	void pouet() {
		std::cout << "pouet" << std::endl;
	}
};

} // namespaces

#endif // _LUADATAIMPL_H_