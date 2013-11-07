#define LUADATA_LIB

#include "luadata.h"
#include "luadataimpl.h"

namespace luadata {;

luavalue::luavalue(const impl::luapath &valuepath, impl::luadataimpl *impl) :
	_valuepath(valuepath),
	_pimpl(impl) {
}

luavalue::luavalue(const luavalue &other) :
	_valuepath(other._valuepath),
	_pimpl(other._pimpl) {
}

luavalue::luavalue(luavalue &&other) :
	_valuepath(std::move(other._valuepath)),
	_pimpl(std::move(other._pimpl)) {
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

double luavalue::asdouble(const double &defaultValue) const {
	return _pimpl->retrievedouble(_valuepath, defaultValue);
}

int luavalue::asint() const {
	return _pimpl->retrieveint(_valuepath);
}

int luavalue::asint(const int &defaultValue) const {
	return _pimpl->retrieveint(_valuepath, defaultValue);
}

std::string luavalue::asstring() const {
	return _pimpl->retrievestring(_valuepath);
}

std::string luavalue::asstring(const std::string &defaultValue) const {
	return _pimpl->retrievestring(_valuepath, defaultValue);
}

bool luavalue::asbool() const {
	return _pimpl->retrievebool(_valuepath);
}

bool luavalue::asbool(const bool &defaultValue) const {
	return _pimpl->retrievebool(_valuepath, defaultValue);
}

std::ptrdiff_t luavalue::tablelen() const {
	return _pimpl->tablelen(_valuepath);
}

std::vector<luakey> luavalue::tablekeys() const {
	return _pimpl->tablekeys(_valuepath);
}

luavalue luavalue::operator[](const luakey &key) const {
	impl::luapath appendedPath = _valuepath;
	appendedPath.push_back(impl::luapathelement(key));
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
	impl::luapath argsedPath = _valuepath;
	argsedPath[argsedPath.size() - 1].args = args;
	return luavalue(argsedPath, _pimpl);
}

luatype luavalue::type() const {
	return _pimpl->type(_valuepath);
}

std::ostream& operator<<(std::ostream& os, const luakey& key) {
	return os << key.str();
}

std::ostream& operator<<(std::ostream& os, const luavalue& val) {
	if(val.type() == luatype::lua_table) {
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

luadata::luadata(luadata &&other) :
	_pimpl(std::move(other._pimpl)) {
}

luadata::~luadata() {
	delete _pimpl;
}

bool luadata::loadfile(const std::string &path) {
	return _pimpl->loadfile(path);
}

bool luadata::loadcode(const std::string &code) {
	return _pimpl->loadcode(code);
}

void luadata::hotreload() {
	_pimpl->hotreload();
}

std::vector<luakey> luadata::keys() const {
	return _pimpl->tablekeys();
}

luavalue luadata::operator[](const luakey &key) const {
	return luavalue(impl::luapath(1, impl::luapathelement(key)), _pimpl);
}

} // namespace luadata
