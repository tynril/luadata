#include <iostream>
#include "luadata.h"
#include "gtest/gtest.h"

namespace {;

class LuaDataSimpleValuesTest : public ::testing::Test
{
protected:
	virtual void SetUp() {
		ASSERT_TRUE(data_.loadfile("simple_values.lua"));
	}

	luadata::luadata data_;
};

// Test the various faces of the 'nil' value.
TEST_F(LuaDataSimpleValuesTest, TestNil)
{
	luadata::luavalue value = data_["value_nil"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_nil, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("nil", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it's not a table, array-access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's not an array, it should be empty.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

// Test the various faces of a boolean true value.
TEST_F(LuaDataSimpleValuesTest, TestBooleanTrue)
{
	luadata::luavalue value = data_["value_boolean_true"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_boolean, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(true, value.asbool());
	EXPECT_EQ(1.0, value.asdouble());
	EXPECT_EQ(1, value.asint());
	EXPECT_EQ("true", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it's not a table, array-access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's not an array, it should be empty.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

// Test the various faces of a boolean false value.
TEST_F(LuaDataSimpleValuesTest, TestBooleanFalse)
{
	luadata::luavalue value = data_["value_boolean_false"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_boolean, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("false", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it's not a table, array-access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's not an array, it should be empty.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

// Test the various faces of a zero number value.
TEST_F(LuaDataSimpleValuesTest, TestNumberZero)
{
	luadata::luavalue value = data_["value_number_zero"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_number, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ(std::to_string(0.0), value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it's not a table, array-access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's not an array, it should be empty.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

// Test the various faces of a non-zero number value.
TEST_F(LuaDataSimpleValuesTest, TestNumberNonZero)
{
	luadata::luavalue value = data_["value_number_non_zero"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_number, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(true, value.asbool());
	EXPECT_EQ(3.14159, value.asdouble());
	EXPECT_EQ(3, value.asint());
	EXPECT_EQ(std::to_string(3.14159), value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it's not a table, array-access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's not an array, it should be empty.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

// Test the various faces of an empty string value.
TEST_F(LuaDataSimpleValuesTest, TestStringEmpty)
{
	luadata::luavalue value = data_["value_string_empty"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_string, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it's not a table, array-access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's not an array, it should be empty.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

// Test the various faces of a non-empty string value.
TEST_F(LuaDataSimpleValuesTest, TestStringNonEmpty)
{
	luadata::luavalue value = data_["value_string_non_empty"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_string, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(true, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("this is a string", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it's not a table, array-access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's not an array, it should be empty.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

// Test the various faces of a zero integer value.
TEST_F(LuaDataSimpleValuesTest, TestIntegerZero)
{
	luadata::luavalue value = data_["value_integer_zero"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_number, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ(std::to_string(0.0), value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it's not a table, array-access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's not an array, it should be empty.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

// Test the various faces of a zero integer value.
TEST_F(LuaDataSimpleValuesTest, TestIntegerNonZero)
{
	luadata::luavalue value = data_["value_integer_non_zero"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_number, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(true, value.asbool());
	EXPECT_EQ(42.0, value.asdouble());
	EXPECT_EQ(42, value.asint());
	EXPECT_EQ(std::to_string(42.0), value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it's not a table, array-access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's not an array, it should be empty.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

} // namespace
