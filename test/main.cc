#include <iostream>
#include <string>
#include <thread>
#include <fstream>

#include <luadata.h>

int main() {
	{
	// Loading a LUA file.
	luadata::luadata test;
	if(!test.loadfile("./data/alltypes.lua")) {
		std::cout << "Failed." << std::endl;
		std::cin.get();
		return 1;
	}

	test.loadcode("loaded_from_code = {\"hell\", \"yeah\"}");
	std::cout << "loaded_from_code:              " << test["loaded_from_code"] << std::endl;

	int int_from_array = test["table_list_mixed_value"][0].asint();
	std::cout << "int_from_array:                " << int_from_array << std::endl;

	luadata::luavalue t = test["table_assoc_value"];
	for(luadata::luakey k : t.tablekeys()) {
		std::cout << k << ": " << t[k].asstring() << std::endl;
	}

	luadata::luavalue t2 = test["table_two_layers"];
	for(luadata::luakey k : t2.tablekeys()) {
		std::cout << k << ":" << std::endl;
		for(luadata::luakey k2 : t2[k].tablekeys()) {
			std::cout << "  " << k2 << ": " << t2[k][k2].asstring() << std::endl;
		}
	}

	luadata::luavalue t3 = test["function_returns_table"];
	for(luadata::luakey k : t3.tablekeys()) {
		std::cout << k << ": " << t3[k].asstring() << std::endl;
	}

	luadata::luavalue t4 = test["table_with_functions"];
	for(luadata::luakey k : t4.tablekeys()) {
		std::cout << k << ": " << t4[k].asstring() << std::endl;
	}

	std::cout << test["function_with_arg_value"](2, 3) << std::endl;

	std::string string_from_assoc_array = test["table_assoc_value"]["a"].asstring();
	std::cout << "string_from_assoc_array:        " << string_from_assoc_array << std::endl;

	double double_value = test["float_value"].asdouble();
	std::cout << "double_value:                   " << double_value << std::endl;

	int integer_value = test["integer_value"].asint();
	std::cout << "integer_value:                  " << integer_value << std::endl;

	std::string string_value = test["string_value"].asstring();
	std::cout << "string_value:                   " << string_value << std::endl;

	bool boolean_value = test["boolean_value"].asbool();
	std::cout << "boolean_value:                  " << boolean_value << std::endl;

	int nil_value_as_int = test["nil_value"].asint();
	double nil_value_as_double = test["nil_value"].asdouble();
	std::string nil_value_as_string = test["nil_value"].asstring();
	bool nil_value_as_bool = test["nil_value"].asbool();
	std::cout << "nil_value:                      " << nil_value_as_int << " (as int)" << std::endl;
	std::cout << "                                " << nil_value_as_double << " (as double)" << std::endl;
	std::cout << "                                " << nil_value_as_string << " (as string)" << std::endl;
	std::cout << "                                " << nil_value_as_bool << " (as bool)" << std::endl;

	int function_value = test["function_value"].asint();
	std::cout << "function_value:                 " << function_value << std::endl; 

	int function_returns_a_function = test["function_returns_a_function"].asint();
	std::cout << "function_returns_a_function:    " << function_returns_a_function << std::endl;

	int function_three_layers = test["function_three_layers"].asint();
	std::cout << "function_three_layers:          " << function_three_layers << std::endl;

	int function_with_arg_value_wo_arg = test["function_with_arg_value"]().asint();
	std::cout << "function_with_arg_value_wo_arg: " << function_with_arg_value_wo_arg << std::endl;

	int function_with_arg_value_w_arg = test["function_with_arg_value"](12).asint();
	std::cout << "function_with_arg_value_w_arg:  " << function_with_arg_value_w_arg << std::endl;

	/*
	luadata::luavalue time_value = test["time_value"];

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "time_value:                     " << time_value.asstring() << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(2));
	
	std::cout << "time_value:                     " << time_value.asstring() << std::endl;

	int integer_value2 = test["integer_value"].asint();
	std::cout << "integer_value:                  " << integer_value2 << std::endl;
	*/

	while(true) {
		int integer_value = test["integer_value"].asint();
		std::cout << "integer_value:                  " << integer_value << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		test.hotreload();
	}
	
	}
	std::cin.get();
	return 0;
}