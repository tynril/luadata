#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <sys/utime.h>
#else
#include <utime.h>
#endif
#include "luadata.h"
#include "gtest/gtest.h"

namespace {;

// Test loading from source.
TEST(LuaDataLoadingTest, TestLoadFromSource) {
	luadata::luadata data;

	// Loading a first value.
	data.loadcode("a = 12");
	luadata::luavalue aVal = data["a"];
	EXPECT_EQ(12, data["a"].asint());
	EXPECT_EQ(12, aVal.asint());

	// Loading a second value.
	data.loadcode("b = 24");
	EXPECT_EQ(12, data["a"].asint());
	EXPECT_EQ(12, aVal.asint());
	EXPECT_EQ(24, data["b"].asint());

	// Loading a new value for 'a'.
	data.loadcode("a = 42");
	EXPECT_EQ(42, data["a"].asint());
	EXPECT_EQ(42, aVal.asint());
	EXPECT_EQ(24, data["b"].asint());
}

// Test loading from file.
TEST(LuaDataLoadingTest, TestLoadFromFile) {
	luadata::luadata data;

	// Loading a first value.
	data.loadfile("loading_test_1.lua");
	luadata::luavalue aVal = data["a"];
	EXPECT_EQ(12, data["a"].asint());
	EXPECT_EQ(12, aVal.asint());

	// Loading a second value.
	data.loadfile("loading_test_2.lua");
	EXPECT_EQ(12, data["a"].asint());
	EXPECT_EQ(12, aVal.asint());
	EXPECT_EQ(24, data["b"].asint());

	// Loading a new value for 'a'.
	data.loadfile("loading_test_3.lua");
	EXPECT_EQ(42, data["a"].asint());
	EXPECT_EQ(42, aVal.asint());
	EXPECT_EQ(24, data["b"].asint());
}

// Test hot reloading.
TEST(LuaDataLoadingTest, TestHotReload) {
	// Creates a temporary file.
	std::string fileName = "./tmp_hotreload.lua";
	std::fstream stream;

	// Stores something in it.
	stream.open(fileName, std::fstream::out | std::fstream::trunc);
	stream << "a = 12" << std::endl;
	stream.close();

	// Loads the data.
	luadata::luadata data;
	data.loadfile(fileName);

	// The value should be there.
	EXPECT_EQ(12, data["a"].asint());

	// Update the file.
	stream.open(fileName, std::fstream::out | std::fstream::trunc);
	stream << "a = 24" << std::endl;
	stream.close();

	// Force the file time to be updated.
	struct stat st;
	ASSERT_EQ(0, stat(fileName.c_str(), &st));
	utimbuf ut;
	ut.actime = st.st_atime;
	ut.modtime = st.st_mtime + 1;
	ASSERT_EQ(0, utime(fileName.c_str(), &ut));

	// The value should not have changed.
	EXPECT_EQ(12, data["a"].asint());
	
	// Trigger a hot reloading.
	data.hotreload();

	// The value should have been updated.
	EXPECT_EQ(24, data["a"].asint());

	// Removes the file to cleanup.
	std::remove(fileName.c_str());
}

} // namespace
