#include <iostream>
#include "gtest/gtest.h"

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int testsResult = RUN_ALL_TESTS();

#ifdef _WIN32
	std::cout << "Press Enter to continue..." << std::endl;
	std::cin.get();
#endif
	return testsResult;
}
