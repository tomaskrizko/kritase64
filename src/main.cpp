#include <iostream>
#include "kritase64.hpp"
#include <memory.h>

int main(int argc, char* argv[])
{
	std::string str = "fo";
	std::cout << str << std::endl;
	std::vector<uint8_t> bytes;
	bytes.resize(str.length());
	memcpy(bytes.data(), str.data(), str.length());
	std::cout << kritase64::encode(bytes) << std::endl;

	return 0;
}