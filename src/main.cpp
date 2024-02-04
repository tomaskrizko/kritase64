#include <cstdint>
#include <iostream>
#include "kritase64.hpp"
#include <memory.h>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
	//kritase64::Buffer b;
	//kritase64::Bufferstream stream(b, std::ios::out);
	std::stringstream stream(std::ios::out);
	//stream << 420; // TODO: Broken
	stream << (unsigned char*)"hello world"; // TODO: Figure out why typecast to unsigned char* (aka uint8_t) is necessary.
	/*std::string test = "test";
	stream << test;*/ // TODO: Causes compile error.
	std::cout << kritase64::encode((uint8_t*)stream.str().data(), stream.str().size()) << std::ends;

	return 0;
}
