#include <iostream>
#include <string>
#include <thread>

#include <luadata.h>

// TODO :
//  - Remove the implicit cast feature, which sucks
//    - Maybe some template-based return?
//  - Works with copyable/movable/assignable promises to retrieve the value
//    which gets the value at the last possible time instead, to facilitate
//    usage of hot-reload

int main() {

	{
	// Loading a LUA file.
	luadata::luadata test;
	if(!test.loadfile("./data/alltypes.lua", luadata::automatic)) {
		std::cout << "Failed." << std::endl;
		return 1;
	}

	double double_value = test["float_value"].asdouble();
	std::cout << "double_value:                " << double_value << std::endl;

	int integer_value = test["integer_value"].asint();
	std::cout << "integer_value:               " << integer_value << std::endl;

	std::string string_value = test["string_value"].asstring();
	std::cout << "string_value:                " << string_value << std::endl;

	bool boolean_value = test["boolean_value"].asbool();
	std::cout << "boolean_value:               " << boolean_value << std::endl;

	int nil_value_as_int = test["nil_value"].asint();
	double nil_value_as_double = test["nil_value"].asdouble();
	std::string nil_value_as_string = test["nil_value"].asstring();
	bool nil_value_as_bool = test["nil_value"].asbool();
	std::cout << "nil_value:                   " << nil_value_as_int << " (as int)" << std::endl;
	std::cout << "                             " << nil_value_as_double << " (as double)" << std::endl;
	std::cout << "                             " << nil_value_as_string << " (as string)" << std::endl;
	std::cout << "                             " << nil_value_as_bool << " (as bool)" << std::endl;

	int function_value = test["function_value"].asint();
	std::cout << "function_value:              " << function_value << std::endl; 

	int function_returns_a_function = test["function_returns_a_function"].asint();
	std::cout << "function_returns_a_function: " << function_returns_a_function << std::endl;

	int function_three_layers = test["function_three_layers"].asint();
	std::cout << "function_three_layers:       " << function_three_layers << std::endl;

	int function_with_arg_value = test["function_with_arg_value"].asint();
	std::cout << "function_with_arg_value:     " << function_with_arg_value << std::endl;

	luadata::luavalue time_value = test["time_value"];

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "time_value:                  " << time_value.asstring() << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "time_value:                  " << time_value.asstring() << std::endl;

	int integer_value2 = test["integer_value"].asint();
	std::cout << "integer_value:               " << integer_value2 << std::endl;
	
	}
	std::cin.get();
	return 0;
}