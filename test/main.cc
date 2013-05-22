#include <iostream>
#include <string>

#include <luadata.h>

int main() {

	{
	// Loading a LUA file.
	luadata::luadata test;
	if(!test.loadfile("./data/alltypes.lua", luadata::automatic))
		std::cout << "Failed." << std::endl;

	double double_value = test["float_value"];
	std::cout << "double_value:                " << double_value << std::endl;

	int integer_value = test["integer_value"];
	std::cout << "integer_value:               " << integer_value << std::endl;

	std::string string_value = test["string_value"];
	std::cout << "string_value:                " << string_value << std::endl;

	bool boolean_value = test["boolean_value"];
	std::cout << "boolean_value:               " << boolean_value << std::endl;

	int nil_value_as_int = test["nil_value"];
	double nil_value_as_double = test["nil_value"];
	std::string nil_value_as_string = test["nil_value"];
	bool nil_value_as_bool = test["nil_value"];
	std::cout << "nil_value:                   " << nil_value_as_int << " (as int)" << std::endl;
	std::cout << "                             " << nil_value_as_double << " (as double)" << std::endl;
	std::cout << "                             " << nil_value_as_string << " (as string)" << std::endl;
	std::cout << "                             " << nil_value_as_bool << " (as bool)" << std::endl;

	int function_value = test["function_value"];
	std::cout << "function_value:              " << function_value << std::endl; 

	int function_returns_a_function = test["function_returns_a_function"];
	std::cout << "function_returns_a_function: " << function_returns_a_function << std::endl;

	int function_three_layers = test["function_three_layers"];
	std::cout << "function_three_layers:       " << function_three_layers << std::endl;

	int function_with_arg_value = test["function_with_arg_value"];
	std::cout << "function_with_arg_value:     " << function_with_arg_value << std::endl;
	
	}
	std::cin.get();
	return 0;
}