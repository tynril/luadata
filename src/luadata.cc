#define LUADATA_LIB

#include "luadata.h"
#include "luadataimpl.h"

namespace luadata {;

luavalue::operator float() const {
	return 666.6f;
}

luavalue::operator double() const {
	return 777.777;
}

luavalue::operator int() const {
	return 888;
}

luavalue::operator std::string() const {
	return "pouet";
}

luavalue::operator bool() const {
	return false;
}

luavalue& luavalue::operator[](const std::string& valuename) const {
	return __val__;
}

luatype luavalue::type() const {
	return luatype::nil;
}

luadata::luadata(const std::string &file) {
	_pimpl->pouet();
}

luavalue& luadata::operator[](const std::string& valuename) const {
	return __val__;
}

} // namespace luadata