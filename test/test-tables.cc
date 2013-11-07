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

} // namespace
