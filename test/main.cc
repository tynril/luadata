#include <iostream>
#include <string>
#include <thread>

#include <luadata.h>

// TODO:
//  - Implement function call (operator() on luavalue, with a luaarg
//    type wrapping various value types)
//  - Implement hot-reloading
//  - Implement chunk serialization

int main() {
	{
	// Loading a LUA file.
	luadata::luadata test;
	if(!test.loadfile("./data/alltypes.lua", luadata::automatic)) {
		std::cout << "Failed." << std::endl;
		return 1;
	}

	int int_from_array = test["table_list_mixed_value"][0].asint();
	std::cout << "int_from_array:             " << int_from_array << std::endl;

	luadata::luavalue t = test["table_assoc_value"];
	for(std::string s : t.tablekeys()) {
		std::cout << s << ": " << t[s].asstring() << std::endl;
	}

	std::string string_from_assoc_array = test["table_assoc_value"]["a"].asstring();
	std::cout << "string_from_assoc_array:     " << string_from_assoc_array << std::endl;

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

	int function_with_arg_value_wo_arg = test["function_with_arg_value"].asint();
	std::cout << "function_with_arg_value_wo_arg: " << function_with_arg_value_wo_arg << std::endl;

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