luadata
=======

Provides an easy way to access data defined in external Lua scripts from C++ code.

Sample
------

Given the following Lua script, named `sample.lua`:

```lua
-- Defining a constant value
answer_to_life = 42

-- Defining a table containing data
npc = {
	name = "John Doe",
	inventory = {
		{"Apple", 3},
		{"Knife", 1}
	}
}

-- Defining a function
area = function(radius)
	return math.pi * (radius ^ 2)
end
```

The data can easily be accessed using the following C++ program:

```c++
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
	std::cout << "The answer to life is "
			  << data["answer_to_life"].asint() << "." << std::endl;
	
	std::cout << "The area of a disk of radius 12 is "
			  << data["area"](12).asdouble() << "." << std::endl;
	
	std::cout << "The name of the NPC is "
			  << data["npc"]["name"].asstring() << "." << std::endl;
	
	std::cout << "He owns the following items:" << std::endl;
	for(auto &it : data["npc"]["inventory"]) {
		std::cout << " - " << it.second[1].asstring()
				  << " (x" << it.second[2].asint() << ")" << std::endl;
	}
	
	std::cout << "Press Enter to continue..." << std::endl;
	std::cin.get();
	return 0;
}
```

You can find this sample program under the `sample` directory in this repository.

How-to use it
-------------

Luadata links as a static library and embed the Lua runtimes. You can build the library for your platform by first generating the makefiles or solution files using Premake (binaries of the lastest version of Premake - which is necessary with this project - are on the repository, in `3rd-party/premake-dev`), and then compile using your prefered build environment (see Premake help).

This project was tested on:

 * Visual Studio 2013
 * Visual Studio 2012
 * GCC 4.8

It doesn't work on:

 * Visual Studio 2010 or earlier (this project uses C++11)
 * GCC 4.7.x on MinGW because of a GCC bug

There is a sample project within the `sample` directory which includes a sample Premake script as well.

License
-------

This plugin is provided under the [MIT Open Source License](http://opensource.org/licenses/MIT). That means you're free
to use it for anything as long as you keep my name and the copyright notice with it. Also, I'm not responsible for
anything you'll do with this plugin.

    Copyright (c) 2012 Samuel Loretan (tynril at gmail dot com)
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

The [Lua](http://www.lua.org/) source code present in the `3rd-party` folder is also under [MIT Open Source License](http://opensource.org/licenses/MIT) license.

    Copyright © 1994–2013 Lua.org, PUC-Rio.
	
The [Google Test](https://code.google.com/p/googletest/) source code also included in the `3rd-party` folder is under [New BSD License](http://opensource.org/licenses/BSD-3-Clause).

The [Premake](https://bitbucket.org/premake/premake-dev) software is under [BSD license](https://bitbucket.org/premake/premake-dev/src/tip/LICENSE.txt?at=default) as well.

    Copyright (C) 2002-2013 by Jason Perkins
