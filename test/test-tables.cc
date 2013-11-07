#include <iostream>
#include "luadata.h"
#include "gtest/gtest.h"

namespace {;

class LuaDataTablesTest : public ::testing::Test
{
protected:
	virtual void SetUp() {
		ASSERT_TRUE(data_.loadfile("table_values.lua"));
	}

	luadata::luadata data_;
};

// Test an empty table.
TEST_F(LuaDataTablesTest, TestEmpty)
{
	luadata::luavalue value = data_["table_empty"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_table, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("table", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// The table is empty, hence accessing its elements should return nil.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// And since it's empty, its length should be zero.
	EXPECT_EQ(0, value.tablelen());
	EXPECT_EQ(0, value.tablekeys().size());
}

// Test a simple, non-associative, table.
TEST_F(LuaDataTablesTest, TestSimple)
{
	luadata::luavalue value = data_["table_simple"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_table, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("table", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// The table should contain 3 elements.
	EXPECT_EQ(3, value.tablelen());
	EXPECT_EQ(3, value.tablekeys().size());

	// Tables in Lua starts with index 1.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_number, value[1].type());
	EXPECT_EQ(luadata::luatype::lua_number, value[2].type());
	EXPECT_EQ(luadata::luatype::lua_number, value[3].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value[4].type());

	// Testing access to undefined keys.
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// Testing the values themselves.
	EXPECT_EQ(1, value[1].asint());
	EXPECT_EQ(2, value[2].asint());
	EXPECT_EQ(3, value[3].asint());
}

// Test an associative table
TEST_F(LuaDataTablesTest, TestAssoc)
{
	luadata::luavalue value = data_["table_assoc"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_table, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("table", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// An associative table doesn't have a length.
	EXPECT_EQ(0, value.tablelen());

	// But the list of its keys has.
	EXPECT_EQ(3, value.tablekeys().size());

	// Tables in Lua starts with index 1.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_string, value["a"].type());
	EXPECT_EQ(luadata::luatype::lua_string, value["b"].type());
	EXPECT_EQ(luadata::luatype::lua_string, value["c"].type());

	// Testing access to undefined keys.
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// Testing the values themselves.
	EXPECT_EQ("A", value["a"].asstring());
	EXPECT_EQ("B", value["b"].asstring());
	EXPECT_EQ("C", value["c"].asstring());
}

// Testing a table of tables (non associative).
TEST_F(LuaDataTablesTest, TestNested)
{
	luadata::luavalue value = data_["table_nested"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_table, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("table", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// We should get the length of the outer table.
	EXPECT_EQ(3, value.tablelen());

	// The length of the inner tables should be consistant as well.
	for (size_t i = 1; i <= value.tablelen(); i++)
		EXPECT_EQ(2, value[i].tablelen());

	// The key list size of the outer table should be the same as its length.
	EXPECT_EQ(3, value.tablekeys().size());

	// And same for the inner tables.
	for (size_t i = 1; i <= value.tablelen(); i++)
		EXPECT_EQ(2, value[i].tablekeys().size());

	// Tables in Lua starts with index 1.
	EXPECT_EQ(luadata::luatype::lua_nil, value[0].type());
	EXPECT_EQ(luadata::luatype::lua_table, value[1].type());
	EXPECT_EQ(luadata::luatype::lua_table, value[2].type());
	EXPECT_EQ(luadata::luatype::lua_table, value[3].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value[4].type());

	// Testing access to undefined keys.
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// Testing the values themselves.
	EXPECT_EQ(1, value[1][1].asint());
	EXPECT_EQ(2, value[1][2].asint());
	EXPECT_EQ(3, value[2][1].asint());
	EXPECT_EQ(4, value[2][2].asint());
	EXPECT_EQ(5, value[3][1].asint());
	EXPECT_EQ(6, value[3][2].asint());
}

// Testing a table of tables (associative).
TEST_F(LuaDataTablesTest, TestNestedAssoc)
{
	luadata::luavalue value = data_["table_nested_assoc"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_table, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("table", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it is an associative table, it shouldn't have a length.
	EXPECT_EQ(0, value.tablelen());

	// The length of the inner non-associative tables should be set, tho.
	for (size_t i = 1; i <= value.tablelen(); i++)
		EXPECT_EQ(2, value[i].tablelen());

	// The key list size of the outer table should be set.
	EXPECT_EQ(3, value.tablekeys().size());

	// And same for the inner tables.
	for (size_t i = 1; i <= value.tablelen(); i++)
		EXPECT_EQ(2, value[i].tablekeys().size());

	// Check the types of the inner elements.
	EXPECT_EQ(luadata::luatype::lua_table, value["a"].type());
	EXPECT_EQ(luadata::luatype::lua_table, value["b"].type());
	EXPECT_EQ(luadata::luatype::lua_table, value["c"].type());

	// Testing access to undefined keys.
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"].type());
	EXPECT_EQ(luadata::luatype::lua_nil, value["foo"]["bar"].type());

	// Testing the values themselves.
	EXPECT_EQ("a", value["a"][1].asstring());
	EXPECT_EQ("A", value["a"][2].asstring());
	EXPECT_EQ("b", value["b"][1].asstring());
	EXPECT_EQ("B", value["b"][2].asstring());
	EXPECT_EQ("c", value["c"][1].asstring());
	EXPECT_EQ("C", value["c"][2].asstring());
}

// Testing a deep, "object like" table.
TEST_F(LuaDataTablesTest, TestDeep)
{
	luadata::luavalue value = data_["table_deep"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_table, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("table", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As it is an associative table, it shouldn't have a length.
	EXPECT_EQ(0, value.tablelen());

	// The key list size of the outer table should be set.
	EXPECT_EQ(1, value.tablekeys().size());

	// Testing the deep-nested access.
	EXPECT_EQ("qux", value["foo"]["bar"]["baz"].asstring());
}

// Testing a deep, mixed assoc/non-assoc table.
TEST_F(LuaDataTablesTest, TestDeepMixed)
{
	luadata::luavalue value = data_["table_deep_mixed"];

	// Checking if the type is fine.
	EXPECT_EQ(luadata::luatype::lua_table, value.type());

	// Checking if cast returns expected results.
	EXPECT_EQ(false, value.asbool());
	EXPECT_EQ(0.0, value.asdouble());
	EXPECT_EQ(0, value.asint());
	EXPECT_EQ("table", value.asstring());

	// As it's not a function, it should return itself.
	EXPECT_EQ(value.type(), value().type());
	EXPECT_EQ(value.type(), value("foo").type());

	// As the outer layer is not an associative table, the length should be set.
	EXPECT_EQ(2, value.tablelen());

	// The key list size of the outer table should be set.
	EXPECT_EQ(2, value.tablekeys().size());

	// Testing the deep-nested access.
	EXPECT_EQ(1, value[1]["foo"][1].asint());
	EXPECT_EQ(2, value[1]["foo"][2].asint());
	EXPECT_EQ(3, value[1]["foo"][3].asint());
	EXPECT_EQ(1.23, value[1]["bar"][1].asdouble());
	EXPECT_EQ(4.56, value[1]["bar"][2].asdouble());
	EXPECT_EQ(7.89, value[1]["bar"][3].asdouble());
	EXPECT_EQ("QUX", value[1]["baz"]["qux"].asstring());
	EXPECT_EQ("QUUX", value[1]["baz"]["quux"].asstring());
	EXPECT_EQ(2, value[1]["baz"]["corge"][1].asint());
	EXPECT_EQ(4, value[1]["baz"]["corge"][2].asint());
	EXPECT_EQ(6, value[1]["baz"]["corge"][3].asint());
	EXPECT_EQ(8, value[1]["baz"]["corge"][4].asint());
	EXPECT_EQ(42, value[1]["baz"]["grault"][1].asint());
	EXPECT_EQ("A", value[2][1].asstring());
	EXPECT_EQ("B", value[2][2].asstring());
	EXPECT_EQ("C", value[2][3].asstring());
}

} // namespace
