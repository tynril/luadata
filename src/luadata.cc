#define LUADATA_LIB

#include "luadata.h"
#include "luadataimpl.h"

namespace luadata {;

luavalue::luavalue(impl::luavalueimpl *impl) :
	_pimpl(impl),
	_refCount(new unsigned int(1)) {
}

luavalue::luavalue(const luavalue &other) :
	_pimpl(other._pimpl),
	_refCount(other._refCount) {
	*_refCount ++;
}

luavalue::~luavalue() {
	if(*_refCount == 1) {
		delete _pimpl;
		delete _refCount;
	}
	else {
		*_refCount --;
	}
}

luavalue& luavalue::operator=(const luavalue& rhs) {
	// Fallback for self-assignation.
	if(_pimpl == rhs._pimpl) return *this;

	// Drop our current implementation.
	if(*_refCount == 1) {
		delete _pimpl;
		delete _refCount;
	}
	else {
		*_refCount --;
	}

	// Assign the new one.
	_pimpl = rhs._pimpl;
	_refCount = rhs._refCount;
	*_refCount ++;

	return *this;
}

inline luavalue::operator double() const {
	return _pimpl->getdouble();
}

inline luavalue::operator int() const {
	return _pimpl->getint();
}

inline luavalue::operator std::string() const {
	return _pimpl->getstring();
}

inline luavalue::operator bool() const {
	return _pimpl->getbool();
}

inline double luavalue::asdouble() const {
	return _pimpl->getdouble();
}

inline int luavalue::asint() const {
	return _pimpl->getint();
}

inline std::string luavalue::asstring() const {
	return _pimpl->getstring();
}

inline bool luavalue::asbool() const {
	return _pimpl->getbool();
}

inline bool luavalue::isnil() const {
	return _pimpl->isnil();
}

inline luatype luavalue::type() const {
	return _pimpl->type();
}

void swap(luavalue& lhs, luavalue& rhs) {
	std::swap(lhs._pimpl, rhs._pimpl);
	std::swap(lhs._refCount, rhs._refCount);
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

inline luavalue luadata::operator[](const std::string &name) const {
	return _pimpl->get(name);
}

} // namespace luadata