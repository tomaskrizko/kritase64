#include <cstdint>
#include <iostream>
#include "kritase64.hpp"
#include <memory.h>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
	//std::stringstream stream("s")
	kritase64::Stream stream("aGVsbG8gd29ybGQ=", std::ios::in | std::ios::out | std::ios::ate);

	stream << "hello world";

	std::cout << stream.base64() << std::endl;

	return 0;
}
