#include <cstdint>
#include <iostream>
#include "kritase64.hpp"
#include <memory.h>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
	kritase64::Stream stream;

	stream << "hello world";

	std::cout << stream.base64() << std::endl;

	return 0;
}
