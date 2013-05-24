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

std::vector<luakey> luavalue::tablekeys() const {
	return _pimpl->tablekeys(_valuepath);
}

luavalue luavalue::operator[](const luakey &key) const {
	luapath appendedPath = _valuepath;
	appendedPath.push_back(luapathelement(key));
	return luavalue(appendedPath, _pimpl);
}

luavalue luavalue::operator()() const {
	return *this;
}

luavalue luavalue::operator()(luaarg arg0) const {
	return operator()(std::vector<luaarg>(1, arg0));
}

luavalue luavalue::operator()(luaarg arg0, luaarg arg1) const {
	std::vector<luaarg> args;
	args.push_back(arg0);
	args.push_back(arg1);
	return operator()(args);
}

luavalue luavalue::operator()(luaarg arg0, luaarg arg1, luaarg arg2) const {
	std::vector<luaarg> args;
	args.push_back(arg0);
	args.push_back(arg1);
	args.push_back(arg2);
	return operator()(args);
}

luavalue luavalue::operator()(luaarg arg0, luaarg arg1, luaarg arg2, luaarg arg3) const {
	std::vector<luaarg> args;
	args.push_back(arg0);
	args.push_back(arg1);
	args.push_back(arg2);
	args.push_back(arg3);
	return operator()(args);
}

luavalue luavalue::operator()(luaarg arg0, luaarg arg1, luaarg arg2, luaarg arg3, luaarg arg4) const {
	std::vector<luaarg> args;
	args.push_back(arg0);
	args.push_back(arg1);
	args.push_back(arg2);
	args.push_back(arg3);
	args.push_back(arg4);
	return operator()(args);
}

luavalue luavalue::operator()(const std::vector<luaarg> &args) const {
	luapath argsedPath = _valuepath;
	argsedPath[argsedPath.size() - 1].args = args;
	return luavalue(argsedPath, _pimpl);
}

luatype luavalue::type() const {
	return _pimpl->type(_valuepath);
}

std::ostream& operator<<(std::ostream& os, const luakey& key) {
	if(key.type == luakey::p_name)
		os << key.name;
	else
		os << key.index;
	return os;
}

std::ostream& operator<<(std::ostream& os, const luavalue& val) {
	if(val.type() == lua_table) {
		os << "{";
		bool isFirst = true;
		for(luakey key : val.tablekeys()) {
			if(isFirst) isFirst = false;
			else os << ", ";
			os << key << "=" << val[key];
		}
		os << "}";
	}
	else
		os << val.asstring();
	return os;
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

bool luadata::loadcode(const std::string &code) {
	return _pimpl->loadcode(code);
}

bool luadata::savefile(const std::string &path) {
	return _pimpl->savefile(path);
}

luavalue luadata::operator[](const std::string &name) const {
	return luavalue(luapath(1, luapathelement(name)), _pimpl);
}

} // namespace luadata