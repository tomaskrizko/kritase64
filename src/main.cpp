#include <iostream>
#include "kritase64.hpp"
#include <memory.h>

int main(int argc, char* argv[])
{
	std::string str = "foob";
	std::cout << str << std::endl;
	std::vector<uint8_t> bytes;
	bytes.resize(str.length());
	memcpy(bytes.data(), str.data(), str.length());
	std::string encoded = kritase64::encode(bytes);
	std::cout << encoded << std::endl;
	bytes = kritase64::decode(encoded);
	str = "";
	for (auto b : bytes)
	{
		str += (char)b;
	}
	std::cout << str << std::endl;

	return 0;
}