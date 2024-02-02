#include <cstdint>
#include <iostream>
#include "kritase64.hpp"
#include <memory.h>
#include <fstream>

int main(int argc, char* argv[])
{
	///*
	std::fstream file(argv[1], std::ios::in | std::ios::binary);

	kritase64::Buffer bytes;
	while (!file.eof())
	{
		uint8_t b;
		file.read((char*)&b, 1);
		if (!file.eof()) bytes.push_back(b);
	}
	std::string base64 = kritase64::encode(bytes);
	std::cout << base64 << std::ends;
	file.close();
	//*/

	///*
	bytes = kritase64::decode(base64);
	file.open("icon.png", std::ios::out | std::ios::trunc | std::ios::binary);
	file.write((char*)bytes.data(), bytes.size());
	file.close();
	//*/

	return 0;
}