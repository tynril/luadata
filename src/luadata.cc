#define LUADATA_LIB

#include "luadata.h"
#include "luadataimpl.h"

namespace luadata {;

luavalue::luavalue(const luapath &valuepath, impl::luadataimpl *impl) :
	_valuepath(valuepath),
	_pimpl(impl) {
}

luavalue::luavalue(const luavalue &other) :
	_valuepath(other._valuepath),
	_pimpl(other._pimpl) {
}

luavalue::~luavalue() {
}

luavalue& luavalue::operator=(const luavalue& rhs) {
	// Fallback for self-assignation.
	if(_pimpl == rhs._pimpl) return *this;

	_pimpl = rhs._pimpl;
	_valuepath = rhs._valuepath;

	return *this;
}

luavalue::operator double() const {
	return _pimpl->retrievedouble(_valuepath);
}

luavalue::operator int() const {
	return _pimpl->retrieveint(_valuepath);
}

luavalue::operator std::string() const {
	return _pimpl->retrievestring(_valuepath);
}

luavalue::operator bool() const {
	return _pimpl->retrievebool(_valuepath);
}

double luavalue::asdouble() const {
	return _pimpl->retrievedouble(_valuepath);
}

int luavalue::asint() const {
	return _pimpl->retrieveint(_valuepath);
}

std::string luavalue::asstring() const {
	return _pimpl->retrievestring(_valuepath);
}

bool luavalue::asbool() const {
	return _pimpl->retrievebool(_valuepath);
}

std::size_t luavalue::tablelen() const {
	return _pimpl->tablelen(_valuepath);
}

std::vector<std::string> luavalue::tablekeys() const {
	return _pimpl->tablekeys(_valuepath);
}

luavalue luavalue::operator[](const std::string &keyname) const {
	luapath appendedPath(_valuepath);
	appendedPath.push_back(luapathelement(keyname));
	return luavalue(appendedPath, _pimpl);
}

luavalue luavalue::operator[](const int &keyindex) const {
	luapath appendedPath(_valuepath);
	appendedPath.push_back(luapathelement(keyindex));
	return luavalue(appendedPath, _pimpl);
}

luatype luavalue::type() const {
	return _pimpl->type(_valuepath);
}

void swap(luavalue& lhs, luavalue& rhs) {
	std::swap(lhs._pimpl, rhs._pimpl);
	std::swap(lhs._valuepath, rhs._valuepath);
}

luadata::luadata() :
	_pimpl(new impl::luadataimpl()) {
}

luadata::~luadata() {
	delete _pimpl;
}

bool luadata::loadfile(const std::string &path, loadfilemode mode) {
	return _pimpl->loadfile(path, mode);
}

bool luadata::savefile(const std::string &path) {
	return _pimpl->savefile(path);
}

luavalue luadata::operator[](const std::string &name) const {
	return _pimpl->get(name);
}

} // namespace luadata