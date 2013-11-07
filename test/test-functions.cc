#include <iostream>
#include "luadata.h"
#include "gtest/gtest.h"

namespace {;

class LuaDataFunctionsTest : public ::testing::Test
{
protected:
	virtual void SetUp() {
		ASSERT_TRUE(data_.loadfile("functions_values.lua"));
	}

	luadata::luadata data_;
};

// Test a simple function.
TEST_F(LuaDataFunctionsTest, TestEmpty)
{
	luadata::luavalue value = data_["function_simple"];

	// As a function transparently executes when accessing its value,
	// everything relates to the inner type.
	EXPECT_EQ(luadata::luatype::lua_number, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(true, value.asbool());
	EXPECT_EQ(42.0, value.asdouble());
	EXPECT_EQ(42, value.asint());
	EXPECT_EQ(std::to_string(42.0), value.asstring());

	// An explicit call to the function do the same as transparent access.
	EXPECT_EQ(luadata::luatype::lua_number, value().type());
	EXPECT_EQ(42, value().asint());

	// Calling the function with unused arguments should be OK as well.
	EXPECT_EQ(42, value("foo").asint());

	// It's not a table, it should not have a length.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());

	// Array access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
}

// Test a nested function.
TEST_F(LuaDataFunctionsTest, TestNested)
{
	luadata::luavalue value = data_["function_nested"];

	// Even for nested functions, the call is transparent.
	EXPECT_EQ(luadata::luatype::lua_number, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(true, value.asbool());
	EXPECT_EQ(43.0, value.asdouble());
	EXPECT_EQ(43, value.asint());
	EXPECT_EQ(std::to_string(43.0), value.asstring());

	// An explicit call to the function do the same as transparent access.
	EXPECT_EQ(luadata::luatype::lua_number, value().type());
	EXPECT_EQ(43, value().asint());

	// Calling the function with unused arguments should be OK as well.
	EXPECT_EQ(43, value("foo").asint());

	// It's not a table, it should not have a length.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());

	// Array access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
}

// Test a deeply nested function.
TEST_F(LuaDataFunctionsTest, TestNestedDeeper)
{
	luadata::luavalue value = data_["function_nested_deeper"];

	// Even for nested functions, the call is transparent.
	EXPECT_EQ(luadata::luatype::lua_number, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(true, value.asbool());
	EXPECT_EQ(44.0, value.asdouble());
	EXPECT_EQ(44, value.asint());
	EXPECT_EQ(std::to_string(44.0), value.asstring());

	// An explicit call to the function do the same as transparent access.
	EXPECT_EQ(luadata::luatype::lua_number, value().type());
	EXPECT_EQ(44, value().asint());

	// Calling the function with unused arguments should be OK as well.
	EXPECT_EQ(44, value("foo").asint());

	// It's not a table, it should not have a length.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());

	// Array access should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
}

// Test a function that returns a table.
TEST_F(LuaDataFunctionsTest, TestReturnsTable)
{
	luadata::luavalue value = data_["function_returns_table"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_table, value.type());

	// Checking if cast returns expected results.
	EXPECT_FALSE(value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("table", value.asstring());

	// The table should contain 3 elements.
	EXPECT_EQ(3, value.tablelen());
	EXPECT_EQ(3, value.tablekeys().size());

	// Tables in Lua starts with index 1.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_string, value[1].type());
	EXPECT_EQ(luadata::luatype::lua_string, value[2].type());
	EXPECT_EQ(luadata::luatype::lua_string, value[3].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value[4].type());

	// Testing access to undefined keys.
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// Testing the values themselves.
	EXPECT_EQ("A", value[1].asstring());
	EXPECT_EQ("B", value[2].asstring());
	EXPECT_EQ("C", value[3].asstring());
}

// Test a table containing multiple functions.
TEST_F(LuaDataFunctionsTest, TestFunctionsInTable)
{
	luadata::luavalue value = data_["function_in_table"];

	// It's a regular table, a function is a value like any other.
	EXPECT_EQ(luadata::luatype::lua_table, value.type());

	// Checking if cast returns expected results.
	EXPECT_FALSE(value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("table", value.asstring());

	// It's an associative table, so it should have keys.
	EXPECT_EQ(2, value.tablekeys().size());

	// And accessing its content should transparently call the functions.
	EXPECT_EQ("pim", value["pom"].asstring());
	EXPECT_EQ("poum", value["pam"].asstring());
}

// Test a function which is called with arguments.
TEST_F(LuaDataFunctionsTest, TestFunctionWithArgs)
{
	luadata::luavalue value = data_["function_with_arg"];

	// Accessing the value directly calls the function with each
	// argument set to 'nil', which fails and returns a string
	// containing the Lua error.
	EXPECT_EQ(luadata::luatype::lua_string, value.type());
	EXPECT_EQ("functions_values.lua:31: attempt to perform arithmetic on local 'x' (a nil value)", value.asstring());

	// Calling the function with arguments should work.
	EXPECT_EQ(luadata::luatype::lua_number, value(2, 4).type());
	EXPECT_EQ(20, value(2, 4).asint());
}

// Test a function which updates a global.
TEST_F(LuaDataFunctionsTest, TestFunctionWithGlobal)
{
	luadata::luavalue value = data_["function_with_global"];

	// The global should be updated at every call.
	EXPECT_EQ(1, value.asint());
	EXPECT_EQ(2, value.asint());
	EXPECT_EQ(3, value.asint());
}

// Test a deeply nested table with functions.
TEST_F(LuaDataFunctionsTest, TestFunctionWithinDeepTable)
{
	luadata::luavalue value = data_["function_within_deep_table"];

	// Check all values.
	EXPECT_EQ(1, value["one"]["a"].asint());
	EXPECT_EQ(2, value["one"]["b"].asint());
	EXPECT_EQ(3, value["one"]["c"].asint());
	EXPECT_EQ(42, value["two"]["foo"]["fu"].asint());
	EXPECT_EQ("toto", value["two"]["foo"]["fo"].asstring());
	EXPECT_EQ(55, value["two"]["bar"].asint());
}

} // namespace
