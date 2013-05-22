#define LUADATA_LIB

#include "luadata.h"
#include "luadataimpl.h"

namespace luadata {;

luavalue::luavalue(impl::luavalueimpl *impl) :
	_pimpl(impl) {
}

luavalue::~luavalue() {
	delete _pimpl;
}

luavalue::operator double() const {
	return _pimpl->getdouble();
}

luavalue::operator int() const {
	return _pimpl->getint();
}

luavalue::operator std::string() const {
	return _pimpl->getstring();
}

luavalue::operator bool() const {
	return _pimpl->getbool();
}

/*luavalue& luavalue::operator[](const std::string& valuename) const {
	return _pimpl->gettable(valuename);
}*/

luatype luavalue::type() const {
	return _pimpl->type();
}

luadata::luadata() :
	_pimpl(new impl::luadataimpl()) {
}

luadata::~luadata() {
	delete _pimpl;
}

bool luadata::loadfile(const std::string& name, const std::string& path, loadfilemode mode) {
	return _pimpl->loadfile(name, path, mode);
}

bool luadata::loadfile(const std::string& path, loadfilemode mode) {
	return _pimpl->loadfile(path, mode);
}

luavalue luadata::operator[](const std::string& valuename) const {
	return _pimpl->get(valuename);
}

} // namespace luadata