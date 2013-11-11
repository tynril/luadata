#include <iostream>
#include "luadata.h"

int main() {
	// Instanciate the luadata library.
	luadata::luadata data;
	
	// Load the file.
	if(!data.loadfile("sample.lua")) {
	  std::cerr << "Unable to load sample.lua." << std::endl;
	  return 1;
	}
	
	// Access the data.
	std::cout << "The answer to life is " << data["answer_to_life"].asint() << "." << std::endl;
	std::cout << "The area of a disk of radius 12 is " << data["area"](12).asdouble() << "." << std::endl;
	std::cout << "The name of the NPC is " << data["npc"]["name"].asstring() << "." << std::endl;
	std::cout << "He owns the following items:" << std::endl;
	for(auto &it : data["npc"]["inventory"]) {
		std::cout << " - " << it.second[1].asstring() << " (x" << it.second[2].asint() << ")" << std::endl;
	}
	
	std::cout << "Press Enter to continue..." << std::endl;
	std::cin.get();
	return 0;
}
